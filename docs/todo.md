# Architecture

## Components

	+ [x] Symbols

	+ [x] Symbol table

	+ [x] Dependencies graph

## Modules

	+ [x] Deserialization
	

# Clustering

## Algorithms

	+ [x] Louvain method

	+ [ ] Layered label propagation
 
 	+ [ ] Info map

## Features

	+ [x] Cluster with algorithm


# Layout / Topology

## Layout algorithms

	+ [x] Gursoy atun layout algorithm

	+ [ ] Random layout algorithm

## Topology types

	+ [x] Cube topology

	+ [x] Sphere topology

## Features

	+ [x] Update layout

	+ [x] Update topology

	+ [x] Revert to defaults


# Dependencies

	+ [x] Dynamic weight property map

## Features

	+ [x] Change dependency weight value

	+ [x] Restore defaults


## Shared

	+ [ ] Progress Bars

	+ [ ] Blocked signals when running heavy algorithms

	+ [x] Interactive Configuration (deserialization/serialization/defaults)

# Rendering

## Features

	+ [x] Boxes

	+ [x] Illumintation

	+ [x] Texts

	+ [x] Controlls Hud


# Controls

	+ [x] Free Camera 

	+ [x] Control Hud on/off

	+ [x] Frame Hud on/off

	+ [x] Pause/Resume

	+ [x] Quit


## User input sanitizing

# GUI

## Features

	+ [x] Helpers with question marks

	+ [ ] Mini Map

	+ [ ] Search Bar

	+ [ ] More configurations (all of em)

	
# 08 / 07 / 2022

	+ [x] Visualizer/layout sybsystem tests

	+ [x] Check logging best practices

	+ [x] Remake layout demo with user input

	+ [x] Read about message buses vs notiffier pattern


# 09 / 07 / 2022

	+ [x] Ogre input handling


# 21 / 07 / 2022

	+ [x] See MyGUI

	+ [n] Implement layout factory caching

	+ [x] Implement layout revert to defaults use case


# 29 / 07 / 2022

	+ [x] Clean up the app fix fullscreen

	+ [x] Make a mock gui


# 31 / 07 / 2022

	+ [n] OIS Input

	+ [x] Make responsive GUI


# 01 / 08 / 2022

	+ [x] Refactor architecture::graph dependencies out of each sybsystem

	+ [x] Introduce SDL2


# 03 / 08 / 2022

	+ [x] Fix cursor alignment issues with MYGUI

	+ [x] Write logs to archv.log file switch file every now and then

	+ [n] Application specific input listener (fullscreen/exit/pause etc)

	+ [x] Introduce states


# 05 / 08 / 2022

	+ [x] Seperate app into states

		+ [x] Introduce testing/utility package

		+ [x] Introduce undo/redo package

		+ [x] Introduce manager class for architecture package

		+ [x] Implement allfwd.hpp files at each package

		+ [x] Implement running state

		+ [x] Reimplement app
		
		+ [x] Implement paused state

	+ [x] Make GUI popup


# 06 / 08 / 2022

	+ [n] Zoom out on running state pause


# 08 / 08 / 2022

	+ [x] Finish GUI Mockup
	
		+ [x] Implement running state

		+ [x] Implement app
		
		+ [x] Implement paused state

			+ [x] Integrate imgui

			+ [x] Make demo with imgui demo window across states

			+ [x] Make pause menu gui

	+ [n] Create new GUI Mockups


# 09 / 08 / 2022

	+ [x] Dependencies responsive gui

	+ [x] Clang tidy check


# 13 / 08 / 2022

	+ [x] At Layout interactors run no checks before applying functions
	(What if only the dependencies weights change?)

	+ [n] Exceptions instead of assertions on invalid requests

# 14 / 08 / 2022

	+ [x] Undo/Redo frontend

	+ [x] Flow from data to gui

	+ [x] Restore defaults buttons

	+ [x] Default selected gui items

	+ [x] Frame dispatching cleanup


# 15 / 08 / 2022

	+ [x] Central deserialization module


# 16 / 08 / 2022

	+ [x] Dependencies deserialization module

	+ [x] Layout deserialization module

	+ [x] Integrate configs

	+ [x] Death tests cleanup

	+ [n] Add hardcoded default config values



# 17 / 08 / 2022

	+ [n] Fix layout restore use case
	
	+ [n] Progress bars pre work


# 18 / 08 / 2022

	+ [?] Rendering package wth states and app -> Gui package, remove view


# 19 / 08 / 2022

	+ [x] Multithreaded task test

	+ [x] Arch generator task
	
	+ [x] Gui overlay system

	+ [x] Application package

		+ [x] Rename deserialization modules to config

		+ [x] Remove config package

		+ [x] Copy modules to application package

		+ [x] Remove view dispatchers

		+ [x] Remove view


# 20 / 08 / 2022

	+ [n] Architecture generation task with progress bar


# 26 / 08 / 2022

	+ [x] Refactor arch generation - no monotoring

	+ [x] Remove progress bar gui

	+ [x] Cleanup app from asio

	+ [n] Make a separate file dialog

	+ [x] Make Graph Renderer dependent on the graph


# 27 / 08 / 2022

	+ [x] (Clang) Tidy

		+ [x] Remove unneeded input listeners on visualization state

	+ [?] Check for Ogre leaks?


# 29 / 08 / 2022

	+ [x] Arch generation via ui (no progress bar)

		+ [n] layout::core::set_graph()

		+ [x] Dynamic graph setup via ui

		+ [x] Remove graph renderer setter

		+ [n] Shutdown on app if not null

		+ [n] Avoid unnecessary subsystem setups 
 
	+ [n] Config from project root


# 02 / 09 / 2022

	+ [x] Background Renderer

		+ [x] Studio Skybox

		+ [x] Configs

	
# 03 / 09 / 2022

	+ [ ] Graph Renderer

		+ [x] Configs
		
		+ [x] Box meshes

		+ [ ] Arrow Edges

	+ [n] Slider layout scale


# 04 / 09 / 2022

	+ [n] Any configurable layout
	
	+ [x] Graph Renderer

		+ [x] Configs
		
		+ [x] Box meshes

		+ [x] Line edges (Configurable)

		+ [n] Direction on edges


# 06 / 09 / 2022

	+ [x] Backends Cleanup

		+ [x] Layout

		+ [x] Dependencies

	+ [n] Graph Data Access Layer

	+ [x] Application Layer


# 09 / 09 / 2022

	+ [x] Id text billboards on each vertex

		+ [x] Id overlays

		+ [x] Configurable

	+ [n] Vertex material


# 10 / 09 / 2022

	+ [x] GUI to config style


# 11 / 09 / 2022

	+ [ny] GUI Icons https://stackoverflow.com/questions/69189190/dear-imgui-how-to-create-buttons-with-icons-and-text

	+ [x] GUI Rendering Configurator

	+ [x] GUI GUI Configurator

	+ [x] GUI Dependencies Editor 

	+ [x] GUI Layout Editor 

	+ [x] GUI Config Facade / GUI as a UI Component

	+ [x] GUI Graph Actions Cleanup

	+ [ ] GUI File Browser


# 17 / 09 / 2022 

	+ [x] GUI Defaults

	+ [x] GUI Plugins


# 20 / 09 / 2022 

	+ [x] GUI resources

		+ [x] Layout plugins

		+ [x] Configure Resources manually

	+ [n] GUI Editor Hooks

	+ [x] GUI Connections


# 21 / 09 / 2022

	+ [x] Background Renderer Config

	+ [x] Graph Renderer Config


# 22 / 09 / 2022

	+ [x] Graph Renderer Cleanup

	+ [x] Edge Arrows Rendering


# 23 / 09 / 2022

	+ [x] Edge tips cleanup

	+ [n] Configurable vertex material

	+ [x] Configurable edge tips


# 30 / 09 / 2022

	+ [x] Vertex scaling depending on symbol metadata

	+ [x] Vertex scaling with gui editing
	

# 01 / 10 / 2022

	+ [n] Overlays https://wiki.ogre3d.org/-Overlay

	+ [ ] Edge dependencies rendering

		+ [ ] See https://wiki.ogre3d.org/-Particle


# 03 / 10 / 2022

	+ [n] Parallel edges

	+ [x] Misc cleanup

	+ [x] Vertex id overlay on seperate nodes (don't scale with vertex).

	+ [x] File browser


# 04 / 10 / 2022

	+ [x] Multiple dependencies

	+ [ ] Edge types rendering


# 05 / 10 / 2022

	+ [x] Fix graph redraw and edge tips

	+ [x] Edge types rendering


# 09 / 10 / 2022

	+ [x] Handle dependency type rendering on parallels

	+ [x] Edge text configuration

	+ [x] Edge tip mesh


# 11 / 10 / 2022

	+ [x] Edge weight rendering


# 12 / 10 / 2022

	+ [x] See effects (https://wiki.ogre3d.org/-Particle)

	+ [x] Proof of concept in-degree rendering with particles


# 13 / 10 / 2022

	+ [n] See Particle Universe editor https://wiki.ogre3d.org/tiki-index.php?page=Particle%20Universe%20plugin

	+ [n] Install Particle Universe editor


# 15 / 10 / 2022

	+ [x] Easily configurable renderer with degree particle systems


# 18 / 10 / 2022

	+ [x] Degrees particle system management backend

	+ [x] Particle Systems Editor			


# 24 / 10 / 2022

	+ [x] Particle system degree renderer

	+ [x] Introduce config modules to ease compile times

	+ [x] Gui configuration exceptions

	+ [x] Create nifty particle systems for in/out-degrees (3 each)
	Note: Hollow Ellipsoid Emitter (3d ring)

		+ [x] In-degree systems (Direction inward)

			+ [x] Heavy

			+ [x] Medium

			+ [x] Light
			
		+ [x] Out-degree systems (Direction outward)

			+ [x] Heavy

			+ [x] Medium

			+ [x] Light

	+ [x] Gui configurable degree particle systems

		+ [x] Gui editor

		+ [x] Gui preparation

		+ [x] Degree editing commands

		+ [x] Gui connections

	+ [x] Degrees user input sanitizing

	+ [n] File Browser


# 27 / 10 / 2022

	+ [n] Compiler flags controlling wether to render degrees/clusters immediately

	+ [x] POC clustering rendering

		+ [x] k-Spanning Tree clusterer

			+ [x] Prim MST

			+ [x] Kruskal MST

		+ [x] Backend

		+ [x] Cluster vertex property map

		+ [x] Json configurable


# 28 / 10 / 2022

	+ [x] Expand Graph Renderer to color clusters

	+ [x] Rendering Cluster Color Coder policy class


# 31 / 10 / 2022

	+ [x] Hide clusters at graph renderer, draw update

		+ [x] Add vertex material configuration

		+ [x] Revert to config materials on hide

		+ [x] Do not draw materials on draw if clustered

	+ [x] Integrate the rest of the clusterers

		+ [x] Shared nearest neighbour clustering


# 01 / 11 / 2022

	+ [x] Integrate the rest of the clusterers

		+ [x] Strong Components


# 02 / 11 / 2022

	+ [x] Integrate the rest of the clusterers

		+ [x] Maximal Clique Enumeration

	+ [x] GUI Configurable clusterers


# 04 / 11 / 2022

	+ [x] Clustering GUI Commands

	+ [x] Clustering GUI Connections

	+ [x] GUI Help Markers

	+ [x] GUI Icons
	

# 05 / 11 / 2022

	+ [x] Louvain Method


# 07 / 11 / 2022

	+ [n] Fix cluster type typedef

	+ [x] Implement the rest of the backend objects for louvain clustering

	+ [x] Alternatives to rendering materials for clustering


# 09 / 11 / 2022

	+ [x] Edge tip material configuration

	+ [x] Configs tweaks
	
	+ [x] Clustering rendering update

	+ [x] Louvain method randomness fix
	
	+ [x] Introduce the Louvain min parameter on the clustering backend

	+ [x] Interactive Louvain Method settings for nerds


# 10 / 11 / 2022

	+ [n] GUI Configs

	+ [n/y] Multithreaded clustering

	+ [n/y] Layered Label Propagation pre work

	+ [x] Info overlays

		+ [x] Clickable vertices

	+ [x] Gui structure dialog manager


# 11 / 11 / 2022

	+ [ ] Gui structure dialog

	+ [ ] Presentation Layer

	+ [ ] Input unit tests


# 13 / 11 / 2022

	+ [x] Cameraman input lock/unlock

	+ [x] Display GUI always

	+ [x] HUD

	+ [x] Frames

	+ [x] Gui cleanup, extract menu and global state

		+ [x] Overlays hierarchy

		+ [x] Extract menu bar

		+ [x] Extract global state


# 14 / 11 / 2022

	+ [x] Finished up refactored input

	+ [x] Input cleanup


# 15 / 11 / 2022

	+ [x] Structure popups

	+ [x] Vertex effect on hover


# 16 / 11 / 2022

	+ [x] Poc popup code inspection

	+ [x] Popup code inspection with all info

	+ [x] Fix button click resolution on inspection

	+ [x] Menu bar num vertices/edges

	+ [x] Menu bar TODO

		+ [x] Menu bar helper

		+ [x] Menu bar Quit

	+ [x] Hide clusters before drawing graph


# 17 / 11 / 2022

	+ [n/y] GUI Configs

	+ [x] Layered label propagation


# 19 / 11 / 2022

	+ [x] Layered label propagation plugin

	+ [x] Layered label propagation backend details

	+ [x] Layered label propagation gui details

	+ [x] Layered label propagation commands

	+ [x] Presentation Layer


# 20 / 11 / 2022

	+ [x] Fwd typedefs issue

	+ [x] Clustering undo/redo

	+ [x] LLP GUI helpers

	+ [x] Fix input propagation from gui

	+ [ ] Graph interface for accessing vertices by ids


# 21 / 11 / 2022

	+ [ ] Search bar with camera pan

		+ [x] Vertex Marker

		+ [x] Graph Interface functions

		+ [x] Vertex position by id

		+ [ ] Camera pan function

		+ [ ] Menu search bar

	+ [ ] Info Map
