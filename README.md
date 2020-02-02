# Spatial-Partitioning-Quadtree

I am [Carlos Peña Hernando](https://www.linkedin.com/in/carlos-pe%C3%B1a-hernando-aa757aaa/), student of the [Bachelor’s Degree in Video Games by UPC at CITM](https://www.citm.upc.edu/ing/estudis/graus-videojocs/). This content is generated as a game development research for learning purposes at my free time.

## About the project

This project contains a research about **spatial partitioning algorithms** and its applications in game development, used as an advance to optimize generally problem-solving algorithms when we have to check and order a sizable amount of elements. This research wraps up with a demo application focused on the scope of **Region-Point Quadtree** as approached method to test its potential optimization comparing it with force brute method when we are checking collisions at run-time.

You can find information about **Spatial Partitioning** in my [website](https://carlosupc.github.io/Spatial-Partitioning-Quadtree/).

You can find the **source code** at my [github repository](https://github.com/CarlosUPC/Spatial-Partitioning-Quadtree).


## Demo Application

As a little sneak peek, the following gif shows you up the final result of the project demo using my Quadtree code to optimize considerably at collision checking.  

<p align="center">
<img src="https://media.giphy.com/media/S6rmuUcJbBgSSpvvGB/giphy.gif" width = "800">
</p>

> Collision system optimized using Quadtree method


## XML-based Quadtree configuration
Basic and simple configuration about Quadtree's properties using xml:

* X & Y values
* Width & Height values
* Depth (max depth of quadtree)
* Capacity / Bucket Size (max capacity of quadtree)

<p align="center">
<img src="https://raw.githubusercontent.com/CarlosUPC/Spatial-Partitioning-Quadtree/master/docs/xml_qt.JPG?token=AIT55NHFBZYXG7F4IF42DD26IBHTY" width = "800">
</p>

## Compile-time Quadtree code

Quadtree code works as a multiple data-type container to fit with diferent sort of elements (colliders, entities, particles, tiles, gameobjects, etc) with the same logic. Therefore, it turns out more type safety code capable of doing generalizations for many API's and avoids redundant code and its repetition if we need to work with another data-type elements. Evaluated at compile-time and can increase its performance (as an alternative to polymorphism).

* Template-based Quadtree container generic class
* Easy to integrate (Just 2 Header files)
* Readable & easy to use

Straight-forward steps:

* Create a Quadtree pointer with a given data type
 ```cpp
 Quadtree<T>* qtree = new Quadtree<T>(x,y,w,h,capacity,depth);
 ```
* Insert elements to Quadtree
 ```cpp
qtree->Insert(array<T>);
 ```
* Check its relative position every frame querying to *found* array
 ```cpp
qtree->found.clear();
qtree->query(qtree->found(T));
 ```
* Clean it up when application run out!
```cpp
qtree->CleanUp();
 ```
more information about my Quadtree code in my [website](https://carlosupc.github.io/Spatial-Partitioning-Quadtree/).


## Installing

* Download last release from [releases]() tab from the [repository](https://github.com/CarlosUPC/Installer-Research)
* Unzip Quadtree.zip
* Execute Quadtree.exe


## Controls

* **F1** - Enable / Disable debug draw colliders
* **F2** - Switch between Brute Force & Quadtree optimization methods
* **F3** - Enable / Disable debug draw Quadtree

* **1** - Insert one static entity
* **2** - Insert one dynamic entity
* **3** - Insert 1000 dynamic entities (for framerate checking purposes)



## Built With
* [Visual Studio](https://visualstudio.microsoft.com/) - IDE
* [SDL](https://www.libsdl.org/license.php) - Development library
* [STL](https://www.geeksforgeeks.org/the-c-standard-template-library-stl/) - Development library
* [PugiXML](https://pugixml.org/) - XML processing library
* [Brofiler](http://www.brofiler.com/) - Profiler
* [Tiled](https://www.mapeditor.org/) - Creating maps


## Author
**Carlos Peña Hernando** - GitHub account: [CarlosUPC](https://github.com/CarlosUPC) Contact: [tuputamadre@gmail.com]()


## License
This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/CarlosUPC/Spatial-Partitioning-Quadtree/blob/master/LICENSE) file for details

## Previous learning tests coding a Quadtree

[Quadtree Performance Test](https://carlosupc.github.io/Quadtree-Performance-Test/).

[Collision System Performance with Quadtree Test](https://carlosupc.github.io/Quadtree-Collision-Performance-Test/). 

## Acknowledgements and Webgraphy

[Wikipedia](https://es.wikipedia.org/wiki/Quadtree).

[genbeta.com](https://www.genbeta.com/desarrollo/teoria-de-colisiones-2d-quadtree). 

[the coding train](https://www.youtube.com/watch?v=OJxEcs0w_kE).

[Space Partitioning](http://gameprogrammingpatterns.com/spatial-partition.html)

[Quadtree implementations 1](https://github.com/Lectem/YAQ)

[Quadtree implementations 2](http://codereview.stackexchange.com/questions/143955/quadtree-c-implementation)

[Octree](http://www.gamasutra.com/view/feature/131625/octree_partitioning_techniques.php)

[Quadtree and Octree](http://www.i-programmer.info/programming/theory/1679-quadtrees-and-octrees.html)

[Quick Tip: Use Quadtrees to Detect Likely Collisions in 2D Space](https://gamedevelopment.tutsplus.com/tutorials/quick-tip-use-quadtrees-to-detect-likely-collisions-in-2d-space--gamedev-374)

[K-D Tree Implementation 1](https://rosettacode.org/wiki/K-d_tree)

[K-D Tree Implementation 2](https://github.com/gvd/kdtree)

[K-D Tree Implementation 3](https://github.com/orangejulius/kdtree)

[K-D Tree VS Quadtree](http://stackoverflow.com/questions/13487953/difference-between-quadtree-and-kd-tree)

[K-D Tree VS Quadtree](http://gamedev.stackexchange.com/questions/87138/fully-dynamic-kd-tree-vs-quadtree )

[Quadtrees and Hilbert Curves](http://blog.notdot.net/2009/11/Damn-Cool-Algorithms-Spatial-indexing-with-Quadtreesand-Hilbert-Curves)

[JavaScript QuadTree Implementation](http://www.mikechambers.com/blog/2011/03/21/javascript-quadtree-implementation/)

[Pyramid Panic - Feature - QuadTree Optimizations](https://steemit.com/utopian-io/@carsonroscoe/pyramid-panic-feature-quadtree-optimizations)

[Examining Quadtrees, k-d Trees, and Tile Arrays](https://pdfs.semanticscholar.org/422f/63b62aaa6c8209b0dcbe8a53e360ad90514d.pdf)

[Teoría de colisiones 2D: QuadTree](https://www.genbeta.com/desarrollo/teoria-de-colisiones-2d-quadtree)

[AABB Trees for Collision Detection](https://goharsha.com/blog/aabb-trees-for-collision-detection/)

[Quadtrees – Hierarchical Grids](http://www.cs.tau.ac.il/~haimk/seminar12b/Quadtrees.pdf)

[jimkang](https://jimkang.com/quadtreevis/).

[GeeksforGeeks](https://www.geeksforgeeks.org/quad-tree/).



