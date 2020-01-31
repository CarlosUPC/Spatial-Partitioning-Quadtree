#pragma once

#include "j1Entity.h"
#include "Animation.h"


struct SDL_Texture;

class ENTITY_DYNAMIC : public j1Entity
{

public:

	ENTITY_DYNAMIC(int xpos, int ypos, ENTITY_TYPE type, std::string name);

	~ENTITY_DYNAMIC();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void OnCollision(Collider*, Collider*);

private:


};
