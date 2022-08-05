# Architecture

## Components

	+ [x] Symbols

	+ [x] Symbol table

	+ [x] Dependencies graph

## Modules

	+ [x] Deserialization
	

# Clustering

## Algorithms

	+ [ ] Louvain method

	+ [ ] Layered label propagation
 
 	+ [ ] Info map

## Features

	+ [ ] Cluster with algorithm


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

	+ [ ] Seperate app into states

		+ [x] Introduce testing/utility package

		+ [x] Introduce undo/redo package

		+ [x] Introduce manager class for architecture package

		+ [x] Implement allfwd.hpp files at each package

		+ [x] Implement running state

		+ [x] Reimplement app
		
		+ [x] Implement paused state

	+ [ ] Make GUI popup