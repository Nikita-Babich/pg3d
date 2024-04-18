# Description pg3d
 3d triangle-projection grahics made in C/C++ with WinAPI
 
## Structure
There is World, which has a vector of Object, each of which is a set of triangles with additional info (like colors).

There is Camera which tracks position and orientation of the viewer. Holds parameters like type of projection.

"Render" takes Camera and World and loops through objects and their triangles. If normal does not look at the viewer - skip.

Reader takes vtk, builds object and adds it to the world.
 
## Controls
(My keyboard is broken, so mapping might look weird)


## Todo
- Camera
