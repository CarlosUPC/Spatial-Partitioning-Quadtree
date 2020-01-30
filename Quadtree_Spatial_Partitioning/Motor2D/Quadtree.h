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
	 void Insert(T* data);
	 void Query();
	 void Draw();

	 uint GetMaxDepth() const;
	 uint GetMaxBucketSize() const;

	uint max_bucketSize;
private:
	

	QuadNode<T>* root;
	uint max_depth;

};



template<typename T>
inline Quadtree<T>::Quadtree()
{
}

template<typename T>
inline Quadtree<T>::Quadtree(SDL_Rect boundary, uint bucketSize, uint depth)
{
	root = new QuadNode<T>(boundary,0,0,this);

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
inline void Quadtree<T>::Insert(T* data)
{
	root->Insert(data);
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

template<typename T>
inline uint Quadtree<T>::GetMaxDepth() const
{
	return max_depth;
}

template<typename T>
inline uint Quadtree<T>::GetMaxBucketSize() const
{
	return max_bucketSize;
}
