#ifndef _J1_ENTITY_MANAGER
#define _J1_ENTITY_MANAGER

#include "j1Module.h"
#include "j1Entity.h"
#include "PugiXml\src\pugixml.hpp"

#define TIMES_PER_SEC 5

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

public:

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

public:

	j1Entity *CreateEntity(entity_type entityType);
	void DestroyEntity(j1Entity *Entity);

public:

	std::list<j1Entity*> entities_list;

	float accumulated_time = 0.0f;
	float update_ms_cycle = 0.0f;
	int times_per_sec = 0;
	bool do_logic = false;

};

#endif