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


bool j1Collision::Awake(pugi::xml_node& config)
{
	LOG("Awaking Collision");

	// TODO 0: Fill the following variables from config.xml file provided in the project using pugi API code.You can editand configure the values anytime you want to test Quadtree performance.

	return true;
}

bool j1Collision::Start()
{
	LOG("Loading Collision");

	//TODO 1: allocate quadtree memory using qtree pointer declared alreadyand fill the parameter fields using the values you previously defined in TODO0.

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

	// TODO 5:
	// Call Quadtree Draw function in the update.You can use the boolean "debugQT" to make a condition that when this boolean becomes true, Draw is called.
	// To wrap up, remeber to call Quadtree CleanUp functionand delete qtree pointer to free its memory.
	

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


	// TODO 5:
	// Call Quadtree Draw function in the update.You can use the boolean "debugQT" to make a condition that when this boolean becomes true, Draw is called.
	// To wrap up, remeber to call Quadtree CleanUp functionand delete qtree pointer to free its memory.
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

		App->render->DrawQuad((*it)->rect, 240, 255, 0, 255, false);
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

	
	// TODO 3:
	// Use your done Insert() method to add all the elements contained in screen each update and clean it!


	// TODO 4:
	// Just make the same functionality you can see in BruteForceChecking() function but changing the logic of second loop a bit.We need to call the Query() 
	// function to retrieve all of the elements from each bucket into an array(you can use "found"std::list from Quadtree container).
	// Once you get it, iterate it in second loop to check collisions as BruteForce funcion does.Remember to clean the array with retrieved elements because we are doing this every frame!

	quadTreeChecks = 0;

	for (std::list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); it++)
	{
	}

	quadTreeTime = quadTreeTimer.ReadMs();
	return quadTreeTime;
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (bool)SDL_HasIntersection(&rect, &r);
}