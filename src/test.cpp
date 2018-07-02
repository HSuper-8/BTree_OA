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

string createRRN(long int rrn){
  string RRN;
  RRN.assign(to_string(rrn));
  if(RRN.size() < 7)
    while(RRN.size() != 7) RRN.push_back(' ');
  return RRN;
}

void writePage(PAGE page, fstream& Idx, bool newPage, int MAX){
  int k;
  if(!newPage) Idx.seekg(page->RRN);
  else{
    Idx.close();
    Idx.open("../res/indicelista.bt", std::ios::app);
  }

  for(k = 0; k < page->keyCount; k++)
    Idx << page->keys[k] << " ";
  if(k+1 < MAX){
    while(k+1 <= MAX) Idx << "         ";
  }
  int pos = Idx.tellp();
  page->RRN = pos - (MAX * 17);
  Idx << "\n";

  for(k = 0; k < page->keyCount + 1; k++)
    Idx << createRRN(page->childrenRRNs[k]) << " ";
  if(k+1 < MAX+1){
    while(k+1 <= MAX+1) Idx << "        ";
  }
  Idx << "\n";
  
  if(newPage){
    Idx.close();
    Idx.open("../res/indicelista.bt");
  }
}

PAGE getPage(long int rrn, fstream& Idx, int MAX);
PAGE newPage(int MAX);

int main () {
  PAGE page;
  fstream Idx("../res/indicelista.bt");
  page = getPage(0, Idx, 3);
  writePage(page, Idx, true, 3);
  cout << page->RRN << endl;
  Idx.close();
  return 0;
}
