#include "j1App.h"
#include "j1Entity.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "j1Collision.h"
#include "j1Window.h"
#include "j1EntityManager.h"


j1Entity::j1Entity(int xpos, int ypos, ENTITY_TYPE type, std::string name) : position(xpos, ypos), name(name), type(type)
{}

j1Entity::~j1Entity()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

bool j1Entity::Awake(pugi::xml_node&)
{
	return true;
}

bool j1Entity::Start()
{
	return true;
}

bool j1Entity::Update(float dt) 
{
	return true;
}

bool j1Entity::PreUpdate()
{
	return true;
}

bool j1Entity::PostUpdate()
{
	return true;
}

void j1Entity::Draw() {

	collider->SetPos(position.x, position.y);

	if (current_anim != nullptr)
		App->render->Blit(texture, position.x, position.y, &current_anim->frames[current_anim->GetNumCurrentFrame()], 1.0F, true, flip);
}

void j1Entity::Move(float dt)
{
}

void j1Entity::DestroyEntity()
{
	
}

void j1Entity::OnCollision(Collider* collider1, Collider* collider2)
{
	if (anim_coll.Finished()) {
		anim_coll.reset();
		
	}

	current_anim = &anim_coll;
}


bool j1Entity::CleanUp()
{
	bool ret = false;

	ret = App->tex->UnLoad(texture);

	if (collider != nullptr)
		collider->to_delete = true;

	current_anim = nullptr;


	return ret;
}


//Functions to help loading data in xml-------------------------------------
//Get the rect info of an id of tileset
SDL_Rect TileSetEntity::GetTileRect(int id) const {
	SDL_Rect rect;
	rect.w = tilewidth;
	rect.h = tileheight;
	rect.x = margin + ((rect.w + spacing) * (id % columns));
	rect.y = margin + ((rect.h + spacing) * (id / columns));
	return rect;
}
