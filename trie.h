#include <map>
#include <deque>
#include <functional>

using namespace std;

class TrieNode{
    public:
    double value;
    map<char, reference_wrapper<TrieNode>> children;
    TrieNode &parent;
    TrieNode();
    TrieNode(double _value, TrieNode &_parent);
    ~TrieNode();
};

class Trie{
    public:
    Trie();
    TrieNode root;
    static TrieNode DUMMY;
    TrieNode& find(deque<char> &str);
    pair<TrieNode*, bool> find(deque<char> &str, TrieNode& current);
    TrieNode& insert(deque<char> &str);
};
