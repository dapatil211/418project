#include <fstream>
#include <iostream>
#include <vector>
#include "aricompressor.h"
#include "cycletimer.h"


#if OMP
#include <omp.h>
#endif


using namespace std;
// using ht = tsl::htrie_map<char, int32_t>;

void readInput(ifstream& fin, vector<char> &input){
    //int count = 0;
    char c;
    while(fin.get(c)){
        input.push_back(c);
    }
    fin.close();
    //cout << count << endl;
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

    // ifstream fin("test.in", ios::in);
    ifstream fin("enwik8", ios::in);
    //ifstream fin("test.in", ios::in);

    vector<char> input;
    readInput(fin, input);
    fin.close();
    //cout << bitset<17>(Model::ONE_FOURTH) << " " << bitset<17>(Model::ONE_HALF) << " " << bitset<17>(Model::THREE_FOURTHS) << " " << bitset<17>(Model::MAX) << endl;
    cout << "Finished Reading Input" << endl;

    #if OMP
    int total = input.size();
    int numThreads = 15;
    omp_set_num_threads(numThreads);
    double startTime = currentSeconds();
    int tid;
    int chunkSize = (total + numThreads - 1)/numThreads;
    int prefixSum[numThreads];
    int sum = 0;
    cout << "In parallel section" << endl;
    vector<vector<char>> output;
    for(int i = 0; i < numThreads; i++){
        output.push_back(vector<char>());
    }
    #pragma omp parallel for
    for(tid = 0; tid < numThreads; tid++){
        Model m = Model(5, 255);
        int startIndex = tid*chunkSize;
        int endIndex = min((tid+1)*chunkSize, total);

        compress(input, output[tid], startIndex, endIndex, m);

        cout << "Finished compressing tid" << tid << endl;
    }

    for(tid = 0; tid < numThreads; tid++){
        prefixSum[tid] = sum;
        sum += output[tid].size();
    }

    char compressedOut[sum];

    #pragma omp parallel for
    for(tid = 0; tid < numThreads; tid++){
        uint i;
        for(i = 0; i < output[tid].size(); i++){
            compressedOut[prefixSum[tid] + i] = output[tid][i];
        }
    }

    double totalTime = currentSeconds() - startTime;
    cout << "Total Time: " << totalTime << endl;
    ofstream fout("enwikpar.out", ios::out);
    //ofstream fout("test.out", ios::out);
    for(int i = 0; i < numThreads; i++){
        fout <<prefixSum[i]<< " ";
    }


    for(int i = 0; i < sum; i++){
        fout.put(compressedOut[i]);
    }
    cout << "HERE6" << endl;

    fout.close();
    cout << "Finished Writing " << sum << " bytes" << endl;

    #else
    cout << "In sequential section" << endl;
    vector<char> output;
    Model m = Model(5, 255);

    compress(input, output, 0, input.size(), m);


    cout << "Finished compressing " << input.size() << endl;
    // ofstream fout("test.comp", ios::out);
    ofstream fout("enwik8.comp", ios::out);
    for(uint i = 0; i < output.size(); i++){
        fout.put(output[i]);
    }
    fout.close();
    cout << "Finished Writing " << output.size() << " bytes" << endl;

    #endif
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
