HOW IT WAS DONE:

- Created Unreal 4.27 C++ blank project;
- Went to hologate.com and saved a color logo;
- Opened the logo in GIMP, made it b&w then saved again;
- Imported into Inkscape, vectorized then exported to svg;
- Imported the svg into Blender, extruded and exported hologate.obj;
- Deleted part of the mesh, added armature and exported to player.fbx;
- Imported both into the project;
- Started creating the little game with mostly C++.


HOW TO PLAY:

At this point the game was tested only inside the Unreal Editor. Run it as listening server, then check that each player can look around with mouse, fire using left-click and move using WASD.
Energy and score are tracked for each player. When a player is dead, that player can respawn pressing [ENTER]. When respawning, score is kept for the session and energy is reset to full.


HOW I FEEL ABOUT UNREAL:

I wrote a few engines in the past and have an idea about how engines
like Unreal are implemented.

I am not a Unreal specialist (yet), though. I just find it natural
to learn about 3D engines while I go. Threejs, Babylonjs, Unity, Unreal,
they look straightforward to learn.

I definitely checked the docs multiple times while building this tiny game.

Unreal is a great engine overall. If I get a job opportunity, I'll dive into it
and study its source-code to get more details about how everything was
implemented, to master it.
