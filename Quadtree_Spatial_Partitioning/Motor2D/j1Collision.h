#pragma once

#include "j1Module.h"
#include "j1PerfTimer.h"
#include "Quadtree.h"
#include "SDL\include\SDL.h"
#include <list>

class j1Entity;

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_ENTITY,

	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Entity* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Entity* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class j1Collision : public j1Module
{
public:

	j1Collision();
	virtual ~j1Collision();

	// Called when before render is available
	bool Awake(pugi::xml_node& config);

	// Call before first frame
	bool Start();

	// Called before all updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Entity* callback = nullptr);
	void DeleteCollider(Collider* collider);
	void DebugDraw();

	double BruteForceChecking();
	double QuadTreeChecking();

private:

	bool enableQT = false;
	bool debugCollider = false;
	bool debugQT = false;

	std::list<Collider*> colliders;
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];

	double bruteForceTime; double quadTreeTime;
	int bruteForceChecks;int quadTreeChecks;
	j1PerfTimer bruteForceTimer;j1PerfTimer quadTreeTimer;

	
	//-----------Quadtree-------------// 
	Quadtree<Collider>* qtree = nullptr;
	SDL_Rect qtree_rect;
	uint capacity;
	uint depth;

	
};




