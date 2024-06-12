#ifndef SIMPSON
#define SIMPSON

#include <vector>
#include <functional>
#include <cmath>
#include <fstream>
using namespace std;

// Simpson Rule
double SimpsonRule(double h, vector<double>& data);
double SimpsonUnequal(vector<double>& r, vector<double>& data, int r0_index, int rf);

//Trapezoidal Rule
double TrapezoidalRule(vector<double>& r, vector<double>& data, int r0_index, int rf);

#endif