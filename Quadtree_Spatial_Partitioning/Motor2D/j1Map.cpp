#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Window.h"

#include "Brofiler\Brofiler.h"

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.assign("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.assign(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	BROFILER_CATEGORY("Draw", Profiler::Color::Azure);

	if (map_loaded == false)
		return;

	uint id = 0;
	SDL_Rect section = { 0,0,0,0 };
	uint tiles_painted = 0;

	for (std::list<MapLayer*>::iterator layer = data.layers.begin(); layer != data.layers.end(); ++layer) {
		if ((*layer)->visible && (*layer)->properties.draw) {
			for (uint i = 0; i < (*layer)->width; ++i) {
				for (uint j = 0; j < (*layer)->height; ++j) {

					if (App->render->IsOnCamera(MapToWorld(i, j).x, MapToWorld(i, j).y, data.tile_width, data.tile_height)) {
						id = (*layer)->Get(i, j);
						if (id != 0) {
							TileSet* tileset = GetTilesetFromTileId(id);
							if (tileset != nullptr) {

								tiles_painted++;
								App->render->Blit(tileset->texture, MapToWorld(i, j).x, MapToWorld(i, j).y, &tileset->GetTileRect(id));
							}
						}
					}
				}
			}
		}
	}
	if (draw_grid) {
		for (uint i = 0; i < data.width; ++i) {
			for (uint j = 0; j < data.height; ++j) {
				App->render->Blit(grid, MapToWorld(i, j).x, MapToWorld(i, j).y);
			}
		}
	}

}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	std::list<TileSet*>::const_iterator item = data.tilesets.begin();
	TileSet* ret = *item;
	for (; item != data.tilesets.end(); ++item) {
		if (id >= (*item)->firstgid)
			ret = *item;
		else return ret;
	}
	return ret;
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret(0, 0);


	switch (data.type) {
	case MAPTYPE_ORTHOGONAL:
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
		break;
	case MAPTYPE_ISOMETRIC:
		ret.x = int(data.tile_width * 0.5F) * (x - y);
		ret.y = int(data.tile_height * 0.5F) * (x + y);
		break;
	default:
		LOG("Map type unknown");
		break;
	}

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const {
	iPoint ret(0, 0);

	switch (data.type) {
	case MAPTYPE_ORTHOGONAL:
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
		break;
	case MAPTYPE_ISOMETRIC:
		ret.x = x / data.tile_width + y / data.tile_height;
		ret.y = 2 * y / data.tile_height - ret.x;
		ret.x--; ret.y--;
		break;
	default:
		LOG("Map type unknown");
		break;
	}

	return ret;
}


inline uint MapLayer::Get(int x, int y) const {
	return tiles[y * this->width + x];
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	std::list<TileSet*>::iterator item = data.tilesets.begin();

	while (item != data.tilesets.end())
	{
		App->tex->UnLoad((*item)->texture);
		RELEASE(*item);
		++item;
	}
	data.tilesets.clear();

	// Remove all layers
	std::list<MapLayer*>::iterator item2 = data.layers.begin();

	while (item2 != data.layers.end()) {
		RELEASE(*item2);
		++item2;
	}
	data.layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	std::string tmp = (folder.data() + std::string(file_name));

	pugi::xml_parse_result result = map_file.load_file(tmp.data());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.push_back(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer")) {

		MapLayer* lay = new MapLayer();

		if (ret == true)
			ret = LoadLayer(layer, lay);

		data.layers.push_back(lay);
	}

	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

	}

	if (data.type == MAPTYPE_ISOMETRIC)
		grid = App->tex->Load("maps/Quad_Iso.png");
	else if (data.type == MAPTYPE_ORTHOGONAL)
		grid = App->tex->Load("maps/Quad_Ortho.png");


	map_file.reset();
	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		LoadLayerProperties(map.child("properties"));
		std::string bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if (bg_color.length() > 0)
		{
			std::string red;
			std::string green;
			std::string blue;
			bg_color.substr(1, 2);
			bg_color.substr(3, 4);
			bg_color.substr(5, 6);

			int v = 0;

			sscanf_s(red.data(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.data(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.data(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.b = v;
		}

		std::string orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}

		LoadMapProperties(map.child("properties"));
	}

	return ret;
}

bool j1Map::LoadMapProperties(pugi::xml_node& node)
{
	for (node = node.child("property"); node != NULL; node = node.next_sibling()) {
		std::string prop = node.attribute("name").as_string();

		if (prop == "object_texture") {
			data.properties.objects_path = node.attribute("value").as_string();
		}
	}
	return true;
}

void j1Map::LoadLayerProperties(pugi::xml_node& properties_node, MapLayer* layer) {

	for (properties_node = properties_node.child("property"); properties_node != NULL; properties_node = properties_node.next_sibling()) {
		std::string prop = properties_node.attribute("name").as_string();

		if (prop == "NoDraw") {
			layer->properties.draw = properties_node.attribute("value").as_bool(true);
		}
	}
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.assign(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();


	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.data(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if (set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if (set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	if (node.empty())	ret = false;

	layer->name.assign(node.attribute("name").as_string());
	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();
	layer->visible = node.attribute("visible").as_bool(true);

	//Load properties of layer
	pugi::xml_node n_property = node.child("properties");
	LoadLayerProperties(n_property, layer);

	layer->tiles = new uint[layer->width * layer->height];

	memset(layer->tiles, 0, layer->width * layer->height * sizeof(uint));

	pugi::xml_node tile = node.child("data").child("tile");

	for (uint i = 0; i < layer->width * layer->height; i++) {
		layer->tiles[i] = tile.attribute("gid").as_uint();
		tile = tile.next_sibling("tile");
	}

	return ret;
}
