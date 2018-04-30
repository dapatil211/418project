#include <fstream>
#include <iostream>

using namespace std;

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
    ifstream fin("enwik8", ios::in);
    compress(fin);
    return 0;
}