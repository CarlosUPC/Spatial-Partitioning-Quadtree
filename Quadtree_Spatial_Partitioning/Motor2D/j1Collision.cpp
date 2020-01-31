#include "j1Collision.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Entity.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "j1EntityManager.h"
#include "j1Map.h"


j1Collision::j1Collision() : j1Module()
{
	name.assign("collision");

	matrix[COLLIDER_ENTITY][COLLIDER_ENTITY] = true;

}


j1Collision::~j1Collision()
{
}


bool j1Collision::Awake(pugi::xml_node&)
{
	return true;
}

bool j1Collision::Start()
{
	
	uint w, h;
	App->win->GetWindowSize(w, h);

	uint capacity = 4;
	uint depth = 4;
	qtree = new Quadtree<Collider>({ -App->render->camera.x - 60,App->render->camera.y - 90, App->map->data.width * App->map->data.tile_width, App->map->data.height * App->map->data.tile_height }, capacity, depth);

	return true;
}

bool j1Collision::PreUpdate()
{
	for (std::list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); it++) {
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = colliders.erase(it);
		}
	}

	return true;
}

bool j1Collision::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		debugCollider = !debugCollider;
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		enableQT = !enableQT;
	}

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		debugQT = !debugQT;
	}

	

	if (!enableQT) //-------- BRUTE FORCE METHOD ---------//
	{
		bruteForceTime = BruteForceChecking(); 

		static char title[350];
		sprintf_s(title, "Entities: %d | Colliders: %d | Brute force iterations: %d Time: %.5f ms | FPS: %.2f ", App->entities->entities.size() , colliders.size(), bruteForceChecks, bruteForceTime, App->GetFPS());
		App->win->SetTitle(title);

	}
	else //-------- QUADTREE METHOD ---------//
	{
		quadTreeTime = QuadTreeChecking(); 

		static char title[350];
		sprintf_s(title, "Entities: %d | Colliders: %d | Quadtree iterations: %d Time: %.5f ms | FPS: %.2f ", App->entities->entities.size(), colliders.size(), quadTreeChecks, quadTreeTime, App->GetFPS());
		App->win->SetTitle(title);

	}

	if (debugCollider) 
		DebugDraw();

	if(debugQT)
		qtree->Draw();

	return true;
}

bool j1Collision::CleanUp()
{
	LOG("Freeing colliders");
	std::list<Collider*>::reverse_iterator it = colliders.rbegin();

	while (it != colliders.rend())
	{
		RELEASE(*it);
		++it;
	}

	colliders.clear();


	qtree->CleanUp();
	delete qtree;
	qtree = nullptr;

	return true;
}

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Entity* callback)
{
	Collider* ret = new Collider(rect, type, callback);
	colliders.push_back(ret);

	return ret;
}

void j1Collision::DeleteCollider(Collider* collider)
{
	collider->to_delete = true;
}


void j1Collision::DebugDraw()
{

	for (std::list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); it++)
	{
		if ((*it) == nullptr) {
			continue;
		}

		App->render->DrawQuad((*it)->rect, 255, 255, 255, 255, false);
	}
}

double j1Collision::BruteForceChecking()
{

	double bruteForceTime;
	Collider* c1; Collider* c2;
	bruteForceChecks = 0;

	bruteForceTimer.Start();

	for (std::list<Collider*>::iterator col1 = colliders.begin(); col1 != colliders.end(); col1++) {
		c1 = (*col1);

		for (std::list<Collider*>::iterator col2 = colliders.begin(); col2 != colliders.end(); col2++) {
			c2 = (*col2);
			if (c1 != c2) {
				if (c1->CheckCollision(c2->rect) == true) {
					if (matrix[c1->type][c2->type] && c1->callback)
						c1->callback->OnCollision(c1, c2);

					if (matrix[c2->type][c1->type] && c2->callback)
						c2->callback->OnCollision(c2, c1);
				}
				bruteForceChecks++;
			}
		}
	}

	 bruteForceTime = bruteForceTimer.ReadMs();

	 return bruteForceTime;
}

double j1Collision::QuadTreeChecking()
{
	double quadTreeTime;
	quadTreeTimer.Start();

	
	/*if (updateQtree)
	{*/
		qtree->CleanUp();

		for (std::list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); it++) 
			qtree->Insert(*it);

		//updateQtree = false;

	//}
	

	quadTreeChecks = 0;

	for (std::list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); it++)
	{
		qtree->found.clear();
		qtree->Query(qtree->found, *it);

		if (qtree->found.size() > 0)
		{
			LOG("Colliders near found: %i", qtree->found.size());

			for (std::list<Collider*>::iterator it2 = qtree->found.begin(); it2 != qtree->found.end(); it2++) 
			{
				if ((*it)->CheckCollision((*it2)->rect)) {

					if (matrix[(*it)->type][(*it2)->type] && (*it)->callback)
						(*it)->callback->OnCollision((*it), (*it2));

					if (matrix[(*it2)->type][(*it)->type] && (*it2)->callback)
						(*it2)->callback->OnCollision((*it2), (*it));

				}
				quadTreeChecks++;
				
				App->render->DrawQuad((*it2)->rect, 0, 255, 0, 255, false);
				App->render->DrawLine((*it)->rect.x + (*it)->rect.w / 2, (*it)->rect.y + (*it)->rect.h / 2, (*it2)->rect.x + (*it2)->rect.w / 2, (*it2)->rect.y + (*it2)->rect.h / 2, 255, 255, 255, 255);
			}

		}

	}

	quadTreeTime = quadTreeTimer.ReadMs();
	return quadTreeTime;
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (bool)SDL_HasIntersection(&rect, &r);
}