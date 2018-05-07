#include "model.h"
#include <set>
#include <iostream>
#include <map>

Model::Model(int k, int totalChars): k(k){
    for(int i = 0; i < totalChars; i++){
        unfound.insert(i + 'a');
    }
    contextStartDec = 0;
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

tuple<double, double, double> Model::getChar(double scaledValue, char &c){
    while(contextStartDec <= k){
        string key(prevChars.begin() + contextStartDec, prevChars.end());
        auto range = context.equal_prefix_range(key);
        if(range.first == context.prefix_cend()){
            contextStartDec++;
            continue;
        }
        map<string, double> prefixMap;
        double escape = 0;
        set<char> newUnusable;
        for(auto pit = range.first; pit != range.second; pit++){
            if(pit.key().size() == key.size() && !unusable.count(pit.key().back())){
                prefixMap[pit.key()] = pit.value();
                escape ++;
                newUnusable.insert(pit.key().back());
            }
        }
        escape *= .5;

        if(escape > scaledValue){
            contextStartDec ++;
            unusable.insert(newUnusable.begin(), newUnusable.end());
            double total = escape;
            for(auto mit = prefixMap.begin(); mit != prefixMap.end(); mit++){
                total += mit->second;
            }
            return make_tuple(0,escape,total);
        } else{
            scaledValue -= escape;
            double total = escape;
            double preCount = escape;
            double charCount = 0;
            bool found = false;
            for(auto mit = prefixMap.begin(); mit != prefixMap.end(); mit++){
                if(!found && mit->second > scaledValue){
                    c = mit->first.back();
                    preCount = total;
                    charCount = mit->second;
                    found = true;
                }
                total += mit->second;
            }
            contextStartDec = 0;
            unusable.clear();
            return make_tuple(preCount, preCount + charCount, total);
        }
    }
    return make_tuple(0,0,0);
}
