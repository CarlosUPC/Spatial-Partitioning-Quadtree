#ifndef _J1_ENTITY_
#define _J1_ENTITY_

#include "p2Point.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1App.h"

#include <string>
#include "Animation.h"
#include "SDL_image/include/SDL_image.h"

class j1EntityManager;
struct SDL_Texture;
struct SDL_Rect;

struct TileSetEntity {

	SDL_Rect GetTileRect(int id) const;

	std::string name;
	uint tilewidth = 0;
	uint tileheight = 0;
	uint spacing = 0;
	uint margin = 0;
	uint tilecount = 0;
	uint columns = 0;
	std::string imagePath;
	SDL_Texture* texture = nullptr;
	uint width = 0;
	uint height = 0;
};

enum class ENTITY_STATE {
	IDLE = 0,
	WALKING,

	UNKNOWN
};

enum class ENTITY_TYPE {

	ENT_HERO,
	UNKNOWN = 1
};

class j1Entity {

public:

	j1Entity(int xpos, int ypos, ENTITY_TYPE type, std::string name);
	virtual ~j1Entity();

public:

	
	virtual bool Awake(pugi::xml_node&);
	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool PreUpdate();
	virtual bool PostUpdate();
	virtual bool CleanUp();
	virtual void Draw();
	virtual void Move(float dt);

	virtual void DestroyEntity();

	void OnCollision(Collider* col1, Collider* col2);

	ENTITY_TYPE GetType() const { return type; }
	std::string GetName() const { return name; }
	Collider* GetCollider() const { return collider; }


public:

	int life = 0;
	bool active = false;
	bool mustDestroy = false;

	ENTITY_TYPE type;
	std::string name;
	Collider* collider;

	fPoint position;
	iPoint pivot = { 0, 0 };
	iPoint size = { 0, 0 };

	SDL_RendererFlip flip = SDL_RendererFlip::SDL_FLIP_NONE;

	Animation anim_run;
	Animation anim_coll;
	Animation* current_anim = nullptr;
	SDL_Texture *texture = nullptr;

protected:
	bool left = false;

};

#endif