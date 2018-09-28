//
// Name: Aaron Barlow
// Date: 2/23/2016
// Description: Read in a given file with a maze and attempt to solve it.
//

#pragma once
#include <iostream>
#include "stack.c"
using namespace std;

const int MAX_FILE_LENGTH = 256; // Max file length on linux is 255

// A path is a space
const char PATH = ' ';
// The astrick indicates a path used to complete maze
const char PATH_TAKEN = '*';

// Starting point of maze should be [0,1]
const int START_ROW = 0;
const int START_COL = 1;

// Coordinates of maze position
struct Cords {
  int row, col;
};

// Structure for maze
struct Maze {
  char** maze;
  int num_rows, num_cols;
  Cords cords;

  bool down_possible();
  bool right_possible();
  bool left_possible();
  bool up_possible();

  char get_down();
  char get_right();
  char get_left();
  char get_up();

  int num_options();
  void revert_options( Stack *current, Stack *options );

  bool is_edge();
  bool is_exit();
};

void get_file(ifstream& input );
void get_dimensions(ifstream& input, Maze *m);

void load_maze(ifstream& input, Maze *m);
bool solve_maze(Maze *m);
void print_maze(Maze *m);

bool is_path( char c );
