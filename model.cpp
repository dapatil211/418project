#include "model.h"
#include <iostream>
#include <map>
#include <set>

Model::Model(int k, int totalChars) : k(k) {
  for (int i = 0; i < totalChars; i++) {
    unfound.insert(i + 'a');
  }
  contextStartDec = 0;
}

// string Model::constructEscape(string s) {
//   string esc(s);
//   esc.replace(esc.length() - 1, 1, 1, '\xFF');
//   return esc;
// }

void Model::updateModel(char c) {
  prevChars.push_back(c);
  for (uint i = 0; i < prevChars.size(); i++) {
    context.insert(prevChars, i);
  }
  //     string key(it, prevChars.end());
  //     ht::iterator hit = context.find(key.data());
  //     if (hit == context.end()) {
  //       context.insert(key.data(), .5);
  //       // string esc = constructEscape(key);
  //       // ht::iterator esc_it = context.find(esc.data());
  //       // if(esc_it == context.end()){
  //       //     context.insert(esc.data(), .5);
  //       // } else{
  //       //     *esc_it = *esc_it + .5;
  //       // }
  //     } else {
  //       *hit = *hit + 1;
  //     }
  //   }
  if (prevChars.size() > k) {
    prevChars.pop_front();
  }
}

// void Model::addRange(ht::iterator hit,
//                      pair<ht::prefix_iterator, ht::prefix_iterator> range,
//                      vector<tuple<int, int, int>> &probabilities,
//                      set<char> &unusable) {}

vector<tuple<double, double, double>> Model::getProbability(char c) {
  vector<tuple<double, double, double>> probabilities;
  prevChars.push_back(c);
  set<char> unusable;
  // for (deque<char>::iterator it = prevChars.begin(); it != prevChars.end();
  // it++){
  for (uint i = 0; i < prevChars.size(); i++) {
    // string prefix(it, prevChars.end() - 1);
    // string key(it, prevChars.end());
    // ht::iterator hit = context.find(key);
    // auto range = context.equal_prefix_range(prefix.data());
    pair<TrieNode *, bool> findRes = context.find(prevChars, i);

    if (findRes.second) {
      TrieNode &parent = *(findRes.first->parent);
      double escape = 0;
      double total_count = 0;
      double pre_count = 0;
      for (auto pit = parent.children.begin(); pit != parent.children.end();
           pit++) {
        TrieNode *sibling = (pit->second);
        bool usable = !unusable.count(pit->first);
        double value = sibling->value * usable;
        escape += usable;
        total_count += value;
        pre_count += value * (pit->first < c);
        // bool same_size = pit.key().size() == key.size() &&
        //                     !unusable.count(pit.key().back());
        // total_count += pit.value() * same_size;
        // pre_count += pit.value() * same_size * (pit.key() < key);
        // escape += same_size;
      }
      escape *= .5;
      pre_count += escape;
      total_count += escape;
      probabilities.push_back(
          make_tuple(pre_count, pre_count + findRes.first->value, total_count));
      prevChars.pop_back();
      return probabilities;
    } else {
      if (findRes.first->level == prevChars.size() - 1 - i) {
        TrieNode &parent = *(findRes.first);
        double escape = 0;
        double total_count = 0;
        set<char> newUnusable;
        for (auto pit = parent.children.begin(); pit != parent.children.end();
             pit++) {
          TrieNode *sibling = pit->second;
          bool usable = !unusable.count(pit->first);
          escape += usable;
          total_count += sibling->value * usable;
          newUnusable.insert(pit->first);
          // bool same_size = pit.key().size() == key.size() &&
          //                     !unusable.count(pit.key().back());
          // total_count += pit.value() * same_size;
          // pre_count += pit.value() * same_size * (pit.key() < key);
          // escape += same_size;
        }
        if (escape) {
          escape *= .5;
          probabilities.push_back(make_tuple(0, escape, total_count + escape));
        }
        unusable.insert(newUnusable.begin(), newUnusable.end());
      }
      //   if (range.first != context.prefix_cend()) {
      //     double total_count = 0;
      //     double escape = 0;
      //     for (auto pit = range.first; pit != range.second; pit++) {
      //       bool same_size = pit.key().size() == key.size() &&
      //                        !unusable.count(pit.key().back());
      //       total_count += pit.value() * same_size;
      //       escape += same_size;
      //     }
      //     for (auto pit = range.first; pit != range.second; pit++) {

      //       if (pit.key().size() == key.size()) {
      //         unusable.insert(pit.key().back());
      //       }
      //     }
      //     escape *= .5;
      //   }
    }
  }

  set<char>::iterator it = unfound.find(c);
  if (it != unfound.end()) {
    int dist = distance(unfound.begin(), it);
    probabilities.push_back(make_tuple(dist, dist + 1, unfound.size()));
    unfound.erase(c);
  }
  prevChars.pop_back();
  return probabilities;
}

// tuple<double, double, double> Model::getChar(double scaledValue, char &c) {
//   while (contextStartDec <= k) {
//     string key(prevChars.begin() + contextStartDec, prevChars.end());
//     auto range = context.equal_prefix_range(key);
//     if (range.first == context.prefix_cend()) {
//       contextStartDec++;
//       continue;
//     }
//     map<string, double> prefixMap;
//     double escape = 0;
//     set<char> newUnusable;
//     for (auto pit = range.first; pit != range.second; pit++) {
//       if (pit.key().size() == key.size() &&
//       !unusable.count(pit.key().back())) {
//         prefixMap[pit.key()] = pit.value();
//         escape++;
//         newUnusable.insert(pit.key().back());
//       }
//     }
//     escape *= .5;

//     if (escape > scaledValue) {
//       contextStartDec++;
//       unusable.insert(newUnusable.begin(), newUnusable.end());
//       double total = escape;
//       for (auto mit = prefixMap.begin(); mit != prefixMap.end(); mit++) {
//         total += mit->second;
//       }
//       return make_tuple(0, escape, total);
//     } else {
//       scaledValue -= escape;
//       double total = escape;
//       double preCount = escape;
//       double charCount = 0;
//       bool found = false;
//       for (auto mit = prefixMap.begin(); mit != prefixMap.end(); mit++) {
//         if (!found && mit->second > scaledValue) {
//           c = mit->first.back();
//           preCount = total;
//           charCount = mit->second;
//           found = true;
//         }
//         total += mit->second;
//       }
//       contextStartDec = 0;
//       unusable.clear();
//       return make_tuple(preCount, preCount + charCount, total);
//     }
//   }
//   return make_tuple(0, 0, 0);
// }
