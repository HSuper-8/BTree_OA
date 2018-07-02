// find example
#include <iostream>     // cout
#include <algorithm>    // find
#include <vector> 
#include <string>      // vector
#include "indice.h"

using namespace std;

PAGE newPage(int MAX){
  PAGE page;
  page = new struct BTPAGE;
  page->keyCount = 0;
  for(int i = 0; i < MAX; i++) page->keys.push_back("");
  for(int i = 0; i <= MAX; i++) page->childrenRRNs.push_back(-1);
  return page;
}

PAGE getPage(long int rrn, fstream& Idx, int MAX){
  PAGE page = newPage(MAX);
  Idx.seekp(rrn);
  string records, RRNs;
  int k = 0, i = 0;
  getline(Idx, records);
  while(k < records.size()){
    page->keys[i++].assign( records.substr(k, 16) );
    page->keyCount ++;
    k += 17;
  }
  getline(Idx, RRNs);
  k = 0;
  i = 0;
  while(k < RRNs.size()){
    page->childrenRRNs[i++] = stoi( RRNs.substr(k, 7) );
    k += 8;
  }
  page->RRN = rrn;
  return page;
}

PAGE getPage(long int rrn, fstream& Idx, int MAX);
PAGE newPage(int MAX);

int main () {
  PAGE page;
  fstream Idx("../res/indicelista.bt");
  string line;
  while(!Idx.eof()){
    getline(Idx, line);
    if(line.size() == 0) break;
  }
  int pos = Idx.tellp();
  Idx.seekp(pos - 1);
  Idx << "test" << endl;
  Idx.close();

  return 0;
}
