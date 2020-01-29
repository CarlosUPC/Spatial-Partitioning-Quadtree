#ifndef _J1_ENTITY_
#define _J1_ENTITY_

#include "p2Point.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1App.h"

#include <string>

class j1EntityManager;
struct SDL_Texture;
struct SDL_Rect;

enum class entity_type {

	OBJECT_ENT,
	UNKNOWN = 1
};

class j1Entity {

public:

	j1Entity(entity_type entityType) : type(entityType) {}
	~j1Entity() {}

public:

	// Called before render is available
	virtual void Awake() {}

	// Called before the first frame if it was activated before that
	virtual void Start() {}

	// Called each loop iteration
	virtual void FixUpdate(float dt) {}

	// Called each logic iteration
	virtual void Update(float dt) {}

	// Called before all Updates
	virtual void PreUpdate() {}

	// Called before all Updates
	virtual void PostUpdate() {}

	// Called before quitting
	virtual void CleanUp() {}

	virtual void DestroyEntity() {}

	entity_type GetType() const { return type; }
	std::string GetName() const { return name; }

public:

	virtual void Move(float dt) {}
	virtual void Draw(float dt) {}

public:

	int life = 0;
	bool active = false;
	bool mustDestroy = false;
	entity_type type;
	std::string name;

	fPoint position;
	SDL_Texture *texture = nullptr;

};

#endif