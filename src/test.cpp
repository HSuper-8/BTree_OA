// find example
#include <iostream>     // cout
#include <algorithm>    // find
#include <vector> 
#include <string>      // vector
#include "indice.h"

using namespace std;

string CreateKey(string line){
  string record;
  record.push_back(toupper(line[0]));
  record.push_back(toupper(line[1]));
  record.push_back(toupper(line[2]));
  return record + line.substr(41, 5);
}

string createRRN(long int rrn){
  string RRN;
  RRN.assign(to_string(rrn));
  if(RRN.size() < 7)
    while(RRN.size() != 7) RRN.push_back(' ');
  else 
    return (to_string(rrn)).substr(0, 7);
  return RRN;
}

PAGE getPage(long int rrn, int MAX){
  fstream Idx;
  Idx.open("../res/indicelista.bt");
  PAGE page = newPage(MAX);
    Idx.seekg(rrn);
    string records, RRNs;
    int k = 0, i = 0;
    getline(Idx, records);
    while(k < records.size()){
      string str = "                ";
      if(str.compare(records.substr(k, 16)) == 0) break;
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
    Idx.close();
    return page;
}

void writePage(PAGE page, bool newPage, int MAX){
  int k;
  fstream Idx;
    if(!newPage){
      Idx.open("../res/indicelista.bt");
      Idx.seekp(page->RRN);
    }
    else{
      Idx.open("../res/indicelista.bt", std::ios::app);
    }

    for(k = 0; k < MAX; k++)
      Idx << page->keys[k] << " ";

  int pos = Idx.tellp();
    page->RRN = pos - (MAX * 17);
    Idx << "\n";

  for(k = 0; k < page->keyCount + 1; k++)
      Idx << createRRN(page->childrenRRNs[k]) << " ";
    if(k < MAX+1){
      while(k+1 <= MAX+1){
        Idx << createRRN(-1) << " ";
        k++;
      }
    }
  Idx << "\n";
  Idx.close();
}

PAGE newPage(int MAX){
  PAGE page;
  page = new struct BTPAGE;
  page->keyCount = 0;
  for(int i = 0; i < MAX; i++) page->keys.push_back("                ");
  for(int i = 0; i <= MAX; i++) page->childrenRRNs.push_back(-1);
  return page;
}

PAGE newPage(int MAX);
void writePage(PAGE page, bool newPage, int MAX);
PAGE getPage(long int rrn, int MAX);
string createRRN(long int rrn);
string CreateKey(string line);

int main () {
  PAGE page;
  //fstream Idx("../res/indicelista.bt");
  page = newPage(3);

  page = getPage(20 + 85, 3);
  return 0;
}
