#include <fstream>
#include <iostream>
#include "model.h"

using namespace std;
using ht = tsl::htrie_map<char, int32_t>;

void compress(ifstream& fin){
    int count = 0;
    char c;
    while(fin.get(c)){
        count ++;
    }
    fin.close();
    cout << count << endl;
    return;
}

int main(){
    // ifstream fin("enwik8", ios::in);
    // compress(fin);
    // ht context;
    // context.insert("abcd", 1);
    // context.insert("abc", 2);
    // context.insert("abce", 3);
    // context.insert("abcf", 4);
    // context.insert("abcde", 5);
    // auto fst = context.find("abc");
    // auto snd = context.find("abb");
    // for(auto it = fst; it != snd; it++){
    //     cout << it.key() << endl;
    // }
    // auto range = context.equal_prefix_range("abc");
    // for(auto it = range.first; it != range.second; it++){
    //     cout << it.key() << endl;
    // }
    // for(auto it = range.first; it != range.second; it++){
    //     cout << it.key() << endl;
    // }
    // cout << endl;
    // for(auto it = context.begin(); it != context.end(); it++){
    //     cout << it.key() << endl;
    // }
    // cout << (string("acb") < string("abc")) << endl;
    // cout << string("abc").back() << endl;
    Model m = Model(3, 128);
    string compress = string("bananaboat");
    for(auto it = compress.begin(); it != compress.end(); it++){
        auto probs = m.getProbability(*it);
        for(auto vit = probs.begin(); vit !=probs.end(); vit++){

            cout << get<0>(*vit) << " " <<get<1>(*vit) << " " <<get<2>(*vit)  << endl;
        }
        m.updateModel(*it);
    }
    return 0;
}