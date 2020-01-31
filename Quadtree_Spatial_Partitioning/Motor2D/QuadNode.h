#pragma once
#include "j1App.h"
#include "j1Render.h"
#include "p2Defs.h"
#include "SDL/include/SDL.h"
#include <list>
#include "p2Log.h"

template<typename T>
class Quadtree;
struct Collider;
class j1Entity;
struct TileSet;

enum direction_quad
{
	NORTHWEST = 0,
	NORTHEAST,
	SOUTHEAST,
	SOUTHWEST
};


template<class T>
class QuadNode 
{
public:
	
	QuadNode <T>(SDL_Rect boundary, uint capacity, uint level_depth, Quadtree<T>* qtree);
	~QuadNode();

	void Split();
	bool Insert(T* data);
	void Query(std::list<T*>& found, T* data);
	void Draw();

	void CleanUp();
	//iPoint GetDataCoords(T data);
	bool Contains(const T& data);

private:

	SDL_Rect boundary;
	uint BucketSize;
	uint depth;
	bool divided;
	bool leaf;

	QuadNode* nodes[4];
	Quadtree<T>* callback = nullptr;

	std::list<T*> elements;

};



template<class T>
 QuadNode<T>::QuadNode(SDL_Rect boundary, uint capacity, uint level_depth, Quadtree<T>* qtree)
{
	nodes[NORTHWEST] = nodes[NORTHEAST] = nodes[SOUTHWEST] = nodes[SOUTHEAST] = nullptr;
	this->boundary = boundary;
	this->BucketSize = capacity;
	this->depth = level_depth;
	this->divided = false;
	this->leaf = true;
	this->callback = qtree;
}

template<class T>
 QuadNode<T>::~QuadNode()
{
}

template<class T>
 void QuadNode<T>::Split()
{

	nodes[NORTHWEST] = new QuadNode<T>({ boundary.x,boundary.y, boundary.w / 2, boundary.h / 2 }, this->BucketSize, this->depth + 1, this->callback);

	nodes[NORTHEAST] = new QuadNode<T>({ boundary.x + boundary.w / 2,boundary.y,boundary.w / 2, boundary.h / 2 }, this->BucketSize, this->depth + 1, this->callback);

	nodes[SOUTHWEST] = new QuadNode<T>({ boundary.x,boundary.y + boundary.h / 2 , boundary.w / 2, boundary.h / 2 }, this->BucketSize, this->depth + 1, this->callback);

	nodes[SOUTHEAST] = new QuadNode<T>({ boundary.x + boundary.w / 2 ,boundary.y + boundary.h / 2, boundary.w / 2,  boundary.h / 2 }, this->BucketSize, this->depth + 1, this->callback);

	this->divided = true;


}

 template<typename T>
 inline bool QuadNode<T>::Insert(T* data)
 {

	 if (!this->Contains(*data))
		 return false;


	 if (this->leaf) // LEAF NODE
	 {
		 if (this->elements.size() < this->callback->GetMaxBucketSize())
		 {
			 this->elements.push_back(data);
			 return true;
		 }

		 else if (this->depth < this->callback->GetMaxDepth())
		 {
			 this->leaf = false;

			 if (!this->divided)
				 this->Split();


			 for (int i = 0; i < 4; ++i)
				 this->nodes[i]->Insert(data);
					

			typename std::list<T>::iterator it;

			for (std::list<T*>::iterator it = elements.begin(); it != elements.end(); it++)
			{

				for (int j = 0; j < 4; ++j)
					this->nodes[j]->Insert(*it);	

			}

			this->elements.clear();
		 }
	 }
	 else // STEM NODE
	 {

		 for (int i = 0; i < 4; ++i)
			 this->nodes[i]->Insert(data);
				
	 }




 }

 template<class T>
 inline void QuadNode<T>::Query(std::list<T*>& found, T* data)
 {

	 if (!this->Contains(*data))
		 return;

	 if (this->leaf) // LEAF NODE
	 {
		 typename std::list<T>::iterator it;

		 for (std::list<T*>::iterator it = elements.begin(); it != elements.end(); it++)
		 {
			 if(data != *it)
				found.push_back(*it);
		 }

	 }
	 else // STEM NODE
	 {

		 for (int i = 0; i < 4; ++i)
			 this->nodes[i]->Query(found, data);

	 }


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

 template<class T>
 inline void QuadNode<T>::CleanUp()
 {

	 if (this->divided)
	 {
		 for (int i = 0; i < 4; ++i)
		 {

			 if (nodes[i] != nullptr)
			 {
				 nodes[i]->CleanUp();
				 RELEASE(nodes[i]);
			 }
		 }
		 this->divided = false;
	 }
	 else
	 {
		 typename std::list<T>::iterator it;

		 for (std::list<T*>::iterator it = elements.end(); it != elements.begin(); --it)
		 {
			 elements.remove(*it);
			 
		 }
		 elements.clear();

	 }

	 this->leaf = true;

 }

 template<class T>
 inline bool QuadNode<T>::Contains(const T& data)
 {
	 /*Middle point of the tile
	  iPoint pos(data.rect.x + data.rect.w / 2, data.rect.y + data.rect.h / 2);

	  if (pos.x < this->boundary.x ||
	 	 pos.x > this->boundary.x + this->boundary.w ||
	 	 pos.y < this->boundary.y ||
	 	 pos.y > this->boundary.y + this->boundary.h)
	 	 return false;
	  
	  return true;*/

	  return (data.CheckCollision(this->boundary));

	
 }




 //----------------------TESTING----------------------//

 //
 //template<class T>
 //inline iPoint QuadNode<T>::GetDataCoords(T data)
 //{
	// bool value = false;
	// 
	// if(std::is_same<T, Collider>::value)
	//	 return iPoint(data.rect.x + data.rect.w / 2, data.rect.y + data.rect.h / 2);

	// else {
	//	 return(iPoint(0, 0));
	//	 LOG("No element detected");
	// }

	//
 //}

 
 
