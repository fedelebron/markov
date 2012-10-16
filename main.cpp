#include <iostream>
#include "Markov.h"
#include <chrono>
#include <thread>

using namespace std;

int main(int argc, char** argv) {
  if(argc < 3) {
    cerr << "Usage: ";
    cerr << argv[0];
    cerr << " n initial string" << endl;
    return -1;
  }

  int n = atoi(argv[1]);
  list<string> initial;
  for(int i = 2; i < argc; ++i) initial.push_back(string(argv[i]));

  Markov markov(initial.size());
  markov.train(cin);
  markov.initial_sentence(initial);  

  for(const string& s : initial) cout << s << " ";
  for(int i = 0; i < n; ++i) {
    cout << markov.next() << " ";
  }
  cout << endl;
}