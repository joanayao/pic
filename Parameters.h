#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include<tuple>
using namespace std;

tuple<double, double, string, string> ReadParameters(string filename);
void InitDataReader(vector<double>& InitData, string filename);