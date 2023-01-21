# Configuration

Archv supports both interactive configuration, via GUI, and startup 
configuration with .json config file(s).  
This section will explain to you how to best configure the application's style, 
and/or graph visualization properties.

> **_NOTE:_** Wether each subsystem is configured seperately at its own .json 
file, or a single config file is used for the entire application will be decided
at **build** time. 
(See [`build.md`](https://github.com/SoultatosStefanos/archv/blob/master/docs/build.md) ).


## Table of Contents  

[Layout](#layout)  
[Weights](#weights)  
[Scaling](#scaling)  
[Clustering](#clustering)  
[Color Coding](#color-coding)  
[Rendering](#rendering)  
[Gui](#gui)

## Layout

The graph's visualization properties regarding its layout & topology in 3D space 
can be configured with plugged in algorithms and values.  

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

Possible values: <**Gursoy Atun** | **Random**>

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


## Weights

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


## Scaling

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

**ratio**  

**min** (`double`)   
**max** (`double`)  

The min/max ratio values of each scaling factor underlying class metadata 
property, in comparison to the baseline, can be specified here.  
That way, with an e.g. min ratio value of `1`, we can be sure that vertices, 
whose  underlying class metadata property fall behind the baseline, will never 
appear smaller than the average one.  
Useful in order to prevent vertices from going invisible or appearing too big.

Possible values: **any positive floating point number**.


## Clustering

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
    "intensity" : 2000,
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

The available minimum spanning tree finding algorithms used by the 
**k-Spanning Tree** clustering algorithm.

Possible values: <**Prim MST** | **Kruskal MST**>

**clusterer** (`string`)  

The default clustering algorithm selected.  

Possible values: **one listed under clusterers**.  

**intensity** (`double`)  

The scale of "clustered" layout, which attempts to showcase the adjacency of the 
clusters.  

Possible values: **any floating point number**.  

**min-spanning-tree-finder** (`string`)  

The default minimum spanning tree finding algorithm selected.  

Possible values: **one listed under min-spanning-tree-finders**. 

**k** (`int`)  

The "k" value of the **k-Spanning Tree** clustering algorithm.  

Possible values: **any positive integral number larger than one**.  

**snn-threshold** (`int`)  

The "t" value of the **Shared Nearest Neighbour** clustering algorithm.

Possible values: **any integral number**.  

**min-modularity** (`double`)  

The minimum required modularity increase at each modularity optimization step, 
of the **Louvain Method** clustering algorithm.  
The resoultion of the algorithm.  

Possible values: **any floating point number**.  

**llp-gamma** (`double`)  

The **Layered Label Propagation** clustering algorithm "gamma" variable.

Possible values: **any floating point number**.  

**llp-steps** (`double`)  

The maximum number of iterations of the **Layered Label Propagation** clustering
algorithm.

Possible values: **any integral positive number**.  


## Color Coding  

Physical dependencies across C++ components are expressed with edges in the 
visualized dependencies graph. The "color" value of each type of dependency can
be specified here.  

Example .json configuration:

```json

{
    "color-coding" : 
    {
        "dependencies" : 
        [
            {
                "dependency" : "Inherit",
                "color" : [ 255, 0, 0, 1 ],
                "active" : true
            },
        ]
    }
}

```    

Where each **dependency** (`string`) is paired with a **color** (`double array`), 
and an **active** (`boolean`) value, which indicates wether the color coding 
will be rendered. 

> **_NOTE:_** Each dependency found in the graph .json input file must be 
included here.


## Rendering

Archv can be configured regarding the architecture graph's "external" rendering 
properties, like the materials and meshes used to render the scene.  
Additionally, Archv features particle system effects rendering at each vertex, 
depending on the vertex in/out degree. Some configuration options for these 
effects can be configured as well.

Example .json configuration:

```json

"rendering" :
{
    "background" : 
    {
        "skybox-material" : "Gradient/Dark",
        "skybox-distance" : 5000,
        "ambient-color" : [ 0, 0, 0 ],
        "diffuse-color" : [ 0.4, 0.43, 0.42 ],
        "specular-color" : [ 0.337, 0.325, 0.325 ],
        "cam-far-clip-distance" : 0,
        "cam-near-clip-distance" : 5
    },
    "graph" : 
    {
        "vertex-mesh" : "Cube.001.mesh",
        "vertex-material" : "Shaded/StudioGold",
        "vertex-scale" :  [	5, 5, 5 ],
        "vertex-id" : 
        {
            "font-name" : "Gecko-Personal",
            "char-height" : 6,
            "color" : [ 1, 1, 1 ],
            "space-width" : 0
        },
        "edge-material" : "Shaded/LightBlue",
        "edge-tip-mesh" : "triangle.mesh",
        "edge-tip-material" : "Shaded/LightBlue",
        "edge-tip-scale" : [ 1.25, 1.25, 1.25 ],
        "edge-type" :
        {
            "font-name" : "Gecko-Personal",
            "char-height" : 2.5,
            "color" : [ 1, 1, 1 ],
            "space-width" : 0
        }
   },
    "degrees" :
    {
        "in-degree" :
        {
            "thresholds" : 
            { 
                "light" : 1,
                "medium" : 4,
                "heavy": 8
            },
            "particle-systems" : 
            {
                "light" : "Degrees/SwarmLight",
                "medium" : "Degrees/SwarmMedium",
                "heavy" : "Degrees/SwarmHeavy" 
            },
            "applied" : true
        },
		"out-degree" :
        {
            "thresholds" : 
            { 
                "light" : 1,
                "medium" : 4,
                "heavy": 8
            },
            "particle-systems" : 
            {
                "light" : "Degrees/SwarmLight",
                "medium" : "Degrees/SwarmMedium",
                "heavy" : "Degrees/SwarmHeavy" 
            },
            "applied" : false
        },
    },
    "minimap" : 
    {
        "left" : 0.5,
        "top" : -0.5,
        "right" : 0.95,
        "bottom" : -0.95,
        "background-color" : [ 0.0, 0.0, 0.0 ],
        "zoom-out" : 300,
        "render-shadows" : false,
        "render-sky" : true,
        "render-vertices" : true,
        "render-vertex-ids" : false,
        "render-edges" : true,
        "render-edge-types" : false,
        "render-edge-tips" : false,
        "render-particles" : false
    }
}

```

Possible mesh values: **any .mesh file name defined under an imported directory**.  
(See **Set-Up** section at: 
[`build.md`](https://github.com/SoultatosStefanos/archv/blob/master/docs/build.md) )

Possible materials values: **any material name defined in any imported .material script**.  
(See **Set-Up** section at: 
[`build.md`](https://github.com/SoultatosStefanos/archv/blob/master/docs/build.md) )

Possible color values: **any floating point value between 0 and 1**.  
(See colour sections at: https://wiki.ogre3d.org/Materials).  

Possible font name values: **any font name defined in an imported .fontdef file**.  
(See **Set-Up** section at: 
[`build.md`](https://github.com/SoultatosStefanos/archv/blob/master/docs/build.md) )

The particle system effects rendering options can be specified for both in and 
out vertex degrees.  

For each degree the following can be specified:  

**thresholds**  

**light** (`int`)  
**medium** (`int`)  
**heavy** (`int`)  

Here, the thresholds for when to render each effect for each degree value are 
specified.  

Possible values: **any integral positive number**.

**particle-systems**  

**light** (`string`)  
**medium** (`string`)  
**heavy** (`string`)  

Here, the name of the particle system to be rendered for each level of degree 
is specified. When a vertex in/out degree is between the light and medium 
thresholds, then the light particle system is rendered, when it's between the 
medium and heavy thresholds, then the medium particle system is rendered, and 
when it's above, or equal to the heavy threshold, then the heavy particle system 
is rendered.

Possible particle system name values: **any particle system name defined in an imported .particle file**.  
(See **Set-Up** section at: 
[`build.md`](https://github.com/SoultatosStefanos/archv/blob/master/docs/build.md) )

**applied** (`bool`)  

Wether to render the particle system for each type of vertex degree.  


## Gui

Archv's gui style can be configured as well.  

Example .json configuration:

```json

"gui" :
{
    "color-theme" : "Dark",
    "frame-rounding" : 0,
    "window-bordered" : true,
    "frame-bordered" : true,
    "popup-bordered" : true 
}

```

**color-theme** (`string`)  

The color theme of the gui.  

Possible values: <**Dark** | **Light** | **Classic**>  

**frame-rounding** (`double`)  

Specifies how rounded the frames of the gui appear.  

Possible values: **any floating point number**.  

**window-bordered** (`bool`)  

Wether the windows will appear bordered.  

**frame-bordered** (`bool`)  

Wether the frames will appear bordered.  

**popup-bordered** (`bool`)  

Wether the popups will appear bordered.