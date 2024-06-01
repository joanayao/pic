#include <vector>
#include <functional>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

#include <Eigen/Dense>
using namespace Eigen;

int FindZero(vector<double> r){

    int i0;
    for(int i = 0; i < r.size();i++){
        if(r[i]<0) continue;
        else{
            i0 = i;
            break;}
    }
    return i0;
}

pair<double,double> LinearRegression(vector<double>& r, vector<double>& h){
    
    int i0 = FindZero(r);
    // calculate average h and r
    double h_avg = 0;
    double r_avg = 0;
    for(int i = i0; i < r.size(); i++){
        h_avg += h[i]; 
        r_avg += r[i];
    }
    h_avg /= (double)(r.size()-i0);
    r_avg /= (double)(r.size()-i0);
    
    // calculate slope and ordenate on origin
    double slope, b;
    double sum1 = 0;
    double sum2 = 0;
    for(int i = i0; i < r.size(); i++){
        sum1 += (r[i]-r_avg)*(h[i]-h_avg);
        sum2 += (r[i]-r_avg)*(r[i]-r_avg);
    }

    slope = sum1/sum2;
    b = h_avg - slope*r_avg;

    pair<double,double> regression = {b,slope};

    return regression;
}

vector<double> Parabola_fit(vector<double>& r, vector<double>& h){
    int i0 = FindZero(r);

    double r4_sum = 0;
    double r3_sum = 0;
    double r2_sum = 0;
    double r1_sum = 0;
    double r0_sum = 0;
    double r2_h_sum = 0;
    double r_h_sum = 0;
    double h_sum = 0;

    for(int i = i0; i < i0+4 ; i++){
        r4_sum += r[i]*r[i]*r[i]*r[i];
        r3_sum += r[i]*r[i]*r[i];
        r2_sum += r[i]*r[i];
        r1_sum += r[i];
        r0_sum += 1;
        r2_h_sum += r[i]*r[i]*h[i];
        r_h_sum +=r[i]*h[i];
        h_sum += h[i];
    }

    // Solve Ax = b

    Matrix3d A;
    A << r4_sum, r3_sum, r2_sum,
         r3_sum, r2_sum, r1_sum,
         r2_sum, r1_sum, r0_sum;
    
    Vector3d B(r2_h_sum,r_h_sum,h_sum);

    Vector3d params;
    params = A.inverse()*B;

    // PartialPivLU<Matrix3d> lu(A);
    // auto params = lu.solve(B);
    
    vector<double> parabola_params = {params[0], params[1], params[2]};
    
    return parabola_params;
}

int main(int argc, const char * argv[]){

    vector<double> x = {0,0.1,0.2,0.3};
    vector<double> y = {0, 2.999500034998501e-08, 1.199200223961605e-07, 2.695952550516121e-07};



    /*double step = atof(argv[1]);
    for(int i = atoi(argv[2]); i<atoi(argv[3]); i++){
        x.push_back(i*step);
        //y.push_back(0.5+i*step+0.05*sin(i*step*2*M_PI*5/0.4));
        y.push_back(exp(-i*step*i*step/5));
        //y.push_back(i*i*step*step+2*step*i+3);
    }*/


    // pair<double,double> fit = LinearRegression(x,y);

    // ofstream file("LinearReg_NoZero_Exp" + string(1,argv[1][2]) + string(1,argv[1][3]) + ".dat");
    // file << "x " << "y" << endl;
    // for(int i = 0; i<x.size(); i++){
    //     file << x[i] << " " << y[i] << endl;
    // }

    // ofstream file_fit("LinearReg_NoZero_Exp_fit"+ string(1,argv[1][2]) + string(1,argv[1][3]) + ".dat");
    // file_fit << "b " << "slope" << endl;
    // file_fit << fit.first << " " <<fit.second<<endl;

    vector<double> parabola_fit = Parabola_fit(x,y);
    //string filename ="Parabola_exp" + string(1,argv[1][2]) + string(1,argv[1][3]) + ".dat";

    ofstream file("h_parab.dat");
    file << "x " << "y" << endl;
    for(int i = 0; i<x.size(); i++){
        file << x[i] << " " << y[i] << endl;
    }

    ofstream file_fit("h_parab_fit.dat");
    file_fit << "a " << "b " << "c" << endl;
    file_fit << setprecision(16) << parabola_fit[0] << " " << parabola_fit[1] << " " << parabola_fit[2] <<endl;
}
