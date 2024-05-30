#include "Simpson.h"

double SimpsonRule(double h, vector<double>& data){
    
    int n = data.size()-1; // number of slices

    double integral = 0;

    if(n%2 == 0){ // if number of slices is even
        for (int i = 1; i < n; i+=2){
            integral += data[i-1]+ 4.*data[i]+ data[i+1];
        }
        integral = integral*h/3.;
    }
    else{ // if number of slices is odd
        for (int i = 1; i < n-1; i+=2){
            integral += data[i-1]+ 4.*data[i]+ data[i+1];
        }
        integral = integral*h/3. + (-data[n-2] + 8.*data[n-1] + 5.*data[n])*h/12.;
    }
    
    return integral;
}

double SimpsonUnequal(vector<double>& r, vector<double>& data, int r0_index, int rf){ 

    int n = rf-r0_index; // number of slices from r=0

    double integral = 0;
    double h1, h2; // auxiliar var to calculate intervals

    /*if(n%2 == 0){ // if number of slices is even
        for (int i = 0; i < n/2-1; i++){
            
            h1 = r[r0_index+2*i+1] - r[r0_index+2*i];
            h2 = r[r0_index+2*i+2] - r[r0_index+2*i+1];

            integral += (h1+h2)*((2-h2/h1)*data[r0_index+2*i]+(h1+h2)*(h1+h2)*data[r0_index+2*i+1]/(h2*h1)+(2-h1/h2)*data[r0_index+2*i+2]);
        }
        integral/=6.;
    }
    else{ // if number of slices is odd
        for (int i = 0; i < (n-1)/2; i++){
            
            h1 = r[r0_index+2*i+1] - r[r0_index+2*i];
            h2 = r[r0_index+2*i+2] - r[r0_index+2*i+1];

            integral += (h1+h2)*((2-h2/h1)*data[r0_index+2*i]+(h1+h2)*(h1+h2)*data[r0_index+2*i+1]/(h2*h1)+(2-h1/h2)*data[r0_index+2*i+2]);
        }

        h1 = r[r0_index+n-1]-r[r0_index+n-2];
        h2 = r[r0_index+n]-r[r0_index+n-1];

        integral += (2*h2*h2+3*h2*h1)*data[r0_index+n]/(h1+h2)
                    + (h2*h2+3*h1*h2)*data[r0_index+n-1]/h1
                    - h2*h2*h2*data[r0_index+n-2]/(h1*(h1+h2)); 

        integral/=6.;}*/

    if(n%2 != 0){
        h1 = r[r0_index+n-1]-r[r0_index+n-2];
        h2 = r[r0_index+n]-r[r0_index+n-1];

        integral += (2*h2*h2+3*h2*h1)*data[r0_index+n]/(h1+h2)
                    + (h2*h2+3*h1*h2)*data[r0_index+n-1]/h1
                    - h2*h2*h2*data[r0_index+n-2]/(h1*(h1+h2));
        
        n = n-1;
    }

    for (int i = 0; i < n/2; i++){

        h1 = r[r0_index+2*i+1] - r[r0_index+2*i];
        h2 = r[r0_index+2*i+2] - r[r0_index+2*i+1];

        integral += (h1+h2)*((2-h2/h1)*data[r0_index+2*i]+(h1+h2)*(h1+h2)*data[r0_index+2*i+1]/(h2*h1)+(2-h1/h2)*data[r0_index+2*i+2]);
    }

    integral/=6.;

    return integral;
}