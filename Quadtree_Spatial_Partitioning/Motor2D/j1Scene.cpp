#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1PathFinding.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include <time.h> 

j1Scene::j1Scene() : j1Module()
{
	name.assign("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Awaking Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	
	LOG("Loading Scene");
	bool ret = true;
	srand(time(NULL));

	ret = App->map->Load("iso.tmx"); 
	App->render->SetCameraPos(App->map->data.width * App->map->data.tile_width / 2 - 100, 100);

	return ret;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 300 * dt;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 300 * dt;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 300 * dt;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 300 * dt;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		App->win->ZoomOut();

	
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint coords = App->render->ScreenToWorld(x, y);

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) 
		App->entities->CreateEntity(coords.x, coords.y, ENTITY_TYPE::ENT_STATIC, "static_hero");

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) 
		App->entities->CreateEntity(coords.x, coords.y, ENTITY_TYPE::ENT_DYNAMIC, "dynamic_hero");

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		for (int i = 0; i < 1000; ++i)
		{
			iPoint coords = App->render->ScreenToWorld(rand() % 1500 + 10 , rand() % 880 + 130);
			App->entities->CreateEntity(coords.x, coords.y, ENTITY_TYPE::ENT_DYNAMIC, "dynamic_hero");
		}
	}
		
	
	App->map->Draw();
	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	
	return true;
}