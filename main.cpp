#include <vector>
#include <iostream>
#include <functional>
#include <cmath>
#include <fstream>
using namespace std;

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
    for(int i = i0; i < i0+4; i++){ // This is wrong, it should start from i0
        h_avg += h[i]; 
        r_avg += r[i];
    }
    h_avg /= 4;
    r_avg /= 4;
    
    // calculate slope and ordenate on origin
    double slope, b;
    double sum1 = 0;
    double sum2 = 0;
    for(int i = 0; i < 4; i++){
        sum1 += r[i]*h[i];
        sum2 += r[i]*r[i];
    }
    sum1 -= 4*r_avg*h_avg;
    sum2 -= 4*r_avg*r_avg;

    slope = sum1/sum2;
    b = h_avg - slope*r_avg;

    pair<double,double> regression = {b,slope};

    return regression;
}

void Get_hbar_q_g_gbar(vector<double>& y, vector<double>& h_bar, vector<double>& q, vector<double>& g, vector<double>& g_bar){    

    // Get vectors for r and h
    int hi_index = y.size()/2;

    vector<double> r(y.begin(), y.begin() + hi_index);
    vector<double> h(y.begin() + hi_index, y.end());

    // Get linear fit
    pair<double,double> fit = LinearRegression(r,h);

    // Find index for r=0 and insert values for r=0
    int i0 = FindZero(r);

    if(r[i0]!=0){
        r.insert(r.begin()+i0, 0);
        h.insert(h.begin()+i0, fit.first);
    }

    // Empty vectors
    h_bar.clear();
    q.clear();
    g.clear();
    g_bar.clear();
    
    // Calculate first 3 values for h_bar, q, g, g_bar
    for(int i = i0+1; i < i0+4; i++){
        h_bar.push_back(fit.first + fit.second*r[i]/2);
        q.push_back(fit.second*fit.second*r[i]/4);
        g.push_back(1 + M_PI*fit.second*fit.second*r[i]*r[i]/2);
        g_bar.push_back(1 + M_PI*fit.second*fit.second*r[i]*r[i]/4);
    }

    // Calculate quantities h_bar, q, g, g_bar for other r
    for(int i = 4; i < r.size()-i0; i++){
        h_bar.push_back(SimpsonUnequal(r, h, i0, i0+i)/r[i0+i]);
        q.push_back((h[i]-h_bar[i])*(h[i]-h_bar[i])/r[i0+i]);
    }
    for(int i = 4; i < r.size()-i0; i++){
        g.push_back(exp(4*M_PI*SimpsonUnequal(r,q,i0,i0+i)));
    }
    for(int i = 4; i < r.size()-i0; i++){
        g_bar.push_back(SimpsonUnequal(r, g, i0, i0+i)/r[i0+i]);
    }
}

void Get_K(vector<double>& K, vector<double> y, vector<double> g, vector<double> g_bar, vector<double> h_bar,
vector<function<vector<double>(vector<double>,vector<double>,vector<double>,vector<double>,vector<double>)>>& F){
    
    vector<double> aux;
    K.clear();

    int hi_index = y.size()/2;
    vector<double> r_aux(y.begin(), y.begin() + hi_index);
    vector<double> h_aux(y.begin() + hi_index, y.end());

    for(int i = 0; i < F.size(); i++){
        aux = F[i](r_aux, g, g_bar, h_aux, h_bar);
        K.insert(K.begin()+K.size(), aux.begin(), aux.end());
    }
}

void Output(vector<double>& y, vector<double>& h_bar, vector<double>& q, vector<double>& g, vector<double>& g_bar, ofstream& file, double u_current){
    // Write data to file

    int hi_index = y.size()/2;

    vector<double> r_aux(y.begin(), y.begin() + hi_index);
    vector<double> h_aux(y.begin() + hi_index, y.end());

    Get_hbar_q_g_gbar(y, h_bar, q, g, g_bar);
         
    file << "\"Time = " << u_current << endl;
    for(int i = FindZero(r_aux); i < r_aux.size(); i++){
        file << r_aux[i] << " " << h_aux[i] << " " << h_bar[i] << " " << q[i] << " " << g[i]<< " " << g_bar[i] << endl;
    }
    file << endl;
}

void Solver(double step, vector<double>& y, 
vector<function<vector<double>(vector<double>,vector<double>,vector<double>,vector<double>,vector<double>)>>& F,
double u_final, string filename){

    // Initialize variables

    int i0;

    vector<double> K1, K2, K3, K4;

    double u_current = 0;

    vector<double> h_bar, q, g, g_bar;

    // Get vectors for r and h

    int hi_index = F.size();

    vector<double> r_aux(y.begin(), y.begin() + hi_index - 1);
    vector<double> h_aux(y.begin() + hi_index, y.end());

    vector<double> y_aux = y;

    ofstream out_file(filename);

    out_file << "r" << " " << "h" << " h_bar" << " q" << " g" << " g_bar" << endl << endl;
    
    while(u_current < u_final){

        // Output
        Output(y_aux, h_bar, q, g, g_bar, out_file, u_current);

        // Calculate quantities h_bar, q, g, g_bar
        Get_hbar_q_g_gbar(y_aux, h_bar, q, g, g_bar);
    
        // first slope
        Get_K(K1, y_aux, g, g_bar, h_bar, F);

        // Compute new y
        y_aux.clear();
        for(int i = 0; i < K1.size(); i++){
            y_aux.push_back(y[i] + step*K1[i]/2);
        }

        // Calculate quantities h_bar, q, g, g_bar with new h and r
        Get_hbar_q_g_gbar(y_aux, h_bar, q, g, g_bar);

        // second slope
        Get_K(K2, y_aux, g, g_bar, h_bar, F);

        // Compute new r and h
        y_aux.clear();
        for(int i = 0; i < K2.size(); i++){
            y_aux.push_back(y[i] + step*K2[i]/2);
        }

        // Calculate quantities h_bar, q, g, g_bar with new h and r
        Get_hbar_q_g_gbar(y_aux, h_bar, q, g, g_bar);

        // third slope
        Get_K(K3, y_aux, g, g_bar, h_bar, F);

        // Compute new r and h
        y_aux.clear();
        for(int i = 0; i < K3.size(); i++){
            y_aux.push_back(y[i] + step*K3[i]);
        }

        // Calculate quantities h_bar, q, g, g_bar with new h and r
        Get_hbar_q_g_gbar(y_aux, h_bar, q, g, g_bar);

        // fourth slope
        Get_K(K4, y_aux, g, g_bar, h_bar, F);

        // Compute final r and h
        y_aux.clear();
        for(int i = 0; i < K1.size(); i++){
            y_aux.push_back(y[i] + step*(K1[i] + 2*K2[i] + 2*K3[i] + K4[i]));
        }

        // Advance u step

        u_current += step;

    }

    
}

int main(){

    /*vector<double> ydata, xdata;
    double h = 0.01;
    for(int i = -500; i <= 500; i++){
        xdata.push_back(i*h);
        ydata.push_back(exp(-i*h*i*h/2)/sqrt(2*M_PI));
    }

    ofstream out_file("integral.txt");

    out_file << "x" << " " << "integral" << endl;
    for(int i = 0; i < ydata.size(); i++){
        out_file << xdata[i] << " " << SimpsonUnequal(xdata,ydata,0,i) << endl;
    }

    out_file.close();*/

    // Initial conditions (u = 0)

    double p = 0.1;
    double r0 = 0.5;
    double sig = 0.1;

    auto h_init_function = [&](double r){return p*exp(-(r-r0)*(r-r0)/(sig*sig))*(2*r-2*r*r*(r-r0)/(sig*sig));};

    vector<double> y_init;

    double step = 0.005;

    for(int i = 1; i < 300; i++){
        y_init.push_back(i*step);
    }

    for(int i = 1; i < 300; i++){
        y_init.push_back(h_init_function(i*step));
    }

    /*ofstream out_file("integraltest.txt");
    int hi_index = y_init.size()/2;

    vector<double> r_aux(y_init.begin(), y_init.begin() + hi_index);
    vector<double> h_aux(y_init.begin() + hi_index, y_init.end());

    out_file << "x" << " " << "integral" << endl;
    for(int i = 3; i < y_init.size()/2; i++){
        out_file << y_init[i] << " " << SimpsonUnequal(r_aux,h_aux,0,i) << endl;
    }

    out_file.close();*/

    // Evolution functions
    auto hdot = [](double r, double g, double g_bar, double h, double h_bar){
        return (g-g_bar)*(h-h_bar)/(2.*r);
        //return 0;
    };
    auto rdot = [](double r, double g, double g_bar, double h, double h_bar){
        return -g_bar/2.;
        //return -1/2;
    };

    auto hdot_func = [hdot](vector<double> r, vector<double> g, vector<double> g_bar, vector<double> h, vector<double> h_bar){
        vector<double> hdot_vec;
        for(int i = 0; i < r.size(); i++){
            hdot_vec.push_back(hdot(r[i],g[i],g_bar[i],h[i],h_bar[i]));
        }
        return hdot_vec;
    };

    auto rdot_func = [rdot](vector<double> r, vector<double> g, vector<double> g_bar, vector<double> h, vector<double> h_bar){
        vector<double> rdot_vec;
        for(int i = 0; i < r.size(); i++){
            rdot_vec.push_back(rdot(r[i],g[i],g_bar[i],h[i],h_bar[i]));
        }
        return rdot_vec;
    };

    vector<function<vector<double>(vector<double>,vector<double>,vector<double>,vector<double>,vector<double>)>> Funcs = {rdot_func, hdot_func};

    Solver(0.001, y_init, Funcs, 5, "test.dat");
}

