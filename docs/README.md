
<p align="center">
<h1>Spatial-Partitioning-Quadtree</h1> 
</p>

## Search Conception

First of all, in order to talk about the topic we are here, It is very important to start with a brief introduction about the concept of "search" in the computational world. The concept of search comes from search engine/ tool code which, as the name implies, searches for certain types of elements in an area of the space. 

This search stick to a rule or rules, such as "search for all elements found at a radius X of this point" , "Search for the element closest to this point", ... Starting from these rules of searching, there are different ways to sort the space to perform the search.



## Application in Videogame

Search engine is a fundamental tool which at the time was used and nowadays is still used to simulate so many features that have archieved very positive results in videogames.

For example, in a game like Age of Empires, if you tell a villager to "cut down trees to get wood," the villager from the position where he is will look for the nearest tree and when he finds it with the search engine, a pathfinding algorithm will be performed to go to the tree. Once it is finished, if the order has not been revoked, you can search for a nearby tree on a radius with the search engine and retrace the pathfinding to this one, if it does not find any, you can tell it to look for the nearest building and shelter, again with the search engine, but instead of looking for tree entities, look for building entities. 

Another example may be tower defense games. There are enemies that are advancing in a route, and the towers of defense look for with the engine, enemies that are in range of attack, if so, they attack. Once you have eliminated the target, look for another enemy in range, if there is one, attack, if not, keep doing checks on each frame or every X frames in the search for another enemy.


## Issue's Point

Applied to video games, if we want to find a type of entity of the world in a limited area/range, such as to check collisions, render tile maps, render 3D objects, etc. The first solutions that comes to our mind will usually be an exhaustive search, also called **brute force search.**
 
As its name says, this algorithms are generally **problem-solving**, but they scale with the size of candidates they have to check., working OK with a low amount of them but making our game **unplayable if the amount is high**.
 
For example, in order to check the collisions of the particles of the system in the image, if we used a brute force algorithm, we would check each particle colliding with the others, no matter their position.

If we are working with a low amount of particles like in this image, we would iterate 72 (8x8) times each frame. But let’s say we are working with 100 particles, we would need 10.000 (100x100) iterations each frame.

<p align="center">
<img src="images/low_particles.png"><br>
</p>

In case that we have 1000 collision, we are talking about 1000 * 1000 = 1.000.000 checks and so on. Thus, we can see that the check collisions have a cost of elements^2.

So, now imagine how many iterations we would need in a scene like this one:

<p align="center">
<img src="images/lots_particles.png" ><br>
</p>

A really big headache, don't you think?

That’s where **spatial partitioning algorithms** appears to save this issue :)

# Space partitioning

In order to avoid iterations that are not needed (colliders too far from each other, tiles / polygons out of the screen), we can divide the space into different subsets.
 
That process is called space partition. There are a lot of ways to divide the space, and depending on our needs we will choose one or another.

* **BSP (Binary Search Partition)**: First used in *Doom*, to optimize the process and ordering by distance. It cuts the space in hyperplanes and keep branching top/bottom of the plane.

<p align="center">
<img src="https://upload.wikimedia.org/wikipedia/commons/8/81/Binary_space_partition.png" ><br>
</p>

* **QUADTREE**: divides the space into 4 subsets, each of them might be divided into 4 subsets, etc.
<p align="center">
<img src="images/quadtree.png" ><br>
</p>

* **OCTREE**: similar to the quadtrees, but used in 3D instead of 2D. Each node will have 8 children instead of 4.
<p align="center">
<img src="images/octree.png" ><br>

* **K-D TREE**: the space is divided into 2 subspaces, which might not be equal. The “partition lines” are always perpendicular to the coordinates axis.

<p align="center">
<img src="images/kdtree.png" ><br>
</p>

* **AABB TREE**: Creates subspaces for each group of elements we need to check. Mostly used in dynamic entities like collisions.
 <p align="center">
 <img src="images/aabbtree.png" ><br>
 </p>
 
As you cans see in the images above, these data structures translate the space into nodes, which have more subnodes. This structure can be represented in tree diagrams, that's why they are called trees.
 
This are just a **few** ways to part the space explained vrey superficially. There are **a lot more** of them, and honestly, we could spend an entire semester talking about different space partition algorithms, but in this article I’m going to focus on Quadtrees.
 


# Quadtrees
As a approached method to work with, Quadtree is the space partitioning algorithm i choose to develop an optimized collision system you can find in the demo application i made in my [repository]().

As I said before, quadtrees are a data structure that divide the space into 4 sub regions. Each node will have four children, which will have four children each, etc. 

Even though I'm going to use them for a "videogame" aproach, they can be used in a lot of fields, such as image compression.

<p align="center">
 <img src="images/quadtree-image-compression.gif" ><br>

  </p>
  > Compression of an image using quadtrees
  
There are different types of quadtrees, but I will focus on the **Region-Point Quadtrees**, which are the most common and I think will be the most useful in a 2D game.

This quadtree divide the total space into four equal regions, which will be divided into four equal regions and so on until we reach the nodes that are at the bottom of the tree. 

<img src="images/quadtree-1.png" ><br>

This type of structure works mostly with recursive fucntions, which the user will call to the main quadtree but then will be called to all its subnodes. 

OK, nice, now we know how quadtrees divide the space, but how can they be usefull in our game?

Let's go back to **particle** systems. Take a look at this images.

<img src="images/particles-gif.gif" ><br>

In the first frame we see how the space is not divided, therefore, we check collisions between all the particles (brute force). And in a system for only 20 particles, we need 400 iterations for each frame. It's important to know that the checks we make increase exponentially as we add more particles: with 10 particles we need 100 iterations, with 20 particles, 400 iterations, with 30 particles, 900 iterations, and so on.

In the second frame the space is divided into four subspaces, and each particle only checks its collision with the other particles in its own subspace. As you can see, it reduces the number of iterations a lot.

And in the third frame, we divide all the previous subspaces that had more than 3 particles. As you can see, there are some subspaces which only have one particle, so we won't even need to check their collision. 

Only by dividing the space twice, improved the performance of our system in a 1279%, by going from 400 iterations each frame to 29. Amazing, right?


The cases where these search tools comes to are the following ones:

* **Camera Culling**: Instead of showing the tiles from the map that belong to the camera by doing an exhaustive search (which is highly uneficient) we use the QuadTree structure. This lets us achieve a cost of n * log(base 4) n (being n the size of the map). We make this happen by recursively checking one fourth of the map with our camera boundaries. If there is any overlaps we divide this quarter in 4 pieces again. We continue doing this until we reach the maximum number of divisions set by the programmer. By doing this we end up having the tiles that must be shown on camera and a few extra ones to have a margin and not have any tile being cut. As a fact, the QuadTree that we use is a static type of data structure. This is because we save the map tiles inside of it once we load the map. Then we only have to reffer to the QuadTree to acces the desired tile/s.

* **Collision Checking**: This case is more difficult than camera culling because the set of tiles is static but entities and particles are dynamic. In this case we need to create a dynamic QuadTree that always changes along with entities. This it's slower than the quadtree mentioned before but it's faster than the exhaustive method.














### Quadtree structure ###

So, now let me explain superficially which I think are the core elements of a quadtree, and then explain more deeply how I used them in my project.

<img src="images/quadtree-core.png" ><br>

We will find this elements in any quadtree, no matter what we use them for, some functions might change, but the overall purpose will be the same. Obviously, we will need more methods and variables depending on what we use our quadtrees for, but this would be a nice parent class. 

The fact that some methods are virtual in this header does not mean that they have to be, this is how I made it, but as I said at the beggining, you can find your own way of coding them.

Basically, all we need is a rectangle that tells us the area the node is occupying (**section**).

The **level** and **max_levels** variables tell us in which level the node is in the tree and how much levels can the tree have respectively, so when a node's level is equal to max_levels, it won't be divided under any circumstance.

<img src="images/levels.png" ><br>

And obviously, an array with all the **subnodes**.

When it comes to the methods, the most important one is the **Split()**, which will divide the node into 4 subnodes.

<img src="images/split-function.png" ><br>

The **CheckTouch()** tells us if a rectangle fits or not to the current node (I know the name is pretty bad, I apologize).

<img src="images/checktouch.png" ><br>

Then we have the **DrawQuadtree()** which, guess what, is going to draw the quadtrees.

<img src="images/draw-qt.png" ><br>

And obviously the constructor and the cleanup.

### Camera culling with quadtrees ###

So, even thought all my examples were about particles and collisions, I used quadtrees for the camera culling of my game. I didn't mention it because i'll talk about it a little bit more deeply here.

When we are playing a video game, we don't need to render all the map, in fact, we must not, because it's a waste of time. As I said at the beggining, if the map isn't really big, it's not a big deal, but most of the cases it won't be like that, and especially in tiled maps we need to optimize the render process.

Let me put into situation. Let's say I'm developing an RTS with a map of 256x256 tiles. That's a total amount of 65.536 tiles, from which only 500 will appear in the screen, more or less. So, how do we print only the tiles that appear in the screen?

<img src="images/camera-culling.png" ><br>

The first solution that might come to our minds is to put a condition before drawing each tile to make sure they appear in the screen, right? **WRONG** We don't want to do that, that's brute force, we're still checking all the tiles. 

What we want to do is to only go across those tiles that appear int the screen, instead of going across all of them and only printing those that are in the screen. How do we do that?

I will create a quadtree in which i will store all the tiles distributed among all the nodes. Unlike particles, tiles are static, so I will only need to place them once. I will store the tiles in the nodes that are at the **bottom of the tree**, so I don't need to create more nodes and alocate data during the update phase.

Before, I showed you a screenshot of what the header of a quadtre would look like. To do this i will create a children class from that one. The header looks like this:

<img src="images/tile-quadtree-header.png" ><br>

First of all, I've created a struct called **TileData** which will store the position and ID of a tile. There will be one of them for each tile in the map. Each node will have a dinamic array in order to store them, but as I said before, I will only store the tiles in the nodes located at the bottom of the tree, so the arrays of the nodes that are not at the bottom will be **NULL**.

The **size** of the node will tell us how many tiles it can store, and **tiles_contained** how many it actually stores.

I've also added two functions:

**CheckVisibility()** will return true if the node is in the screen, and false if it does not.

**DrawMap()** if the node has children, what means it is not at the bottom of the tree, it will check if they are on the screen. each node in the screen will call it, if they are not they won't. When a node located at the bottom of the tree calls it, it will draw the tiles stored in his array.

#### Try to do it by yourself! ####


### Quadtree Performance Test
 [Click Here!](https://carlosupc.github.io/Quadtree-Performance-Test/)
### Quadtree Dyn-Collision Performance Test
 [Click Here!](https://carlosupc.github.io/Quadtree-Collision-Performance-Test/)
 
### BRUTE FORCE PERFORMANCE
 
<p align="center">
<img src="https://media.giphy.com/media/h4Bu2Q9UzbYw8WZhbk/giphy.gif">
</p>
 
 
Debug Info:

<p align="center">
<img src="https://raw.githubusercontent.com/CarlosUPC/Spatial-Partitioning-Quadtree/master/docs/more_brute_force.JPG?token=AIT55NEBPR5N4DDJNQKNZL26H3KZO">
</p>

### QUADTREE PERFORMANCE

<p align="center">
<img src="https://media.giphy.com/media/cPZKzQMqiXqaz35P9S/giphy.gif">
</p>

Debug Info:

<p align="center">
<img src="https://raw.githubusercontent.com/CarlosUPC/Spatial-Partitioning-Quadtree/master/docs/qtreeeee.JPG?token=AIT55NEJFTX7QGBRUJNFCNK6H3KW4">
</p>
