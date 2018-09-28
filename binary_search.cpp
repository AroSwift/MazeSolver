#include <iostream>
#include <fstream>
using namespace std;

int binary_search( string s[], string, int num_items );

int main() {
  fstream input;
  input.open("strings.dat");
  int num_items = 100;
  string binary[num_items];

  for( int i = 0; i < num_items && !input.eof(); i++ ) {
    input >> binary[i];
  }

  input.close();

  string search;
  cout << "String to search for: ";
  cin >> search;

  int loc = binary_search(binary, search, num_items);
  cout << "Binary Returned: " << binary[loc];


  return 0;
}


int binary_search( string s[], string search, int num_items ) {
  int lower_bound = 0;
  int upper_bound = num_items -1;

  while(upper_bound >= lower_bound) {
    int mid = (lower_bound + upper_bound) / 2;

    if( s[mid] == search ) {
      return mid;
    } else if( search < s[mid] ) {
      upper_bound = mid + 1;
    } else {
      lower_bound = mid - 1;
    }

  }
}