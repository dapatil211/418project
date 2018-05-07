#include "model.h"
#include <set>
#include <iostream>

Model::Model(int k, int totalChars): k(k){
    for(int i = 0; i < totalChars; i++){
        unfound.insert(i + 'a');
    }
    cout << unfound.size() << endl;
}

string Model::constructEscape(string s){
    string esc(s);
    esc.replace(esc.length() - 1, 1, 1, '\xFF');
    return esc;
}

void Model::updateModel(char c){
    prevChars.push_back(c);

    for (deque<char>::iterator it = prevChars.begin(); it != prevChars.end(); it++){
        string key(it, prevChars.end());
        ht::iterator hit = context.find(key.data());
        if(hit == context.end()){
            context.insert(key.data(), .5);
            // string esc = constructEscape(key);
            // ht::iterator esc_it = context.find(esc.data());
            // if(esc_it == context.end()){
            //     context.insert(esc.data(), .5);
            // } else{
            //     *esc_it = *esc_it + .5;
            // }
        } else{
            *hit = *hit + 1;
        }
    }
    if(prevChars.size() > k){
        prevChars.pop_front();
    }
}

void Model::addRange(ht::iterator hit, pair<ht::prefix_iterator, ht::prefix_iterator> range,
                     vector<tuple<int, int, int>> &probabilities, set<char> &unusable){
    
}

vector<tuple<double, double, double>> Model::getProbability(char c){
    vector<tuple<double,double,double>> probabilities;
    prevChars.push_back(c);
    set<char> unusable;
    for (deque<char>::iterator it = prevChars.begin(); it != prevChars.end(); it++){
        string prefix(it, prevChars.end() - 1);
        string key(it, prevChars.end());
        ht::iterator hit = context.find(key);
        auto range = context.equal_prefix_range(prefix.data());
        if(hit == context.end()){
            if(range.first != context.prefix_cend()){
                double total_count = 0;
                double escape = 0;
                for(auto pit = range.first; pit != range.second; pit++){
                    bool same_size = pit.key().size() == key.size() && 
                                        !unusable.count(pit.key().back());
                    total_count += pit.value() * same_size;
                    escape += same_size;
                }
                for(auto pit = range.first; pit != range.second; pit++){
                    
                    if(pit.key().size() == key.size()){
                        unusable.insert(pit.key().back());
                    }
                }
                escape *= .5;
                probabilities.push_back(make_tuple(0, escape, total_count + escape));
            }
        } else {
            double total_count = 0;
            double escape = 0;
            double pre_count = 0;
            for(auto pit = range.first; pit != range.second; pit++){
                bool same_size = pit.key().size() == key.size() && 
                                    !unusable.count(pit.key().back());
                total_count += pit.value() * same_size;
                pre_count += pit.value() * same_size * (pit.key() < key);
                escape += same_size;
            }
            escape *= .5;
            pre_count += escape;
            probabilities.push_back(make_tuple(pre_count, pre_count + *hit, total_count + escape));
            prevChars.pop_back();
            return probabilities;
        }
    }

    set<char>::iterator it = unfound.find(c);
    if(it != unfound.end()){
        int dist = distance(unfound.begin(), it);
        probabilities.push_back(make_tuple(dist, dist + 1, unfound.size()));
        unfound.erase(c);
    }
    prevChars.pop_back();
    return probabilities;
}
