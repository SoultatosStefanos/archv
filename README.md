# A 3D visualizer of software architectural dependencies 


## Summary 

**Archv** (Architecture visualizer) is a tool for visualizing C++ 
object-oriented, software architectural dependencies taken from global symbol
tables, in the form of a 3D graph.


## Features

| 3D Graph rendering | Dynamic illumination
|----|----|
| ![](data/screenshots/3d_graph.png) | ![](data/screenshots/dynamic_illumination.png) |

| Class Metadata inspection | Procedural edge-curve generation
|----|----|
| ![](data/screenshots/class_info_inspection.png) | ![](data/screenshots/procedural_edge_curves.png) |

| Layout & topology editing | Edge weight editing
|----|----|
| ![](data/screenshots/layout_n_topology_editing.png) | ![](data/screenshots/edge_weight_editing.png) |

| Vertex dynamic scaling according to class metadata | Clustering
|----|----|
| ![](data/screenshots/vertex_by_property_scaling.png) | ![](data/screenshots/clustering.png) |

| Particle system effects depending on vertex in/out degrees | Extensive configuration
|----|----|
| ![](data/screenshots/particle_effects.png) | ![](data/screenshots/extensive_configuration.png) |


## Usage

The visualizer uses the output .json file from the GraphGenerator project 
(see forked/updated repo) https://github.com/SoultatosStefanos/Code-Smell-Detector
as command line input.

E.g. `./archv_app foo/bar/buzz/graph.json`

## Third-Party Dependencies

* GoogleTest  
https://github.com/google/googletest
* Boost Graph Library (BGL)  
https://www.boost.org/doc/libs/1_80_0/libs/graph/doc/index.html
* Boost Signals2  
https://www.boost.org/doc/libs/1_61_0/doc/html/signals2.html
* Boost Log v2  
https://www.boost.org/doc/libs/1_63_0/libs/log/doc/html/index.html
* Boost Exception  
https://theboostcpplibraries.com/boost.exception
* Jsoncpp  
https://github.com/open-source-parsers/jsoncpp
* Infomap  
https://github.com/mapequation/infomap
*  IconFontCppHeaders  
https://github.com/juliettef/IconFontCppHeaders
* SDL  
https://github.com/libsdl-org/SDL
* OpenGL
* Freetype  
https://github.com/freetype/freetype
* Stb  
https://github.com/nothings/stb
* Dear ImGui  
https://github.com/ocornut/imgui
* Ogre 3D v13  
https://github.com/OGRECave/ogre
* Ogre 3D Bites Component
* Ogre 3D Overlay Component
* Ogre 3D Procedural


## Set-Up / Build

Archv uses CMake as its build system on all supported platforms (tested on 
ubuntu debian Pop!_OS version 22.04 LTS). This guide will explain to you how to
use CMake to build Archv from source, under this OS.

### Getting dependencies

* GoogleTest  
(This dependency can be skipped if you do not wish to build the Archv tests.)  
Download the v1.12.1 release from: https://github.com/google/googletest.
You should now create a build directory for GTest somewhere outside GTest's
sources.  
Then:  
`cd build && cmake ..`  
`make`  
`sudo make install`  
Archv should now be able to locate a CMake configuration file for GTest.  If 
not, the path to the directory containing the CMake configuration file must be 
given to CMake with the CMake variable: GTest_DIR (see [Cmake](#cmake)).

* Boost  
`sudo apt-get install libboost-all-dev`  
This will install all of the required Boost modules (Graph, Signals2, Log, 
Exception).  If a compile/linking error occurs when building Archv from source,
make sure that the CMake variables: Boost_INCLUDE_DIR, Boost_LOG_LIBRARY_RELEASE
, Boost_LOG_SETUP_LIBRARY_RELEASE and Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE are
configured appropriately (see [Cmake](#cmake)).

* Jsoncpp  
`sudo apt-get install libjsoncpp-dev`  
This will install the compatible JsonCpp library on your system. Archv should 
now be able to link against the libjsoncpp target.  

* Infomap  
This dependency is being used as a git submodule 
(see https://git-scm.com/book/en/v2/Git-Tools-Submodules) and will be fetched 
and built from source when building the Archv application. (see [Set-Up](#set-up)).

* IconFontCppHeaders  
This dependency is being used as a git submodule 
(see https://git-scm.com/book/en/v2/Git-Tools-Submodules) and will be fetched 
and built from source when building the Archv application. (see [Set-Up](#set-up)).

* SDL  
`sudo apt-get install libsdl2-dev`  
This is also an Ogre3D dependency. 
(see https://ogrecave.github.io/ogre/api/1.12/building-ogre.html).  
This will install the compatible SDL library on your system.  Archv should 
now be able to link against the libSDL2 target.  

* OpenGL  
`sudo apt-get install libgles2-mesa-dev`  
This is an Ogre3D dependency. 
(see https://ogrecave.github.io/ogre/api/1.12/building-ogre.html).  
Archv should now be able to locate a CMake configuration file for OpenGL. 

* Freetype  
See: https://github.com/freetype/freetype/blob/master/docs/INSTALL.UNIX  
Clone the repo from: https://github.com/freetype/freetype.  
You should now create a build directory for Freetype somewhere outside
Freetype's sources.  
Then:  
`cd build && cmake ..`  
`make`  
`sudo make install`  
Archv should now be able to locate a CMake configuration file for Freetype. If 
not, the path to the directory containing the CMake configuration file must be 
given to CMake with the CMake variable: FreeType_DIR (see [Cmake](#cmake)).

* Stb  
`sudo apt install libstb-dev`  
This will install the compatible stb library on your system. Archv should 
now be able to link against the libstb target.  

* Dear ImGui  
Just clone the repo from: https://github.com/ocornut/imgui somewhere. The build 
process of the Ogre 3D Overlay component will take care of the rest.

* Ogre 3D  
See: https://ogrecave.github.io/ogre/api/1.12/building-ogre.html.  
Download a v13 release from: https://github.com/OGRECave/ogre.  
You should now create a build directory for Ogre somewhere outside
Ogre's sources.  
Then:  
`cd build`  
Archv makes use of the Bites & Overlay components, so be sure to build them as
well. Thus the CMake variables: OGRE_BUILD_COMPONENT_BITES, 
OGRE_BUILD_COMPONENT_OVERLAY, OGRE_BUILD_COMPONENT_OVERLAY_IMGUI, 
OGRE_BUILD_DEPENDENCIES must be set to: TRUE. Also, make sure the CMake variable
IMGUI_DIR is set to the directory path of the imgui headers.  
The rest of Ogre's settings can be configured as you wish.  
(Tip: use the cmake-gui tool here, see: 
https://cmake.org/cmake/help/latest/manual/cmake-gui.1.html).  
`make`  
`sudo make install`  
Archv should now be able to locate a CMake configuration file for Ogre. If 
not, the path to the directory containing the CMake configuration file must be 
given to CMake with the CMake variable: OGRE_DIR (see [Cmake](#cmake)).

### CMake

### Notes

### Set-Up

### Building

## Configuration


