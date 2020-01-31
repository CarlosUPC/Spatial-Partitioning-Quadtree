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
	 void Query(std::list<T*>& found, T* data);
	 void Draw();
	 void CleanUp();

	 uint GetMaxDepth() const;
	 uint GetMaxBucketSize() const;

	

private:
	

	QuadNode<T>* root;
	uint max_depth;
	uint max_bucketSize;

public:
	std::list<T*> found;

};



template<typename T>
inline Quadtree<T>::Quadtree()
{
}

template<typename T>
inline Quadtree<T>::Quadtree(SDL_Rect boundary, uint bucketSize, uint depth)
{
	root = new QuadNode<T>(boundary,bucketSize,0,this);

	this->max_bucketSize = bucketSize;
	this->max_depth = depth;

}

template<typename T>
Quadtree<T>::~Quadtree()
{
	delete root;
	root = nullptr;
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
inline void Quadtree<T>::Query(std::list<T*>& found, T* data)
{
	root->Query(found, data);
}

template<typename T>
inline void Quadtree<T>::Draw()
{
	root->Draw();
}

template<typename T>
inline void Quadtree<T>::CleanUp()
{
	root->CleanUp();
	found.clear();
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

