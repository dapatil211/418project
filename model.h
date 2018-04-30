#include "external/hat-trie/tsl/htrie_map.h"
#include <deque>
#include <string>

using namespace std;

class Model{
    using ht = tsl::htrie_map<char, int32_t>;
    int k;

    public:
    Model(int k);
    double getProbability(char c);
    void updateModel(char c);
    private:
    ht context;
    deque<char> prevChars;
    string constructEscape(string s);
    int unfound = 128;
};