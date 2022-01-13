![Title](/Title.jpg)

"The Battle Spire" is a turn-based dungeon crawler game, that incorporates a card structure for combat. 

Levels are generated as you explore each room, ending with bosses and treasures. Equipment decides your combat abilities, this takes in the form of a deck of cards where a hand of 3 will be drawn at the start of a turn, 1 movement, 1 combat, 1 random. Combat is deadly and requires tactical thinking of positioning and direction.

Each level is structured of square rooms, that consist of hexagon tiles. Rooms have an exit on each cardinal direction, hiding more rooms to explore. Once a door is opened there is no going back, so choose wisely.



## Features

<br>

<a>
<img alt="Room Gen" src="/HexRoomGen.gif" width="80%">
</a>

- Each room consists of a 2D array grid of hexagons, each side leads to another room based on its selected room type. Each room is populated using a table, objects include (but not limited to) - Walls, Doors, Enemies, Items, Traps, Scenery...

- Progrssing further increase the difficulty of each room of the dungeon. Ending with a final room containing the boss.

<a>
<img alt="Control" src="/HexControls.gif" width="80%">
</a>

- The player has control over the main camera, with panning, moving and zooming based on their needs. A Double click will also move the camera to the selected location.

- With the use of UI, the player will also be able to play actions for each of their pawns

<a>
<img alt="Light" src="/HexLight.gif" width="80%">
</a>

- Lighting will play a large role in this dungeon crawler. Light is only provided in certain areas, and enemies in the dark will be harder to hit, this forces the player to utilise the pawns directions to light the way. There is also a handly light attached to the mouse position, which will dim with distance to the pawns. 

- As seen above this light creates a more dramatic and dark atmosphere. Different room types can also produce light based on the interior scenery.

<a>
<img alt="Health" src="/HexHealth.gif" width="80%">
</a>

- Each pawn has a health bar displayed above. The size is increased when the mouse pointer is closer to the pawn.

- It also shows a marker for if the pawn is defended by a shield or magical barrier, blocking the first matching hit.

<a>
<img alt="Environment" src="/HexEnviro.gif" width="80%">
</a>

- Each room is themed based on the chosen adventure path. This includes the enemies that the player will come across. Pawn placement and facing direction will be important to keep them alive and in the fight.

<!--
![Rooms](/HexRoomGen.gif) 
![Controls](/HexControls.gif)
![Light](/HexLight.gif)
![Health](/HexHealth.gif)
![Movement](/HexEnviro.gif)
-->

### TODO

- Card Combat - Based on equipment each unit will have different abilities they can act upon each turn. A card system will add a more dynamic feel to the combat.

- Story

- Save/Load


## Reference

- [Quixel](https://quixel.com/megascans) - Models and Textures

<< [Return](https://chriswestwood.github.io/)
