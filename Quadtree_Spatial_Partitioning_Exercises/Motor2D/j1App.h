#ifndef __j1APP_H__
#define __j1APP_H__

#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"

#include "j1Timer.h"
#include <list>

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Scene;
class j1Map;
class j1PathFinding;
class j1Collision;
class j1EntityManager;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;
	float GetDT() const;
	float GetFPS() const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

public:

	// Modules
	j1Window*			win = nullptr;
	j1Input*			input = nullptr;
	j1Render*			render = nullptr;
	j1Textures*			tex = nullptr;
	j1Scene*			scene = nullptr;
	j1Map*				map = nullptr;
	j1PathFinding*		pathfinding = nullptr;
	j1Collision*		collision = nullptr;
	j1EntityManager*	entities = nullptr;

private:

	int					argc;
	char**				args;

	std::string			title;
	std::string			organization;

	std::list<j1Module*>modules;


	uint32				framerate_cap = 0;
	bool				cap_framerate = true;
	
	uint64				frame_count = 0;
	j1Timer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	uint				frames;
	float					dt;
	float               avg_fps;
};

extern j1App* App;

#endif