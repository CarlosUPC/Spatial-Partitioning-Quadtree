#pragma once
#include "j1App.h"
#include "j1Render.h"
#include "p2Defs.h"
#include "SDL/include/SDL.h"
#include <list>

template<typename T>
class Quadtree;

enum direction_quad
{
	NORTHWEST = 0,
	NORTHEAST,
	SOUTHEAST,
	SOUTHWEST
};


template<class T>
class QuadNode : public Quadtree<T>
{
public:
	
	QuadNode <T>(SDL_Rect boundary, uint capacity, uint level_depth);
	~QuadNode();

	void Split();
	void Draw();

private:

	SDL_Rect boundary;
	uint BucketSize;
	uint depth;
	bool divided;

	QuadNode* nodes[4];
	QuadNode* callback = nullptr;

	std::list<T> elements;

};



template<class T>
 QuadNode<T>::QuadNode(SDL_Rect boundary, uint capacity, uint level_depth)
{
	nodes[NORTHWEST] = nodes[NORTHEAST] = nodes[SOUTHWEST] = nodes[SOUTHEAST] = nullptr;
	this->boundary = boundary;
	this->BucketSize = capacity;
	this->depth = level_depth;
	this->divided = false;

}

template<class T>
 QuadNode<T>::~QuadNode()
{
}

template<class T>
 void QuadNode<T>::Split()
{

	nodes[NORTHWEST] = new QuadNode<T>({ boundary.x,boundary.y, boundary.w / 2, boundary.h / 2 }, this->BucketSize, this->depth + 1);

	nodes[NORTHEAST] = new QuadNode<T>({ boundary.x + boundary.w / 2,boundary.y,boundary.w / 2, boundary.h / 2 }, this->BucketSize, this->depth + 1);

	nodes[SOUTHWEST] = new QuadNode<T>({ boundary.x,boundary.y + boundary.h / 2 , boundary.w / 2, boundary.h / 2 }, this->BucketSize, this->depth + 1);

	nodes[SOUTHEAST] = new QuadNode<T>({ boundary.x + boundary.w / 2 ,boundary.y + boundary.h / 2, boundary.w / 2,  boundary.h / 2 }, this->BucketSize, this->depth + 1);

	this->divided = true;


}

template<class T>
 void QuadNode<T>::Draw()
{
	App->render->DrawQuad(this->boundary, 255, 255, 255, 255, false);

	for (int i = 0; i < 4; i++)
	{
		if (this->divided)
		{
			nodes[i]->Draw();
		}
	}
}
