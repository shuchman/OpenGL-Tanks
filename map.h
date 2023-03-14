#include <bits/stdc++.h>
#include "stdio.h"
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3



class Map
{
    public:
    //int maze_size[2];
    std::vector< std::vector< int > > dfs_path;
    std::vector< std::vector< std::vector< bool > > > maze;
    int start_axis;
    int start_side;
    // Member Functions()
    bool randomMove(bool first_move);
    void generateMaze();
    void initializeMaze();
    void randomPoint(bool part);
  
    int maze_size[2];
    void map_constructor();
    void printMaze();
};