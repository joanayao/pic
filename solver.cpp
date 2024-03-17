#include <vector>
#include <iostream>
#include <functional>
#include <cmath>
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

void Solver(double step, vector<double>& h, vector<double>& r, 
function<vector<double>(vector<double>,vector<double>,vector<double>,vector<double>,vector<double>)>& hdot_func,
function<vector<double>(vector<double>)>& rdot_func, 
double uf){

    // Initialize variables

    int i0;
    double k1, k2, k3, k4, l1, l2, l3, l4;
    vector<double> K1, K2, K3, K4, L1, L2, L3, L4;
    double u_aux = 0;

    vector<double> h_bar, q, g, g_bar;

    vector<double> r_aux = r;
    vector<double> h_aux = h;

    while(u_aux < uf){

        // Find indice for r=0

        i0 = FindZero(r_aux);

        // Calculate quantities h_bar, q, g, g_bar

        for(int i = 0; i < r_aux.size(); i++){
            h_bar.push_back(SimpsonUnequal(r_aux, h_aux, i0, i));
            q.push_back((h_aux[i]-h_bar[i])*(h_aux[i]-h_bar[i])/r_aux[i]);
        }
        for(int i = 0; i < r_aux.size(); i++){
            g.push_back(exp(4*M_PI*r_aux[i]*SimpsonUnequal(r_aux,q,i0,i)));
        }
        for(int i = 0; i < r_aux.size(); i++){
            g_bar.push_back(SimpsonUnequal(r_aux, g, i0, i));
        }

        // first slope

        K1 = hdot_func(r_aux, g, g_bar, h_aux, h_bar);
        L1 = rdot_func(g_bar);

        // Compute new r and h

        r_aux.clear();
        h_aux.clear();
        for(int i = 0; i < K1.size(); i++){
            r_aux.push_back(r[i] + step*K1[i]/2);
            h_aux.push_back(h[i] + step*L1[i]/2);
        }

        // Calculate quantities h_bar, q, g, g_bar with new h and r

        h_bar.clear();
        q.clear();
        g.clear();
        g_bar.clear();

        i0 = FindZero(r_aux);
        
        for(int i = 0; i < r_aux.size(); i++){
            h_bar.push_back(SimpsonUnequal(r_aux, h_aux, i0, i));
            q.push_back((h[i]-h_bar[i])*(h[i]-h_bar[i])/r_aux[i]);
        }

        for(int i = 0; i < r_aux.size(); i++){
            g.push_back(exp(4*M_PI*r_aux[i]*SimpsonUnequal(r_aux,q,i0,i)));
        }
        for(int i = 0; i < r_aux.size(); i++){
            g_bar.push_back(SimpsonUnequal(r_aux, g, i0, i));
        }

        // second slope

        K2 = hdot_func(r_aux, g, g_bar, h_aux, h_bar);
        L2 = rdot_func(g_bar);

        // Compute new r and h

        r_aux.clear();
        h_aux.clear();
        
        for(int i = 0; i < K1.size(); i++){
            r_aux.push_back(r[i] + step*K2[i]/2);
            h_aux.push_back(h[i] + step*L2[i]/2);
        }

        // Calculate quantities h_bar, q, g, g_bar with new h and r

        h_bar.clear();
        q.clear();
        g.clear();
        g_bar.clear();

        i0 = FindZero(r_aux);
        
        for(int i = 0; i < r_aux.size(); i++){
            h_bar.push_back(SimpsonUnequal(r_aux, h_aux, i0, i));
            q.push_back((h[i]-h_bar[i])*(h[i]-h_bar[i])/r_aux[i]);
        }

        for(int i = 0; i < r_aux.size(); i++){
            g.push_back(exp(4*M_PI*r_aux[i]*SimpsonUnequal(r_aux,q,i0,i)));
        }
        for(int i = 0; i < r_aux.size(); i++){
            g_bar.push_back(SimpsonUnequal(r_aux, g, i0, i));
        }

        // third slope

        K3 = hdot_func(r_aux, g, g_bar, h_aux, h_bar);
        L3 = rdot_func(g_bar);

        // Compute new r and h

        r_aux.clear();
        h_aux.clear();
        
        for(int i = 0; i < K1.size(); i++){
            r_aux.push_back(r[i] + step*K3[i]);
            h_aux.push_back(h[i] + step*L3[i]);
        }

        // Calculate quantities h_bar, q, g, g_bar with new h and r

        h_bar.clear();
        q.clear();
        g.clear();
        g_bar.clear();

        i0 = FindZero(r_aux);
        
        for(int i = 0; i < r_aux.size(); i++){
            h_bar.push_back(SimpsonUnequal(r_aux, h_aux, i0, i));
            q.push_back((h[i]-h_bar[i])*(h[i]-h_bar[i])/r_aux[i]);
        }

        for(int i = 0; i < r_aux.size(); i++){
            g.push_back(exp(4*M_PI*r_aux[i]*SimpsonUnequal(r_aux,q,i0,i)));
        }
        for(int i = 0; i < r_aux.size(); i++){
            g_bar.push_back(SimpsonUnequal(r_aux, g, i0, i));
        }

        // fourth slope

        K4 = hdot_func(r_aux, g, g_bar, h_aux, h_bar);
        L4 = rdot_func(g_bar);

        // store points


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

    auto hdot_func = [](vector<double> r, vector<double> g, vector<double> g_bar, vector<double> h, vector<double> h_bar){
        vector<double> hdot;
        for(int i = 0; i < r.size(); i++){
            hdot.push_back((g[i]-g_bar[i])*(h[i]-h_bar[i])/(2.*r[i]));
        }
        return hdot;
    };

    auto rdot_func = [](vector<double> g_bar){
        vector<double> rdot;
        for(int i = 0; i < r.size(); i++){
            rdot.push_back(-g_bar[i]/2.);
        }
        return rdot;
    };
}

