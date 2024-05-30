#include "Parameters.h"

tuple<double, double, string, string> ReadParameters(string filename){

    // read text file with fstream

    fstream F(filename, ios::in); // input file

    // read first 2 lines

    string s;
    for (int i=0; i<2; i++) {
        getline(F,s);
        cout << s << endl;
    }

    // read data

    string line;

    double u_step, u_final;
    string InitData, OutputFile;
    

    while (getline(F,line)) { // true is returned if values are read
        stringstream linha(line); // split line into words(separated by a space)
        string word;
        linha >> word;

        if(word == "u_step:"){
            linha >> word;
            u_step = stod(word);
        }
        else if(word == "u_final:"){
            linha >> word;
            u_final = stod(word);
        }
        else if(word == "InitData:"){
            linha >> word;
            InitData = word;
        }
        else if(word == "OutputFile:"){
            linha >> word;
            OutputFile = word;
        }
    }
    F.close();

    tuple<double, double, string, string> Parameters(u_step,u_final,InitData,OutputFile);

    return Parameters;
}

void InitDataReader(vector<double>& InitData, string filename){

    InitData.clear();

    // read text file with fstream

    fstream F(filename, ios::in); // input file
    
    // read data

    string line;

    while (getline(F,line)) { // true is returned if values are read
        InitData.push_back(stod(line)); // change word to double and input in Data
    }
    F.close();
}