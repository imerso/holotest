HOW IT WAS DONE:

- Created Unreal 4.27 C++ blank project;
- Went to hologate.com and saved a color logo;
- Opened the logo in GIMP, made it b&w then saved again;
- Imported into Inkscape, vectorized then exported to svg;
- Imported the svg into Blender, extruded and exported hologate.obj;
- Deleted part of the mesh, added armature and exported to player.fbx;
- Imported both into the project;
- Started creating the little game with mostly C++.

PROBLEMS:

- Sockets from the player mesh were not reading correctly,
  so I just used a param to adjust inside BP (yes, ugly...);

- For some reason the exported logo is becoming black
  after a lighting rebuild;

