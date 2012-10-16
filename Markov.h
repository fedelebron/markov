#include <iosfwd>
#include <list>
#include <map>
#include <random>
#include <utility>
#include <vector>

using std::default_random_engine;
using std::istream;
using std::list;
using std::map;
using std::pair;
using std::string;

class Markov {
 public:
  Markov(unsigned int order) : order_(order), engine_(time(0)) {
  }
  void initial_sentence(const list<string>& input);
  string next();
  void train(istream& input);

 private:
  typedef pair<double, string> weighted_string;
  typedef list<string> state;
  
  static string trim(string input);

  void create_transition(const state& s, const list<string>& followers);
  void create_transitions(const map<state, list<string>>& followers);
  void pick_initial_sentence();
  string pick_transition(const list<weighted_string>& choices);

  state current_state_;
  default_random_engine engine_;
  unsigned int order_;
  map<state, list<weighted_string>> transitions_;
};