# A 3D visualizer of software architectural dependencies 


## Summary 

**Archv** (Architecture visualizer) is a tool for visualizing C++ 
object-oriented, software architectural dependencies taken from global symbol
tables, in the form of a 3D graph.


## Table of Contents  
[Features](#features)  
[Set-Up / Build](#set-up--build)  
[Usage](#usage)  
[Configuration](#configuration)


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
* OGRE v13  
https://github.com/OGRECave/ogre
* OGRE Bites Component
* OGRE Overlay Component
* Ogre Procedural


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
given to CMake with the CMake variable: GTest_DIR 

* Boost  
`sudo apt-get install libboost-all-dev`  
This will install all of the required Boost modules (Graph, Signals2, Log, 
Exception).  If a compile/linking error occurs when building Archv from source,
make sure that the CMake variables: Boost_INCLUDE_DIR, Boost_LOG_LIBRARY_RELEASE
, Boost_LOG_SETUP_LIBRARY_RELEASE and Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE are
configured appropriately 

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
This is also an Ogre dependency. 
(see https://ogrecave.github.io/ogre/api/1.12/building-ogre.html).  
This will install the compatible SDL library on your system.  Archv should 
now be able to link against the libSDL2 target.  

* OpenGL  
`sudo apt-get install libgles2-mesa-dev`  
This is an Ogre dependency. 
(see https://ogrecave.github.io/ogre/api/1.12/building-ogre.html).  
Archv should now be able to locate a CMake configuration file for OpenGL. 

* Freetype  
See: https://github.com/freetype/freetype/blob/master/docs/INSTALL.UNIX  
Clone the repo from: https://github.com/freetype/freetype.  
`git clone https://github.com/freetype/freetype`  
You should now create a build directory for Freetype somewhere outside
Freetype's sources.  
Then:  
`cd build && cmake ..`  
`make`  
`sudo make install`  
Archv should now be able to locate a CMake configuration file for Freetype. If 
not, the path to the directory containing the CMake configuration file must be 
given to CMake with the CMake variable: FreeType_DIR 

* Stb  
`sudo apt install libstb-dev`  
This will install the compatible stb library on your system. Archv should 
now be able to link against the libstb target.  

* Dear ImGui  
Just clone the repo from: https://github.com/ocornut/imgui somewhere.  
`git clone https://github.com/ocornut/imgui`  
The build  process of the Ogre Overlay component will take care of the rest.

* OGRE  
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
given to CMake with the CMake variable: OGRE_DIR 

* Ogre Procedural  
This is a dependency that is not packaged with Ogre from the previous step.  
Clone the repo from: https://github.com/OGRECave/ogre-procedural.  
`git clone https://github.com/OGRECave/ogre-procedural`  
You should now create a build directory for  Ogre Procedural somewhere outside
 Ogre Procedural's sources.  
Then:  
`cd build && cmake ..`  
(But make sure that the CMake variable: OGRE_DIR is set to the directory path 
containing a CMake configuration file for Ogre).  
`make`  
`sudo make install`  
Archv should now be able to locate a CMake configuration file for Ogre 
Procedural.  If  not, the path to the directory containing the CMake 
configuration file must be given to CMake with the CMake variable: 
OGRE_PROCEDURAL_DIR 


### CMake optional variables

Before building the application, some final CMake variables can optionally be
set. Namely, the: ARCHV_RENDERING_CONFIG_PATH, ARCHV_GUI_CONFIG_PATH, 
ARCHV_WEIGHTS_CONFIG_PATH, ARCHV_LAYOUT_CONFIG_PATH, ARCHV_SCALING_CONFIG_PATH,
ARCHV_CLUSTERING_CONFIG_PATH, ARCHV_RESOURCE_GROUP.  
The ARCHV_RENDERING_CONFIG_PATH, ARCHV_GUI_CONFIG_PATH, 
ARCHV_WEIGHTS_CONFIG_PATH, ARCHV_LAYOUT_CONFIG_PATH, ARCHV_SCALING_CONFIG_PATH, 
ARCHV_CLUSTERING_CONFIG_PATH CMake variables must be set in order to specify the
file path(s) to the .json configuration file(s) for each Archv subsystem.
(see [Configuration](#configuration)).  
The ARCHV_RESOURCE_GROUP CMake variable is the resource group name used by Archv
in order for Ogre to locate its assets. (see Configuration Files section at: 
https://ogrecave.github.io/ogre/api/1.12/setup.html).

> **_NOTE:_**  If any of the Archv config file paths have not been 
specified, they will be defaulted to the respective seperate file paths under: 
'data/config'. (See [Configuration](#configuration)).  

> **_NOTE:_**  If the Archv resource group name has not been manually specified,
it will be defaulted to: 'Archv'. (See [Configuration](#configuration)).  


### Notes

Archv makes use of several C++20 library features (input range adaptors), 
thus certain compilers that do not support these features will fail to build 
this project (like Clang libc++).  
(see https://en.cppreference.com/w/cpp/compiler_support).  
Archv has been tested with GCC libstdc++.


### Building

Clone the repο:  
`git clone https://github.com/SoultatosStefanos/archv`  
Init the git submodules:  
`git submodule update --init --recursive`  
You should create a build directory for Archv somewhere outside Archv's sources.  
Then:  
`cd build && cmake ..`  
`make` or  
`make archv_app` or  
`make archv_tests`  
In order to build all the targets, the application, or the tests respectively.


### Set-Up

Lastly, before attempting to run the application, the directory paths of all of
the used assets must be specified for Ogre, under the Archv resource group name.  
Add the following lines to your resources.cfg file: (probably located under: 
/usr/local/share/OGRE)  
```cfg
[<Archv resource group name>]
FileSystem=/path/to/archv/data/textures
FileSystem=/path/to/archv/data/materials
FileSystem=/path/to/archv/data/models
FileSystem=/path/to/archv/data/fonts
FileSystem=/path/to/archv/data/particles   
```
(See Configuration Files section at: 
https://ogrecave.github.io/ogre/api/1.12/setup.html).

> **_NOTE:_**  If the Archv resource group name has not been manually specified,
it will be defaulted to: 'Archv'. (see 
[CMake optional variables](#cmake-optional-variables), [Configuration](#configuration) ).  


## Usage

The visualizer uses the output .json file from the GraphGenerator project 
(see forked/updated repo) https://github.com/SoultatosStefanos/Code-Smell-Detector
as command line input.

E.g. `./archv_app foo/bar/buzz/graph.json`  

Or, in order to run the tests:  

`./tests/archv_tests`


## Configuration

Archv supports both interactive configuration, via GUI, and startup 
configuration with .json config file(s).  
This section will explain to you how to best configure the application's style, 
and/or graph visualization properties.

> **_NOTE:_** Wether each subsystem is configured seperately at its own .json 
file, or a single config file is used for the entire application will be decided
at **build** time. 
(See [CMake optional variables](#cmake-optional-variables)).


### Layout

The graph's visualization properties regarding its layout & topology in 3D space.  

Example .json configuration:

```json

"layout" :
{	
    "layouts" :
    [
        "Gursoy Atun"
    ],
    "topologies" :
    [
        "Cube",
        "Sphere"
    ],
    "layout" : "Gursoy Atun",
    "topology" : "Sphere",
    "scale" : 1300
}

```

**layouts** (`string list`)

The available layout algorithms that can be selected at runtime.  
A layout algorithm generates the position of each vertex inside the 3D space.

Possible values: <**Gursoy Atun**>

**topologies** (`string list`)

The available topologies that can be selected at runtime.  
A topology is a description of space inside which the layout is performed.  

Possible values: <**Cube** | **Sphere**>

**layout** (`string`)

The default layout algorithm used.   

Possible values: **one listed under layouts**.

**topology** (`string`)

The default topology used.  

Possible values: **one listed under topologies**.

**scale** (`double`)

The default scale used to generate the graph layout.  
This variable controls the "magnitude" of the layout, that is, how far the graph
will take up space. 

Possible values: **any positive floating point number**.


### Weights

Physical dependencies across C++ components are expressed with edges in the 
visualized dependencies graph. The "weight" value of each type of dependency can
be specified here.  

The weight values of each dependency affect the output of both clustering and 
layout algorithms.

Example .json configuration:

```json

"weights" : 
{
    "dependencies" : 
    [
        {
            "Inherit" : 1
        },
        {
            "Friend" : 1
        }
    ]
}

```

Where each **dependency** (`string`) is paired with a **weight** (`int`).  

> **_NOTE:_** Each dependency found in the graph .json input file must be 
included here.


### Scaling

Archv can be configured in order to apply dynamic scaling to each graph vertex, 
depending on the vertex underlying class metadata.  
That way, quick visual comparisons can be made between vertices.  
Here, scaling factors are defined. These factors can be taken into account, 
and/or combined, in order to scale the graph's vertices in relation to each 
other.

Example .json configuration:

```json

"scaling" :
{
    "factors" : 
    [
        {
            "Fields" : 
            {
                "enabled" : true,
                "dimensions" : [ true, true, true ],
                "baseline" : 3,
                "ratio" : { "min" : 0.5, "max" : 2.5 }
            }
        }
    ]
}

```

**factors** (`objects list`)

The available scaling factors that can be selected at runtime.  

Possible values: <**Fields** | **Methods** | **Nested**>.  
In order to scale according to the number of the vertex underlying class: fields
, methods, nested classes, respectively.  

For each scaling factor the following variables can be specified:

**enabled** (`bool`)

Wether this scaling factor / class metadata property is taken into account when
computing the final vertex scale.

**dimensions** (`bool array`)

The axes on which the scaling is applied on the vertex. For each axis, (x, y, z) 
,a boolean value is specified, in order to indicate that the scaling is applied.  
E.g. `[true, false, true]` means that the scaling will be applied on the x, z 
axes only.

**baseline** (`double`)

This is the assumed, system-wide, average value of each class metadata property.
E.g. A baseline of: `3` for a `Methods` scaling factor means that it is assumed
that on average a class of the visualized software contains 3 methods. Thus, 
vertices whose underlying classes contain more than 3 methods will appear 
larger, and vertices whose underlying classes contain less than 3 methods will
appear smaller.

Possible values: **any positive floating point number**.

**ratio** (`double`)

The min/max ratio values of each scaling factor underlying class metadata 
property, in comparison to the baseline, can be specified here.  
That way, with an e.g. min ratio value of `1`, we can be sure that vertices, 
whose  underlying class metadata property fall behind the baseline, will never 
appear smaller than the average one.  
Useful in order to prevent vertices from going invisible or appearing too big.

Possible values: **any positive floating point number**.


### Clustering

Archv features real-time graph clustering, with plugged in graph clustering 
algorithms.  
In addition, for many clustering algorithms, specific parameters can be 
configured.

Example .json configuration:

```json

"clustering" :
{
    "clusterers" :
    [
        "Louvain Method",
        "Layered Label Propagation",
        "Infomap"
    ],
    "min-spanning-tree-finders" : 
    [
        "Prim MST"
    ],
    "clusterer": "Infomap",
    "min-spanning-tree-finder" : "Prim MST",
    "k" : 3,
    "snn-threshold" : 5,
    "min-modularity" : 0.2,
    "llp-gamma" : 0.2,
    "llp-steps" : 2
}

```

**clusterers** (`string list`)  

The available clustering algorithms that can be selected at runtime.  

Possible values: 
<**k-Spanning Tree** | **Shared Nearest Neighbour** | **Strong Components** | 
**Maximal Clique Enumeration** | **Louvain Method** | 
**Layered Label Propagation** | **Infomap**>

**min-spanning-tree-finders** (`string list`)  

The available minimum spanning tree finding algorithms used by the **k-Spanning Tree** clustering algorithm.

Possible values: <**Prim MST** | **Kruskal MST**>

**clusterer** (`string`)  

The default clustering algorithm selected.  

Possible values: **one listed under clusterers**.

**min-spanning-tree-finder** (`string`)  

The default minimum spanning tree finding algorithm selected.  

Possible values: **one listed under min-spanning-tree-finders**. 

**k** (`int`)  

The "k" value of the **k-Spanning Tree** clustering algorithm.  

Possible values: **any positive integral number larger than one**.  

