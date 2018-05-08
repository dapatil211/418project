#include "trie.h"

TrieNode::TrieNode(double _value, TrieNode &_parent): value(_value), parent(_parent){}

TrieNode::~TrieNode(){
    children.erase(children.begin(), children.end());
}

Trie::Trie(){
    TrieNode root(0, DUMMY);
}

TrieNode &Trie::insert(deque<char> &str){
    TrieNode *curNode = &root;
    for(int i = 0; i < str.size(); i++){
        auto it = curNode->children.find(str[i]);
        if(it == curNode->children.end()){
            TrieNode newNode(.5, *curNode);
            curNode->children[str[i]] = newNode;
            curNode = &newNode;
        } else{
            curNode->children[str[i]].get().value++;
            curNode = &(curNode->children[str[i]].get());
        }
    }
    return *curNode;
}

pair<TrieNode*, bool> find(deque<char> &str, TrieNode& current){
    TrieNode *curNode = &current;
    for(int i = 0; i < str.size(); i++){
        auto it = curNode->children.find(str[i]);
        if(it == curNode->children.end()){
            return make_pair(curNode, false);
        } else{
            curNode = &(curNode->children[str[i]].get());
        }
    }
    return make_pair(curNode, true);
}