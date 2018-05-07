#include "external/hat-trie/tsl/htrie_map.h"
#include <deque>
#include <string>
#include <tuple>
#include <vector>
#include <set>
using namespace std;

class Model{
    using ht = tsl::htrie_map<char, double_t>;
    uint k;

    public:
    Model(int k, int totalChars);
    vector<tuple<double, double, double>> getProbability(char c);
    void updateModel(char c);
    private:
    ht context;
    deque<char> prevChars;
    string constructEscape(string s);
    set<char> unfound;
    void addRange(ht::iterator hit, pair<ht::prefix_iterator, ht::prefix_iterator> range,
                     vector<tuple<int, int, int>> &probabilities, set<char> &unusable);
    
};