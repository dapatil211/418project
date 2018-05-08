// #include "external/hat-trie/tsl/htrie_map.h"
#include <deque>
#include <string>
#include <tuple>
#include <vector>
#include <set>
#include "trie.h"
using namespace std;

class Model{
    // using ht = tsl::htrie_map<char, double_t>;
    uint k;

    public:
    Model(int k, int totalChars);
    vector<tuple<double, double, double>> getProbability(char c);
    void updateModel(char c);
    static const uint MAX = (1 << 17) - 1;
    static const uint ONE_FOURTH = (1 << 15);
    static const uint ONE_HALF = ONE_FOURTH * 2;
    static const uint THREE_FOURTHS = ONE_FOURTH * 3;


    private:
    // ht context;
    Trie context;
    deque<char> prevChars;
    // string constructEscape(string s);
    set<char> unfound;
    // void addRange(ht::iterator hit, pair<ht::prefix_iterator, ht::prefix_iterator> range,
    //                  vector<tuple<int, int, int>> &probabilities, set<char> &unusable);
    
    // tuple<double, double, double> getChar(double scaledValue, char &c);
    uint contextStartDec;
    // set<char>unusable;
};