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
* Ogre 3D v13  
https://github.com/OGRECave/ogre
* Ogre 3D Bites Component
* Ogre 3D Overlay Component
* Ogre 3D Procedural Component


## Set-Up / Build

Lorem ipsum lorem ipsum

### CMake


## Configuration


