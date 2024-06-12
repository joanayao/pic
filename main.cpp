#include "Solver.h"
#include "Parameters.h"

using namespace std;

int main(int argc, const char * argv[]){

    if(argc != 2){
        cout<<"Error: Input parameters filename.";
        return 0;
    }

    tuple<double, double, string, string> parameters = ReadParameters(argv[1]);
    
    cout<< get<0>(parameters)<<endl;
    cout<< get<1>(parameters)<<endl;
    cout<< get<2>(parameters)<<endl;
    cout<< get<3>(parameters)<<endl;

    vector<double> y_init;
    InitDataReader(y_init, get<2>(parameters));
    
    // Initial conditions (u = 0)

    /*double p = 1e-4;
    double r0 = 10;
    double sig = 1;

    auto h_init_function = [&](double r){return r*(p*exp(-(r-r0)*(r-r0)/(sig*sig))*(2*r-2*r*r*(r-r0)/(sig*sig)))+p*r*r*exp(-(r-r0)*(r-r0)/(sig*sig));
                                         //return sin(r);
                                         };

    vector<double> y_init;

    double step = 0.04;
    for(int X = 1; i < 376; i++){
        y_init.push_back(i*step);
        //y_init.push_back(h_init_function(i*step));
    }

    for(int i = 1; i < 376; i++){
        y_init.push_back(h_init_function(i*step));
        //y_init.push_back(0);
    }*/

    // Evolution functions
    auto hdot = [](double r, double g, double g_bar, double h, double h_bar){
        return (g-g_bar)*(h-h_bar)/(2.*r);
        //return 0;
        //return -cos(r)/2.;
        //return h;
    };
    auto rdot = [](double r, double g, double g_bar, double h, double h_bar){
        return -g_bar/2.;
        //return -1./2.;
        //return r;
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

    Solver(get<0>(parameters), y_init, Funcs, get<1>(parameters), get<3>(parameters));
}

