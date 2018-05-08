#include "trie.h"

TrieNode::TrieNode(double _value, int _level, TrieNode *_parent)
    : value(_value), level(_level), children(map<char, TrieNode *>()), parent(_parent) {}

TrieNode::~TrieNode() { children.erase(children.begin(), children.end()); }

Trie::Trie() : root(TrieNode(0,0,0)){}



TrieNode &Trie::insert(deque<char> &str, uint start) {
  TrieNode *curNode = &root;
  for (uint i = start; i < str.size(); i++) {
    auto it = curNode->children.find(str[i]);
    if (it == curNode->children.end()) {
      TrieNode *newNode = new TrieNode(.5, curNode->level + 1, curNode);
      curNode->children[str[i]] = newNode;
      return *newNode;
    } else {
      curNode = curNode->children[str[i]];
    }
  }
  curNode->value++;
  return *curNode;
}

pair<TrieNode *, bool> Trie::find(deque<char> &str, uint start, TrieNode &current) {
  TrieNode *curNode = &current;
  for (uint i = start; i < str.size(); i++) {
    auto it = curNode->children.find(str[i]);
    if (it == curNode->children.end()) {
      return make_pair(curNode, false);
    } else {
      curNode = curNode->children[str[i]];
    }
  }
  return make_pair(curNode, true);
}

pair<TrieNode *, bool> Trie::find(deque<char> &str, uint start) {
  return find(str, start, root);
}
