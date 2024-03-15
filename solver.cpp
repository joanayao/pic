#include <vector>
#include <iostream>
#include <functional>
using namespace std;

struct Point{
    double u;
    double v;
    double r;
    double hbar;
    double q;
    double g;
    double gbar;
};

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

double SimpsonUnequal(vector<double>& r, vector<double>& data, int r0_indice, int rf){ 

    int n = rf-r0_indice; // number of slices from r=0

    double integral = 0;
    double h1, h2; // auxiliar var to calculate intervals

    if(n%2 == 0){ // if number of slices is even
        for (int i = 0; i < n/2-1; i++){
            
            h1 = r[r0_indice+2*i+1] - r[r0_indice+2*i];
            h2 = r[r0_indice+2*i+2] - r[r0_indice+2*i+1];

            integral += (h1+h2)*((2-h2/h1)*data[r0_indice+2*i]+(h1+h2)*(h1+h2)*data[r0_indice+2*i+1]/(h2*h1)+(2-h1/h2)*data[r0_indice+2*i+2]);
        }
        integral/=6.;
    }
    else{ // if number of slices is odd
        for (int i = 0; i < (n-1)/2; i++){
            
            h1 = r[r0_indice+2*i+1] - r[r0_indice+2*i];
            h2 = r[r0_indice+2*i+2] - r[r0_indice+2*i+1];

            integral += (h1+h2)*((2-h2/h1)*data[r0_indice+2*i]+(h1+h2)*(h1+h2)*data[r0_indice+2*i+1]/(h2*h1)+(2-h1/h2)*data[r0_indice+2*i+2]);
        }

        h1 = r[r0_indice+n-1]-r[r0_indice+n-2];
        h2 = r[r0_indice+n]-r[r0_indice+n-1];

        integral += (2*h2*h2+3*h2*h1)*data[r0_indice+n]/(h1+h2)
                    + (h2*h2+3*h1*h2)*data[r0_indice+n-1]/h1
                    - h2*h2*h2*data[r0_indice+n-2]/(h1*(h1+h2)); 

        integral/=6.;}
    
    return integral;
}

void Solver(double step, vector<double>& h, vector<double>& r, 
function<double(double,vector<double>)>& F, double uf, vector<vector<double>>& Data){

    // Calculate quantities h_bar, q, g, g_bar

    vector<double> h_bar, q, g, g_bar;

    for(int i = 0; i < r.size(); i++){
        h_bar.push_back(SimpsonUnequal(r, h, 0, i));
        q.push_back((h[i]-h_bar[i])*(h[i]-h_bar[i])/r[i]);
        g.push_back();
    }
    

    double K1, K2, K3, K4;
    double u_aux = ui;

    Data.clear();

    Data.push_back(InitData);

    for (int i = 0; i < InitData.size(); i++){
        vector<double> iData;
        Data.push_back(iData);
    }

    double y;

    while(u_aux < uf){

        for (int i = 0; i < Data.size(); i++)
        {
            for (int j = 0; j < Data.size(); j++){
                K1 = h*F(u_aux, Data[i]);
                K2 = h*F(u_aux+h/2., Data[i]+K1/2.);
                K3 = h*F(u_aux+h/2., Data[i]+K2/2.);
                K4 = h*F(u_aux+h, Data[i]+K3);

                y = InitData[i] + (K1 + 2*K2 + 2*K3 + K4);
                Data[i].push_back(y);
            }
        }

        taux += h;
    }

}

int main(){

    /*vector<double> data;
    double h = 0.01;
    for(int i = 0; i <= 10000; i++){
        data.push_back(i*i*h*h);
    }

    double integral = SimpsonRule(h,data);

    cout << integral << endl;


    vector<double> r = {0,0.00067,0.0098,0.01,0.023,0.03,0.05,0.06,0.09,0.1,0.1056,0.11,0.12,0.14,0.17,0.18,0.23,0.25,0.26,0.27,0.30,0.32,0.36,0.39,0.40,0.45,0.47,0.5,0.51,0.52,0.55,0.56,0.59,0.6,0.62,0.67,0.68,0.69,0.7,0.72,0.736,0.746,0.761,0.7834,0.792,0.812,0.841,0.8678,0.899,0.923,0.924,0.9561,0.9742,0.981,0.9876,0.99,1};
    vector<double> data2 = {0,0.00067,0.0098,0.01,0.023,0.03,0.05,0.06,0.09,0.1,0.1056,0.11,0.12,0.14,0.17,0.18,0.23,0.25,0.26,0.27,0.30,0.32,0.36,0.39,0.40,0.45,0.47,0.5,0.51,0.52,0.55,0.56,0.59,0.6,0.62,0.67,0.68,0.69,0.7,0.72,0.736,0.746,0.761,0.7834,0.792,0.812,0.841,0.8678,0.899,0.923,0.924,0.9561,0.9742,0.981,0.9876,0.99,1};

    cout << SimpsonUnequal(r, data2, r.size()) << endl;*/

    auto h_dot_func = [](vector<double> r, vector<double> g, vector<double> g_bar, vector<double> h, vector<double> h_bar){
        vector<double> h_dot;
        for(int i = 0; i < r.size(); i++){
            h_dot.push_back((g[i]-g_bar[i])*(h[i]-h_bar[i])/(2.*r[i]));
        }
        return h_dot;
    }

    auto r_dot_func = [](vector<double> r, vector<double> g, vector<double> g_bar, vector<double> h, vector<double> h_bar){
        vector<double> r_dot;
        for(int i = 0; i < r.size(); i++){
            r_dot.push_back(-g_bar[i]/2.);
        }
        return r_dot;
    }
}

