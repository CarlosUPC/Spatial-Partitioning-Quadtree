#pragma once
#include "p2Defs.h"
#include "SDL/include/SDL.h"
#include <list>


enum direction_quad
{
	NORTHEAST = 0,
	NORTHWEST,
	SOUTHEAST,
	SOUTHWEST
};


template<typename T>
struct QuadData
{
	std::list<T> elements;
};


class QuadNode 
{
public:
	QuadNode();
	~QuadNode();

private:

	SDL_Rect boundary;
	uint BucketSize;
	uint depth;
	bool divided;

	QuadNode* nodes[4];
	QuadNode* callback = nullptr;

	
	QuadData<int> elements;
};



class Quadtree
{
public:

	Quadtree();
	virtual ~Quadtree();
	
	void Split();

	void Insert();

	void Query();
	void Draw();


private:
	
	QuadNode root;

	uint max_depth;
	uint max_bucketSize;

};
