#include <map>
#include <deque>
#include <functional>

using namespace std;

class TrieNode{
    public:
    double value;
    uint level;
    map<unsigned char, TrieNode*> children;
    TrieNode *parent;
    // TrieNode();
    TrieNode(double _value, int _level, TrieNode *_parent);
    ~TrieNode();
};

class Trie{
    public:
    Trie();
    TrieNode root;
    // static TrieNode DUMMY;
    pair<TrieNode*, bool> find(deque<unsigned char> &str, uint start, TrieNode& current);
    pair<TrieNode*, bool> find(deque<unsigned char> &str, uint start);
    TrieNode& insert(deque<unsigned char> &str, uint start);
};
