#include "j1EntityManager.h"
#include "p2Log.h"
#include "j1Entity.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "j1Render.h"
#include "ENTITY_HERO.h"
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
	//times_per_sec = TIMES_PER_SEC;
	//update_ms_cycle = 1.0f / (float)times_per_sec;

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

	//do_logic = false;
	return true;
}


bool j1EntityManager::Update(float dt) {

	/*
	accumulated_time += dt;

	if (accumulated_time >= update_ms_cycle)
		do_logic = true;

	if (do_logic == true) {

		if (entities.size() > 0) {

			std::list<j1Entity*>::iterator item = entities.begin();
			for (; *item != nullptr; item = next(item))
				(*item)->FixUpdate(dt);
		}
	}

	accumulated_time -= update_ms_cycle;

	if (entities.size() > 0) {

		std::list<j1Entity*>::iterator item = entities.begin();
		for (; *item != nullptr; item = next(item))
			(*item)->Update(dt);
	}
	*/

	uint numEntities = 0;
	//std::list<j1Entity*> draw_entities;

	for (std::list<j1Entity*>::iterator item = entities.begin(); item != entities.end(); ++item) {
		
		if (*item != nullptr) 
		{
			++numEntities;
			(*item)->Update(dt);
			(*item)->Draw();
		
			/*if (App->render->IsOnCamera((*item)->position.x, (*item)->position.y, (*item)->size.x, (*item)->size.y)) 
				draw_entities.push_back(*item);*/
			
		}
	}

	//std::sort(draw_entities.begin(), draw_entities.end(), j1EntityManager::SortByYPos);


	/*for (std::list<j1Entity*>::iterator item = draw_entities.begin(); item != draw_entities.end(); ++item) {

	}

	draw_entities.clear();*/

	static char title[30];
	sprintf_s(title, 30, " | Entities: %i", numEntities);
	App->win->ConcatTitle(title);

	
	numEntities = 0;
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

	static_assert(ENTITY_TYPE::UNKNOWN == ENTITY_TYPE(1), "UPDATE ENTITY TYPES");
	
	j1Entity* entity = nullptr;

	switch (type) {

		case ENTITY_TYPE::ENT_HERO:
			entity = new ENTITY_HERO(xpos, ypos,  type,  name);
			break;

		default:
			break;

	}

	entities.push_back(entity);
	return entity;
}


void j1EntityManager::DestroyEntity(j1Entity* entity) 
{
	for (std::list<j1Entity*>::iterator item = entities.begin(); item != entities.end(); ++item) 
	{	
		if ((*item) == entity) 
		{	
			//(*item)->DestroyEntity();
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
