#ifndef __j1MAP_H__
#define __j1MAP_H__

#include <list>
#include <string>
#include "j1Module.h"
#include "j1Collision.h"

struct SDL_Texture;

// ----------------------------------------------------

struct LayerProperties
{
	bool draw = true;
};

struct MapProperties
{
	std::string objects_path;
};

struct MapLayer {
	std::string	name;
	uint		width = 0;
	uint		height = 0;
	uint* tiles = nullptr;
	bool		visible = true;
	LayerProperties	properties;

	~MapLayer() { delete[] tiles; tiles = nullptr; }

	inline uint Get(int x, int y) const;
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	std::string			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture* texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;

};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int							width;
	int							height;
	int							tile_width;
	int							tile_height;
	MapProperties				properties;
	SDL_Color					background_color;
	MapTypes					type;
	std::list<TileSet*>			tilesets;
	std::list<MapLayer*>		layers;
	std::list<Collider*>		mapColliders;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;

	//void AddCollidersMap();


private:

	bool LoadMap();
	bool LoadMapProperties(pugi::xml_node& node);
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& layer_node, MapLayer* layer);
	bool LoadObjects(pugi::xml_node& layer_node, SDL_Rect& rect);
	void LoadLayerProperties(pugi::xml_node& properties_node, MapLayer* layer = nullptr);

	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData			data;
	std::string		sceneName;

	bool			draw_grid = false;

private:

	pugi::xml_document	map_file;
	std::string			folder;
	bool				map_loaded;
	SDL_Texture* grid = nullptr;

};

#endif // __j1MAP_H__