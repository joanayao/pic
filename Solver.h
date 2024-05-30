#ifndef SOLVER
#define SOLVER 

#include <vector>
#include <functional>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "Simpson.h"

using namespace std;

#include <Eigen/Dense>
using namespace Eigen;

// Find first value great or equal to zero in a vector
int FindZero(vector<double> r);

// Linear regression
pair<double,double> LinearRegression(vector<double>& r, vector<double>& h);

// Calculate quantities h_bar, q, g, g_bar
void Get_hbar_q_g_gbar(vector<double>& y, vector<double>& h_bar, vector<double>& q, vector<double>& g, vector<double>& g_bar);

// Calculate K in runge kutta
void Get_K(vector<double>& K, vector<double> y, vector<double> g, vector<double> g_bar, vector<double> h_bar,
vector<function<vector<double>(vector<double>,vector<double>,vector<double>,vector<double>,vector<double>)>>& F);

// Output file for each step in runge kutta
void Output(vector<double>& y, vector<double>& h_bar, vector<double>& q, vector<double>& g, vector<double>& g_bar, ofstream& file, double u_current);

// Runge kutta solver
void Solver(double step, vector<double>& y, 
vector<function<vector<double>(vector<double>,vector<double>,vector<double>,vector<double>,vector<double>)>>& F,
double u_final, string filename);

#endif