#include "j1EntityManager.h"
#include "p2Log.h"
#include "j1Entity.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "j1Render.h"
#include "ENTITY_DYNAMIC.h"
#include "ENTITY_STATIC.h"
#include <algorithm>

j1EntityManager::j1EntityManager()
{
	name.assign("entities");
}


j1EntityManager::~j1EntityManager()
{
}


bool j1EntityManager::Awake(pugi::xml_node&) {

	LOG("Awaking all entities");
	

	return true;
}


bool j1EntityManager::Start() {

	LOG("Loading all entities");

	for (std::list<j1Entity*>::iterator item = entities.begin(); item != entities.end(); ++item)
	{
		if ((*item) != nullptr) 
			(*item)->Start();
	}

	return true;
}


bool j1EntityManager::PreUpdate() {

	
	return true;
}


bool j1EntityManager::Update(float dt) {

	
	std::list<j1Entity*> draw_entities;

	for (std::list<j1Entity*>::iterator item = entities.begin(); item != entities.end(); ++item) {
		
		if (*item != nullptr) 
		{
			
			(*item)->Update(dt);
		
			if (App->render->IsOnCamera((*item)->position.x, (*item)->position.y, (*item)->size.x, (*item)->size.y)) 
				draw_entities.push_back(*item);
			
		}
	}

	for (std::list<j1Entity*>::iterator item = draw_entities.begin(); item != draw_entities.end(); ++item) 
	{
		(*item)->Draw();
	}


	static char title[30];
	sprintf_s(title, 30, " | Entities drawn: %i", draw_entities.size());
	App->win->ConcatTitle(title);

	
	draw_entities.clear();
	
	return true;
}

bool j1EntityManager::PostUpdate()
{
	return true;
}

bool j1EntityManager::CleanUp() {

	LOG("Clean Up all entities");

	for (std::list<j1Entity*>::iterator item = entities.begin(); item != entities.end(); ++item)
	{
		if ((*item) != nullptr) {
			(*item)->CleanUp();
			RELEASE(*item);
		}
	}
	entities.clear();

	return true;
}


j1Entity *j1EntityManager::CreateEntity(int xpos, int ypos, ENTITY_TYPE type, std::string name) {

	
	j1Entity* entity = nullptr;

	switch (type) {

		case ENTITY_TYPE::ENT_DYNAMIC:
			entity = new ENTITY_DYNAMIC(xpos, ypos,  type,  name);
			break;
		case ENTITY_TYPE::ENT_STATIC:
			entity = new ENTITY_STATIC(xpos, ypos, type, name);
			break;

		default:
			break;

	}

	entities.push_back(entity);
	return entity;
}


void j1EntityManager::DeleteEntity(j1Entity* entity) 
{
	for (std::list<j1Entity*>::iterator item = entities.begin(); item != entities.end(); ++item) 
	{	
		if ((*item) == entity) 
		{	
			RELEASE(*item);
			*item = nullptr;
			entities.erase(item);
			break;	
		}
	}
}


bool j1EntityManager::SortByYPos(const j1Entity& ent1, const j1Entity& ent2)
{
	return ent1.pivot.y + ent1.position.y < ent2.pivot.y + ent2.position.y;
}
