#include "Markov.h"
#include <algorithm>
#include <istream>
#include <map>
#include <random>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using std::default_random_engine;
using std::isalnum;
using std::list;
using std::map;
using std::remove_if;
using std::runtime_error;
using std::string;
using std::to_string;
using std::uniform_real_distribution;
using std::unordered_map;
using std::vector;

void Markov::train(istream& input) {
  state s;
  map<state, list<string>> followers;
  string buffer;
  for (int i = 0; i < order_; ++i) {
    if (input.eof()) {
      throw runtime_error(
        "Unexpected end of input, training requires at least "
        + to_string(order_) + " words.");
    }

    input >> buffer;
    buffer = trim(buffer);
    if(buffer == "") {
      i--;
      continue;
    }
    s.push_back(buffer);
  }
  
  while (input >> buffer) {
    buffer = trim(buffer);
    followers[s].push_back(buffer);
    s.pop_front();
    s.push_back(buffer);
  }

  create_transitions(followers);
}

void Markov::create_transitions(const map<state, list<string>>& followers) {
  for (const auto& entry : followers) {
    const state& s = entry.first;
    const list<string>& follow = entry.second;
    create_transition(s, follow);
  }
}

void Markov::create_transition(const state& s, const list<string>& followers) {
  if (!followers.size()) return;

  unordered_map<string, int> counts;
  int total = 0;
  for (const string& s : followers) {
    counts[s]++; 
    total++;
  }

  double dtotal = double(total);
  for (const auto& entry : counts) {
    double probability = double(entry.second)/dtotal;
    transitions_[s].push_back(weighted_string(probability, entry.first));
  }

}

string Markov::pick_transition(const list<weighted_string>& choices) {
  uniform_real_distribution<double> distribution(0.0, 1.0);

  double pick = distribution(engine_);
  double cumulative = 0;

  for (const weighted_string& choice : choices) {
    cumulative += choice.first;
    if (pick <= cumulative) return choice.second;
  }
}

void Markov::initial_sentence(const list<string>& input) {
  if (input.size() != order_) { 
    string order = to_string(order_);
    throw runtime_error("A Markov generator of order " + order +
                        " requires an initial sentence of " + order +
                        " words.");
  }
  current_state_ = input;
}

string Markov::next() {
  if (current_state_.empty()) return "";
  auto& possible_transitions = transitions_[current_state_];
  if (possible_transitions.size() == 0)
    throw runtime_error("Error: Invalid Markov chain state.");

  string next_word = pick_transition(possible_transitions);
  current_state_.pop_front();
  current_state_.push_back(next_word);
  return next_word;
}

string Markov::trim(string input) {
  auto remover = remove_if(input.begin(), input.end(), [](int t) {
    return !isalnum(t);
  });
  input.erase(remover, input.end());
  return input;
}
