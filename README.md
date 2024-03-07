
#DesignPatternsAssignment
Change the object pool so it's a template now. Now I am using quicksort and binary search to locate specific enemies and projectiles base on their ID.

Created a StateStack which handles the different states of the game. Which makes it easier to jump between main menu screen, game state, pause screen and game over screen.

Created a WeaponComponent class which one of the enemy uses. Now instead of using 2 different classes to have a ranged and melee enemy, I'm using 1 class that has a weapon component which decides if it's range or melee.





# SpaceShooterAssignment

Assignment for: FutureGames Computer Technology course.

Naive implementation:

Creates enemies every other second using the factory pattern.

Projectiles are created in a similar way.

Currently, the collision is checked by going through all projectiles and checking their damage type. If they can damage the player, it checks if the projectile collides with the player. If the projectile can damage enemies, it loops through all the enemies and checks if it collides with any of them. 

Since the enemies and projectiles are created at runtime, that also drains on the CPU usage.

When set so enemies spawn every other second, the fps drops drastically in that frame.

Trying it with 5000 enemies made it unplayable. The fps seemed to go down to 0.4 when the enemies were shooting projectiles.


There are two ideas I have for improving this:

The first idea I have in mind is to pool both the enemies and the projectiles. So they are all created in compiling time instead of during runtime. This would result in a longer load time, but the game would run at better performance.

The other idea I have is using a quadtree to handle the collision. This way, I don't have to compare players' projectiles with every enemy and vice versa every frame. It would only compare those in the same quadtree node.

I have some idea of how they work, but haven't used it in a project before.



I pooled the enemies and projectiles. I tried creating a template, but couldn't get it to work due to template crashes. So I created a vector for inactive objects and one for active objects which will work as the pool.

During playtime, it is around 7 - 15 fps for me. Which is a massive improvement compared to the earlier 0.4.

However, this is true if the player doesn't shoot. Since the players projectile check collisions against every active enemy. In this case, 10000 * the amount of active player projectiles. 

Up next is trying to implement a quadtree that handles the collision. I don't have a very good understanding of quadtree, so this will be a challenge to implement.

It was similar to my understanding of the object pool, but quadtree seems to be a bit more tricky to implement than an object pool.

Successfully created a QuadTree template that I can use when checking the collision between the players' projectile and the enemies. Before the QuadTree, I checked the projectile collision against every enemy every frame. Which made the project run at unplayable fps. It was less than 0.1 against 2500 enemies at that point. Now it runs at between 5 and 25 fps while shooting projectiles.



Next up is adding a separation behavior for the enemies. Currently, they stack up in the same position after a while when chasing the player. This results in all enemies being in the same QuadTreeNode, which drops the fps a ton.

Created a class that takes care of the movement behaviour. The idea is to add more behaviours to use further on since I plan to work more on this project as a testing ground for AI stuff. Currently I'm only using separation behaviour and it seams that the fps doesn't drop as much since they the enemies doesn't stack up in the same quadTreeNode.

Now I want to create a sorting algorithm for the projectiles and enemies. When I remove an enemy, I am looping through every enemy comparing their id to the enemies id the QuadTree returns. The same goes for projectiles. Either a sorting algorithm to make it easier to find the object by index in a vector or a better way to locate the specific object.


Managed to use a binary search in order to locate a specific object in the vector for both projectiles and enemies. Now it uses a binary search for the specific ID of the object that will get removed.
Tried to create a way to insert projectiles and enemies in order depending on their ID. Got quicksort to work for it, but couldn't find a good way to implement it. Currently I'm simply going through the active objects and checks if the index id is larger than the objects id, if it is then I insert the object at that location. I wanted to do it in a better way, however couldn't figure out how.

Also changed pointers to smart pointers where the project let me do it. DIdn't work in templates and with the SDL parts like sprites and text.

That's all I can manage to do for this assignment. There are things that I want to and will improve. Even if it doesn't help with the performance, I want to create a template for binary search, quicksort and object pool so I can reuse it if needed.

I also want to create a better way to handle the enemies. A faster way to locate where the enemy should be inserted in the vector and to get a specific type of enemy in the same vector. So I can have more varieties of enemy types and let the vector with the parent class handle all enemies.
Same goes for projectiles.




