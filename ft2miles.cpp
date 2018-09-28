#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

const double MILE = 5280;

int main( int num_args, char *arv[] ) {
  for( int i = 1; i < num_args; i++ ) {
    int feet = atoi(argv[i]);
    float miles = feet / 5280.0;
    cout <<
  }

}
