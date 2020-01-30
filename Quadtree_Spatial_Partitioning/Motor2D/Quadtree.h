#pragma once
#include "p2Defs.h"
#include "SDL/include/SDL.h"
#include <list>

#include "QuadNode.h"

template<typename T> 
class Quadtree
{
public:

	Quadtree();
	Quadtree <T>(SDL_Rect boundary, uint bucketSize, uint depth);
	virtual ~Quadtree();

	 void Split();
	 void Insert();
	 void Query();
	 void Draw();


private:
	

	QuadNode<T>* root;
	uint max_depth;
	uint max_bucketSize;

};



template<typename T>
inline Quadtree<T>::Quadtree()
{
}

template<typename T>
inline Quadtree<T>::Quadtree(SDL_Rect boundary, uint bucketSize, uint depth)
{
	root = new QuadNode<T>(boundary,bucketSize,depth);

	this->max_bucketSize = bucketSize;
	this->max_depth = depth;

}

template<typename T>
Quadtree<T>::~Quadtree()
{
}

template<typename T>
inline void Quadtree<T>::Split()
{
	root->Split();
}

template<typename T>
inline void Quadtree<T>::Insert()
{
}

template<typename T>
inline void Quadtree<T>::Query()
{
}

template<typename T>
inline void Quadtree<T>::Draw()
{
	root->Draw();
}
