## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Setup](#setup)
* [Status](#status)

## General info
RAMES is a project dedicated to solving numerically heat transfer problem for 1-dimensional structures (2D in the future).
It utilizes Finite Element Method (FEM) in Galerkin approach.
	
## Technologies
Project is created with:
* Visual Studio 2019 ver. 16.8.4
	
## Setup
To build this project, run in Visual Studio commmand line:

```
> msbuild RAMES.sln
```

##Features
- Calculate temperature distribution across given structure
- Visualize results both as temperature gradient and graph of temperature vs position
- Output results to txt file in form of position - temperature

## Status
The project is still in development phase.
TO DO:
- 2D structures computations
- Interactive temperature gradient and graph in the View
- Non-uniform grid generator
- Iterative method of solving linear systems
- Computation time display
- Code clean up
