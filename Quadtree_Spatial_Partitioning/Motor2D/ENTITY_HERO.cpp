#include "j1App.h"
#include "ENTITY_HERO.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Collision.h"

ENTITY_HERO::ENTITY_HERO(int xpos, int ypos, ENTITY_TYPE type, std::string name) : j1Entity(xpos, ypos, type, name)
{
	this->Start();
}

ENTITY_HERO::~ENTITY_HERO()
{
}

bool ENTITY_HERO::Start()
{
	texture = App->tex->Load("entities/hero.png");

	anim_run.PushBack({ 17,33,27,30 });
	anim_run.PushBack({ 81,36,27,28 });
	anim_run.speed = 4.5;
	anim_run.loop = true;

	anim_run.PushBack({ 16,82,28,31 });
	anim_run.PushBack({ 83,83,23,30 });
	anim_run.PushBack({ 149,82,21,31 });
	anim_run.PushBack({ 213,81,28,32 });
	anim_run.speed = 6;
	anim_run.loop = false;


	pivot.create(27 / 2, 25);
	size.create(27, 30);

	current_anim = &anim_run;

	collider = App->collision->AddCollider({ (int)position.x, (int)position.y,size.x,size.y }, COLLIDER_TYPE::COLLIDER_ENTITY, this);
	
	return true;
}

bool ENTITY_HERO::Update(float dt)
{
	
	current_anim->GetCurrentFrame(dt);

	
	iPoint enemy_pos = App->map->WorldToMap(position.x, position.y);
	bool exist = false;
	iPoint objective;

	if (left)
		objective = iPoint(enemy_pos.x - 1, enemy_pos.y - 1);
	else
		objective = iPoint(enemy_pos.x + 1, enemy_pos.y + 1);

	if (App->map->data.width - 1 < objective.x || App->map->data.height - 1 < objective.y || objective.x == -1 || objective.y == -1)
		left = !left;

	if (App->map->data.width - 1 >= objective.x && App->map->data.height - 1 >= objective.y && objective.x >= -1 && objective.y >= -1)
	{
		if (left)
		{
			position.x -= 20 * dt;
			position.y -= 20 * dt;
			flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
		}
		else
		{
			position.x += 20 * dt;
			position.y += 20 * dt;
			flip = SDL_RendererFlip::SDL_FLIP_NONE;
		}
	}
	if (current_anim->Finished())
		current_anim = &anim_run;

	return true;
}

// Called before quitting
bool ENTITY_HERO::CleanUp()
{
	return App->tex->UnLoad(texture);

	if (collider != nullptr)
		collider->to_delete = true;

	current_anim = nullptr;

}

void ENTITY_HERO::OnCollision(Collider* c1, Collider* c2)
{}

