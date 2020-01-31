#pragma once

#include "j1Module.h"
#include "j1Entity.h"
#include "PugiXml\src\pugixml.hpp"

#define TIMES_PER_SEC 5

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:

	j1Entity *CreateEntity(int PositionX, int PositionY,ENTITY_TYPE entityType, std::string name);
	void DestroyEntity(j1Entity *Entity);
	static bool SortByYPos(const j1Entity& ent1, const j1Entity& ent2);

public:

	std::list<j1Entity*> entities;

	float accumulated_time = 0.0f;
	float update_ms_cycle = 0.0f;
	int times_per_sec = 0;
	bool do_logic = false;

};

