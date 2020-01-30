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
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	if(App->map->Load("iso.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if(App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png");


	uint w, h;
	App->win->GetWindowSize(w, h);
	uint capacity = 4;
	uint depth = 4;
	SDL_Rect map;
	map.x = -App->map->data.width * App->map->data.tile_width / 2;
	map.y = 0;
	map.w = App->map->data.width * App->map->data.tile_width;
	map.h = App->map->data.height * App->map->data.tile_height;

	//coll = new SDL_Rect();
	

	qtree = new Quadtree<SDL_Rect>({ -App->render->camera.x,0,(int)w,(int)h }, capacity, depth);
	//qtree->Split();
	return true;
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
		App->render->camera.y += 1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		App->win->ZoomOut();

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_UP) {
		qtree->Insert(coll = new SDL_Rect({50,50,300,200}));
		colliders.push_back(coll);
	}



	for (std::list<SDL_Rect*>::iterator it = colliders.begin(); it != colliders.end(); it++) 
	{
		found.clear();
		qtree->Query(found, *it);

		if (found.size() > 0)
		{
			LOG("Colliders near found: %i", found.size());
		}
	
	}


	App->map->Draw();
	qtree->Draw();
	
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