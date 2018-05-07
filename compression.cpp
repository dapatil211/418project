#include <fstream>
#include <iostream>
#include <vector>
#include "aricompressor.h"
using namespace std;
using ht = tsl::htrie_map<char, int32_t>;

void readInput(ifstream& fin, vector<char> &input){
    int count = 0;
    char c;
    while(fin.get(c)){
        input.push_back(c);
    }
    fin.close();
    cout << count << endl;
    return;
}

int main(){
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
    ifstream fin("enwik8", ios::in);
    vector<char> input;
    vector<char> output;
    readInput(fin, input);
    fin.close();
    Model m = Model(5, 255);
    compress(input, output, 0, input.size(), m);
    cout << "Finished compressing " << input.size() << endl;
    ofstream fout("enwik8.comp", ios::out);
    for(int i = 0; i < output.size(); i++){
        fout.put(output[i]);
    }
    fout.close();
    cout << "Finished Writing " << output.size() << " bytes" << endl;
    // string compress = string("bananaboat");
    // for(auto it = compress.begin(); it != compress.end(); it++){
    //     auto probs = m.getProbability(*it);
    //     for(auto vit = probs.begin(); vit !=probs.end(); vit++){

    //         cout << get<0>(*vit) << " " <<get<1>(*vit) << " " <<get<2>(*vit)  << endl;
    //     }
    //     m.updateModel(*it);
    // }
    return 0;
}