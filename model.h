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
    static const uint MAX = 0xFFFFFFFFU;
    static const uint ONE_FOURTH = 0x40000000;
    static const uint ONE_HALF = 0x80000000;
    static const uint THREE_FOURTHS = 0xC0000000;


    private:
    ht context;
    deque<char> prevChars;
    string constructEscape(string s);
    set<char> unfound;
    void addRange(ht::iterator hit, pair<ht::prefix_iterator, ht::prefix_iterator> range,
                     vector<tuple<int, int, int>> &probabilities, set<char> &unusable);
    
    tuple<double, double, double> getChar(double scaledValue, char &c);
    int contextStartDec;
    set<char>unusable;
};