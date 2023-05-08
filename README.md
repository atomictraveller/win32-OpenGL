a minimal use of modern OpenGL in win32 using no external libraries.

only the standard gl header is added to the default win32 app framework..
* #include <GL/gl.h>
* #pragma comment(lib, "opengl32.lib")

* rendering context creation in modern OpenGL is identical to "classic"
* the GL/gl.h included in win32 is OpenGL 1.1 (ms DirectX is a competitor)
* modern gl APIs are loaded directly from the .dll with LoadLibrary

this eg. allows me to use OpenGL at version 4.6 on my current platform (a brief comment in ogl.h explains reading the version number).

the initial array of resources for using OpenGL on windows is overwhelming. my assessment is that glm, which is distributed as a header and is used seemingly universally, is the most sensible path to using modern OpenGL APIs. however, it is possible to use LoadLibrary to extract the modern APIs directly from the OpenGL .dll if you don't want to have any external dependencies whatsoever.

this choice may seem insensate for those acclimatised to resources, there may be still a few of us out here who for some reason or other maintain eclectic habits.


so here you have the default msvc 2019 win32 c/c++ "windows desktop app" framework. there are two header files:
#include "ogl.h"	//	my opengl header
#include "win32 OpenGL.h" //  the header for this project

ogl.h includes a list of constants and prototypes for the created API. the short list of OpenGL APIs was inherited from this win32 OpenGL source:
https://github.com/vbsw/opengl-win32-example/blob/3.0/Main.cpp

i found i needed to add a few more APIs than these to use basic functions. in order to add absent APIs, reference a document called glcorearb.h and find the prototype in there, then you'll see two places to add stuff in ogl.h, or however you organise yourself, and two equally obvious things to add in InitInstance in the .cpp file, takes less than a minute.

so if you can handle populating the APIs yourself, you can do things in your own minimal way, without having to use 4x4 matrices for everything ;)

mozilla's webgl tutorial is comparatively concise and geared towards developers who understand more concepts previously, if you're like me and pick through rather than follow what's out there.


beyond contect creation, this program displays a height field with a 3d 'perlin' texture and directional ('diffuse') lighting. you can 'fly over' the height field using the arrow keys and a/z to look up/down.

ftr both perlin implementations are my code for efficiency, modern references seem to indicate original perlin used 2d vector random values, which i don't remember. shaping the interpolator with an 's' function takes some of the linearity out of the 3d perlin texture, but i noticed my simple dot product diffuse lighting can elicit linear interpolation along ridges/transitions.. i'm saying the rendering methods used here are economic and would be replaced by more intensive procedures in quality software haha.

animation is performed by a simple function call in WM_PAINT to draw the frame, and WM_PAINT is called by a timer set in WM_CREATE to 50 fps (generally, games use a looping code maybe from WM_CREATE to call WM_PAINT so the achieved framerate can be measured, not the windows timer function).

all of the OpenGL setup is called from InitInstance, which runs before WM_CREATE. i don't know if that's the best or standard way to do it, it worked that way for me. hopefully this source is simple and clear enough so you can add OpenGL to whatever you have.
