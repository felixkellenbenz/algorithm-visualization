# algorithm-visualization
This is an application to visualize and explore path finding algorithms on
a 2D grid. It was built to get more familiar with SDL2 and 
learn more about C++.

## Implemented Algorithms
The application is able to execute the following path finding
algorithms on a 2D grid:
1. Dijkstra
2. BFS
3. A*

## Development environment

### Software
    - Linux Ubuntu 23.10
    - CMake 3.27.4 
    - g++ 13.2.0 

### Hardware
    - Microsoft Laptop Studio
    - 11th Gen Intel Core i7

## Guide
This is a quick guid on how to edit the 2D grid, select start 
and end node, select an algorithms and execute the algorithm.

### Edit the grid
To edit the grid you have to enter edit mode you can do so by
pressing the **i - key** Once in edit mode you have the following options.
- **left mousebutton**: draw obstacles.
- **right mousebutton**: remove obstacles.
- **e - key**: create the end node at the location of the cursor.
- **s - key**: create the start node at the location of the cursor.
- **r - key**: reset the grid.
- **c - key**: remove the parts of the grid that were tainted by a algorithm

### Select an algorithm
Outside the edit mode you can chose between diffrent algorithms you want 
to execute on the grid.
- **1 - key**: Null Algorithm (execute nothing on the grid).
- **2 - key**: BFS.
- **3 - key**: Dijkstra (not implemented yet).
- **4 - key**: A* (not implemented yet).

You can begin the execution of an algorithm by pressing the **b - key**.
