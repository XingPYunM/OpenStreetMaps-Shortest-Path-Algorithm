## Open Street Maps - Shortest Path Algorithm
UIC Data Structures Course Project

## About This Project
 Dijkstra's algorithm can be used to find the shortest path between two points on a graph.

In this project, students are provided OpenStreetMap data about UIC's campus and base code to load the map data in. It is the student's job to implement a graph data structure, Dijkstra's algorithm, and make any and all design choices there after.

## What I learned
 - Dijkstra's algorithm
   - Used to calculate thedistance between a start and destination point via "footways".
   - Modified to be able to handle loops
 - Implementing a graph data structure
   -  Designed a generic graph data structure


## Files To Look At
Helper and driver files were provided, however, the builk of actual the project is written in these two files.

- graph.h
  - Generic graph that takes any two types to use for a vertex and a weight
- application 
  - Driver code for program
  - Dijkstra's algorithm written in this file
  
## How To Use
A makefile is provided in order to compile the program.
