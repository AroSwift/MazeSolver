#include <iostream>
using namespace std;

int summation( int n );

int main() {
  cout << "Enter number: ";
  int number;
  cin >> number;

  cout << summation(number) << endl;

  return 0;
}

int summation( int n ) {
  if( n == 1) return 1;
  return n + summation(n-1);
}