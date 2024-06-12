//#include "Solver.h"
#include "Simpson.h"
#include <vector>
#include <functional>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

int main(){

    vector<double> ydata, xdata;
    double h = 0.025;
    for(int i = -180; i <= 180; i++){
        xdata.push_back(i*h);
        ydata.push_back(exp(-(i*h)*(i*h)/2)/sqrt(2*M_PI));
    }

    ofstream out_file("trap_h.dat");
    
    out_file << "x" << " " << "integral" << endl;
    for(int i = 0; i < ydata.size(); i++){
        out_file << xdata[i] << " " << setprecision(16) << TrapezoidalRule(xdata,ydata,0,i) << endl;
    }
    out_file.close();

    /*ofstream out_file_odd("integral01odd.txt");

    out_file_odd << "x" << " " << "integral" << endl;
    for(int i = 1; i < ydata.size(); i+=2){
        out_file_odd << xdata[i] << " " << setprecision(16) << SimpsonUnequal(xdata,ydata,0,i) << endl;
    }
    out_file_odd.close();

    ofstream out_file_even("integral01even.txt");

    out_file_even << "x" << " " << "integral" << endl;
    for(int i = 2; i < ydata.size(); i+=2){
        out_file_even << xdata[i] << " " << setprecision(16) << SimpsonUnequal(xdata,ydata,0,i) << endl;
    }
    out_file_even.close();*/

    // double p = 1e-4;
    // double r0 = 10;
    // double sig = 1;

    // auto h_init_function = [&](double r){return r*(p*exp(-(r-r0)*(r-r0)/(sig*sig))*(2*r-2*r*r*(r-r0)/(sig*sig)))+p*r*r*exp(-(r-r0)*(r-r0)/(sig*sig));
    //                                      //return sin(r);
    //                                      };
    
    // vector<double> ydata, xdata;
    // double h = 0.04;
    // for(int i = 1; i <= 375; i++){
    //     xdata.push_back(i*h);
    //     ydata.push_back(h_init_function(i*h));
    // }

    // ofstream out_file("RK_FirstSlice04_hbar.dat");

    // out_file << "r" << " " << "hbar" << endl;
    // for(int i = 1; i < ydata.size(); i++){
    //     out_file << xdata[i] << " " << setprecision(16) << SimpsonUnequal(xdata,ydata,0,i)/xdata[i] << endl;
    // }
    // out_file.close();
}