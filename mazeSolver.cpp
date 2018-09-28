//
// Name: Aaron Barlow
// Date: 2/23/2016
// Description: Read in a given file with a maze and attempt to solve it.
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cctype>
#include "mazeSolver.h"
using namespace std;


int main() {
  ifstream input;
  Maze *m = new Maze();

  // Open file
  get_file( input );
  // Get the dimensions of the maze
  get_dimensions( input, m );

  // Dynamically allocate a 2d array of chars
  m->maze = new char*[m->num_rows+1];
  for(int i = 0; i < m->num_rows+1; ++i) {
    m->maze[i] = new char[m->num_cols+1];
  }

  // Load the maze into a 2d array of chars
  load_maze( input, m );

  // Close the file
  input.close();

  // Attempt to solve maze
  bool solved = solve_maze( m );

  // Display state of maze solvability
  if(solved) {
    print_maze( m );
  } else {
    cout << "The maze can not be solved." << endl;
  }

  // Delete dynamically allocated 2d array
  for(int row = 0; row < m->num_rows+1; ++row) delete [] m->maze[row];
  delete [] m->maze;
  // Delete the maze after deleting all instantiations
  delete m;

  return 0;
}

//
// get_file
// Continuously prompts user for a file and
// Then attempts to get that file until the file is opened.
//
void get_file( ifstream& input ) {
  char filename[MAX_FILE_LENGTH];
  bool file_errors;

  do { // Find a file that exists
    file_errors = false;

    // Prompt for file and read it in
    cout << "Enter file name: ";
    cin.getline( filename, MAX_FILE_LENGTH );

    input.open( filename );

    // When file could not be found
    if( input.fail() ) {
      cout << "Input file " << filename << " does not exist. \n";
      file_errors = true;

    // When file is empty
    } else if( input.peek() == EOF ) {
      cout << "Input file " << filename << " is empty. \n";
      file_errors = true;
    }

  } while( file_errors );

}

//
// get_dimensions
// Get the dimensions of the maze from a file.
// Dimensions are row then col and on the first line of the file.
//
void get_dimensions( ifstream& input, Maze *m ) {
  input >> m->num_rows;
  input >> m->num_cols;

  // Ignore new line to go to the beginning of maze
  input.ignore(1, '\n');
}

//
// load_maze
// Load the maze into a 2d demensional array of chars
//
void load_maze( ifstream& input, Maze *m ) {
  // Read in file row by row
  for( int row = 0; row < m->num_rows; row++ ) {
    // Ensure null character is accounted for in file
    input.getline( m->maze[row], m->num_cols+1 );
  }
}

//
// solve_maze
// Attempts to solve the maze while not solved.
// It goes down, right, left, and up and push cords onto a stack.
// When there are multiple options, push those cords onto a seperate stack.
// When maze is solved or unsolvable, stop attempting to solve maze.
//
bool solve_maze( Maze *m ) {
  Stack current_location, options_location;
  initStack(&current_location);
  initStack(&options_location);
  bool solved = false;

  // Set the starting row and col of maze
  Cords* start_position = new Cords();
  start_position->row = m->cords.row = START_ROW;
  start_position->col = m->cords.col = START_COL;
  void* sp = start_position;
  // Push starting point onto stack
  push(&current_location, sp);

  // Set the starting point to an asterisk
  m->maze[m->cords.row][m->cords.col] = '*';

  // Continue to attempt to solve the maze until
   // there are no possible moves or the maze is unsolvable
  while(!solved) {
    bool switched_location = false;

    // Go down when possible
    if( m->down_possible() ) {
      m->cords.row++;
      switched_location = true;
    }

    // Go right when possible and not already moved
    if( !switched_location && m->right_possible() ) {
      m->cords.col++;
      switched_location = true;
    }

    // Go left when possible and not already moved
    if( !switched_location && m->left_possible() ) {
      m->cords.col--;
      switched_location = true;
    }

    // Go up when possible and not already moved
    if( !switched_location && m->up_possible() ) {
      m->cords.row--;
      switched_location = true;
    }

    if(switched_location) {
      // Set the current position to an astrick because it is a possible path
      m->maze[m->cords.row][m->cords.col] = '*';

      // Push the current position of the maze of the maze onto the stack
      Cords* current_postion = new Cords();
      current_postion->row = m->cords.row;
      current_postion->col = m->cords.col;
      void* cp = current_postion;
      push(&current_location, cp);
    }

    if( m->num_options() > 1 ) {
      // Push onto the options stack when there is more than one option
      Cords* options_position = new Cords();
      options_position->row = m->cords.row;
      options_position->col = m->cords.col;
      void* op = options_position;
      push(&options_location, op);

    // When no options
    } else if( m->num_options() == 0 ) {
      if( m->is_exit() ) {
        // Maze is solved
        solved = true;
        // If there are no options
      } else if( isEmpty(&options_location) ) {
        // It is impossible to solve the maze
        solved = false;
        // So exit loop
        break;
      } else { // Option stack is not empty
        // Go back to the location of maze that had options
        m->revert_options( &current_location, &options_location );
      }
    }

  } // end while loop

  // State of maze resolution
  return solved;
}

//
// down_possible
// Return whether going below the maze
// From the current position is possible.
//
bool Maze::down_possible() {
  // Is moving the current position of the row down by 1 possible?
  bool valid_row = cords.row+1 >= 0 && cords.row+1 <= num_rows;
  // Is moving the current position of the column possible?
  bool valid_col = cords.col >= 0 && cords.col <= num_cols;

  // When going down is possible and the character of that
  // possition is a possible path return true otherwise false
  return valid_row && valid_col && is_path(get_down()) ? true : false;
}

//
// right_possible
// Return whether goign right of the maze
// From the current position is possible.
//
bool Maze::right_possible() {
  // Is moving the current position of the row possible?
  bool valid_row = cords.row >= 0 && cords.row <= num_rows;
  // Is moving the current position of the column right by 1 possible?
  bool valid_col = cords.col+1 >= 0 && cords.col+1 <= num_cols;

  // When going right is possible and the character of that
  // possition is a possible path return true otherwise false
  return valid_row && valid_col && is_path(get_right()) ? true : false;
}

//
// left_possible
// Return whether going left of the maze
// From the current position is possible.
//
bool Maze::left_possible() {
  // Is moving the current position of the row possible?
  bool valid_row = cords.row >= 0 && cords.row <= num_rows;
  // Is moving the current position of the column left by 1 possible?
  bool valid_col = cords.col-1 >= 0 && cords.col-1 <= num_cols;

  // When going left is possible and the character of that
  // possition is a possible path return true otherwise false
  return valid_row && valid_col && is_path(get_left()) ? true : false;
}

//
// up_possible
// Return whether moving up the maze
// From the current position is possible.
//
bool Maze::up_possible() {
  // Is moving the current position of the row  up by 1 possible?
  bool valid_row = cords.row-1 >= 0 && cords.row-1 <= num_rows;
  // Is moving the current position of the column possible?
  bool valid_col = cords.col >= 0 && cords.col <= num_cols;

  // When going up is possible and the character of that
  // possition is a possible path return true otherwise false
  return valid_row && valid_col && is_path(get_up()) ? true : false;
}

//
// get_down
// Return the character that is below the current maze position.
//
char Maze::get_down() {
  return maze[cords.row+1][cords.col];
}

//
// get_right
// Return the character that is right of the current maze position.
//
char Maze::get_right() {
  return maze[cords.row][cords.col+1];
}

//
// get_left
// Return the character that is left of the current maze position.
//
char Maze::get_left() {
  return maze[cords.row][cords.col-1];
}

//
// get_up
// Return the character that is above the current maze position.
//
char Maze::get_up() {
  return maze[cords.row-1][cords.col];
}

//
// num_options
// Return the number of possible options that can be
// traversed from the current position of the maze.
//
int Maze::num_options() {
  int options = 0;

  if( down_possible() ) options++;
  if( right_possible() ) options++;
  if( left_possible() ) options++;
  if( up_possible() ) options++;

  // Return the number of possible paths that can be taken
  return options;
}

//
// revert_options
// Pop back to the last location that options were available.
// While popping back, set asterisks to dashes.
//
void Maze::revert_options( Stack *current, Stack *options ) {
  bool is_reverted = false;

  // Get the position where more than one option exists
  Cords *options_pos = (Cords*)pop( options );

  // Continue to pop back current position until that position is option position
  while(!is_reverted) {
    // Get the new current position
    Cords *current_pos = (Cords*)pop(current);
    // Set the current position to the new cords
    cords = *current_pos;

    // Set the asterisks to dashes when current position is not option position
    if( current_pos->row != options_pos->row || current_pos->col != options_pos->col ) {
      maze[current_pos->row][current_pos->col] = '-';
    } else {
      // The the current position now is the option position
      is_reverted = true;
    }

  }

  // Push back onto stack to set the new current position
  Cords* new_current_pos = new Cords();
  new_current_pos->row = cords.row;
  new_current_pos->col = cords.col;
  void* ncp = new_current_pos;
  push(current, ncp);

}

//
// is_edge
// Return whether the current position is an edge of the maze.
//
bool Maze::is_edge() {
  // Is the current row the edge?
  bool row_edge = (cords.row == 0 || cords.row == num_rows-1) &&
    (cords.row >= 0 && cords.row <= num_rows-1);

  // Is the current col the edge?
  bool col_edge = (cords.col == 0 || cords.col == num_cols-2) &&
    (cords.col >= 0 && cords.col <= num_cols-2);

  // If the current col or row is at the edge then return true otherwise false
  return row_edge || col_edge;
}

//
// is_exit
// Return whether the current position of the maze is the exit.
//
bool Maze::is_exit() {
  // Current position is exit when at an edge and set to an asterisk
  if( is_edge() && maze[cords.row][cords.col] == PATH_TAKEN ) {
    return true;
  } else {
    return false;
  }
}

//
// is_path
// Return whether the given character is a path.
//
bool is_path( char c ) {
  return c == PATH ? true : false;
}

//
// print_maze
// Print completed maze.
//
void print_maze( Maze *m) {
  // Iterate through 2d array and print out by row
  for( int row = 0; row < m->num_rows; row++ ) {
    cout << m->maze[row] << endl;
  }
}

