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

vector<double> Parabola_fit(vector<double>& r, vector<double>& h){
    int i0 = FindZero(r);

    double r4_sum = 0;
    double r3_sum = 0;
    double r2_sum = 0;
    double r_sum = 0;
    double r2_h_sum = 0;
    double r_h_sum;
    double h_sum;

    for(int i = i0; i < i0+4 ; i++){
        r4_sum += r[i]*r[i]*r[i]*r[i];
        r3_sum += r[i]*r[i]*r[i];
        r2_sum += r[i]*r[i];
        r_sum += r[i];
        r2_h_sum += r[i]*r[i]*h[i];
        r_h_sum +=r[i]*h[i];
        h_sum += h[i];
    }

    Matrix3d A;
    A << r4_sum, r3_sum, r2_sum,
         r3_sum, r2_sum, r_sum,
         r2_sum, r_sum, 1;
    
    Vector3d B(r2_h_sum,r_h_sum,h_sum);

    Vector3d params;
    params = A.inverse()*B;
    
    vector<double> parabola_params = {params[0], params[1], params[2]};
    
    return parabola_params;
}