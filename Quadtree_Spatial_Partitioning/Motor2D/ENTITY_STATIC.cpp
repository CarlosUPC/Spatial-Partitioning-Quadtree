#include "j1App.h"
#include "ENTITY_STATIC.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Collision.h"

ENTITY_STATIC::ENTITY_STATIC(int xpos, int ypos, ENTITY_TYPE type, std::string name) : j1Entity(xpos, ypos, type, name)
{
	this->Start();
}

ENTITY_STATIC::~ENTITY_STATIC()
{
}

bool ENTITY_STATIC::Start()
{
	texture = App->tex->Load("entities/static.png");

	anim_run.PushBack({ 17,33,27,30 });
	anim_run.PushBack({ 81,36,27,28 });
	anim_run.speed = 4.5;
	anim_run.loop = true;

	anim_coll.PushBack({ 16,82,28,31 });
	anim_coll.PushBack({ 83,83,23,30 });
	anim_coll.PushBack({ 149,82,21,31 });
	anim_coll.PushBack({ 213,81,28,32 });
	anim_coll.speed = 6;
	anim_coll.loop = false;


	pivot.create(27 / 2, 25);
	size.create(27, 30);

	current_anim = &anim_run;

	collider = App->collision->AddCollider({ (int)position.x, (int)position.y,size.x,size.y }, COLLIDER_TYPE::COLLIDER_ENTITY, this);

	return true;
}

bool ENTITY_STATIC::Update(float dt)
{

	current_anim->GetCurrentFrame(dt);


	if (current_anim->Finished())
		current_anim = &anim_run;


	return true;
}

// Called before quitting
bool ENTITY_STATIC::CleanUp()
{
	App->tex->UnLoad(texture);

	if (this->collider != nullptr)
		this->collider->to_delete = true;

	current_anim = nullptr;

	return true;
}

void ENTITY_STATIC::OnCollision(Collider* c1, Collider* c2)
{
}
