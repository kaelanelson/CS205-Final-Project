/* Kyra Ballard, CS 205 Project, Spring 2020
 sequential degree centrality implementation */

#include <iostream>
#include <fstream>
#include <map>

using namespace std;

map<int,int> degrees;

int main(int argc, char * argv[]){
    
    if(argc != 2){
        cout << "Specify an input file" << endl;
        exit(1);
    }
    
    ifstream file(argv[1], ifstream::binary);
    if(!file.is_open()) {
        cout << "Unable to open file " << argv[1] << endl;
        exit(2);
    }
    
    string n1,n2;
    while(!file.eof()){
        file >> n1;
        file >> n2;
        int id1 = stoi(n1);
        int id2 = stoi(n2);
        if(degrees.count(id1) <= 0)
            degrees[id1] = 0;
        if(degrees.count(id2) <= 0)
            degrees[id2] = 0;
        if(id1 != id2) {
            degrees[id1]++;
            degrees[id2]++;
        }
    }
    file.clear();
    file.seekg(0);
    
    cout << "Key\tDegree Centrality" << endl;
    int max_neighbors = degrees.size() - 1;
    for(map<int,int>::iterator iter = degrees.begin(); iter != degrees.end(); ++iter) {
        int k =  iter->first;
        int v = iter->second;
        double deg = (float)v / max_neighbors;
        cout << k << "\t" << deg << endl;
    }
    
    return 0;
}
