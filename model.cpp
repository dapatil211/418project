#include "model.h"
#include <deque>

Model::Model(int k): k(k){}

string Model::constructEscape(string s){
    string esc(s);
    esc.replace(esc.length() - 1, 1, 1, '\x1B');
    return esc;
}

void Model::updateModel(char c){
    prevChars.push_back(c);

    for (deque<char>::iterator it = prevChars.begin(); it != prevChars.end(); it++){
        string key(it, prevChars.end());
        ht::iterator it = context.find(key);
        if(it == context.cend()){
            context.insert(key, .5);
            string esc = constructEscape(key);
            context.insert(esc, .5);
        } else{
            *it = *it + 1;
        }
    }
    if(prevChars.size() > k){
        prevChars.pop_front();
    }
}

double Model::getProbability(char c){
    prevChars.push_back(c);
    for (deque<char>::iterator it = prevChars.begin(); it != prevChars.end(); it++){
        string key(it, prevChars.end());
        ht::iterator it = context.find(key);
        if(it != context.cend()){
            

        }
    }
    return 0.;
}