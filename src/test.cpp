// find example
#include <iostream>     // cout
#include <algorithm>    // find
#include <vector> 
#include <string>      // vector
using namespace std;


int main () {
  string str;
  string str2 = "2";
  str = str2.substr(0, str2.size());
  int b = stoi(str2);
  cout << b << endl;



  vector<string> v;
  v.push_back("cat");
  v.push_back("dog");
  v.push_back("bird");
  v.insert(v.begin() + 2, "new");
  //std::cout << "Element found in myvector: " << *it << '\n';
  /* using default comparison:
  sort(v.begin(), v.end());

  cout << "looking for a bird... ";
  if (binary_search (v.begin(), v.end(), "bird"))
    cout << "found!\n"; else cout << "not found.\n";
  */
  return 0;
}