#include "Solver.h"

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
    //cout<<r[i0]<<endl;
    // calculate average h and r
    double h_avg = 0;
    double r_avg = 0;
    for(int i = i0; i < i0+4; i++){
        h_avg += h[i]; 
        r_avg += r[i];
    }
    h_avg /= 4;
    r_avg /= 4;
    
    // calculate slope and ordenate on origin
    double slope, b;
    double sum1 = 0;
    double sum2 = 0;
    for(int i = i0; i < i0+4; i++){
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

    for(int i = i0; i < i0 + 4; i++){
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
void Get_hbar_q_g_gbar(vector<double>& y, vector<double>& h_bar, vector<double>& q, vector<double>& g, vector<double>& g_bar){    

    // Get vectors for r and h
    int hi_index = y.size()/2;

    vector<double> r(y.begin(), y.begin() + hi_index);
    vector<double> h(y.begin() + hi_index, y.end());

    // Get linear fit
    pair<double,double> fit = LinearRegression(r,h);

    // Get parabola fit
    vector<double> parab_fit = Parabola_fit(r,h);

    // Find index for r=0 and insert values for r=0
    int i0 = FindZero(r);

    /*if(r[i0]!=0){
        r.insert(r.begin()+i0, 0);
        h.insert(h.begin()+i0, fit.first);
    }
    else{
        h[i0] = fit.first;
    }*/

    // Empty vectors
    h_bar.clear();
    q.clear();
    g.clear();
    g_bar.clear();

    for(int i = 0; i < i0; i++){
        h_bar.push_back(0);
        q.push_back(0);
        g.push_back(0);
        g_bar.push_back(0);
    }

    // Calculate first 3 values for h_bar, q, g, g_bar
    // for(int i = i0; i < i0+3; i++){
    //     h_bar.push_back(fit.first + fit.second*r[i]/2.);
    //     q.push_back(fit.second*fit.second*r[i]/4.);
    //     g.push_back(1 + M_PI*fit.second*fit.second*r[i]*r[i]/2.);
    //     g_bar.push_back(1 + M_PI*fit.second*fit.second*r[i]*r[i]/6.);
    // }

    // Calculate first 3 values for h_bar, q, g, g_bar using parabola fit
    for(int i = i0; i < i0+3; i++){
        h_bar.push_back(parab_fit[2] + parab_fit[1]*r[i]/2. + parab_fit[0]*r[i]*r[i]/3.);
        q.push_back(parab_fit[1]*parab_fit[1]*r[i]/4. - 2.*parab_fit[1]*parab_fit[0]*r[i]*r[i]/3.);
        g.push_back(1 + M_PI*parab_fit[1]*parab_fit[1]*r[i]*r[i]/2. - 8*M_PI*parab_fit[1]*parab_fit[0]*r[i]*r[i]*r[i]*r[i]/9.);
        g_bar.push_back(1 + M_PI*parab_fit[1]*parab_fit[1]*r[i]*r[i]/6. - 8*M_PI*parab_fit[1]*parab_fit[0]*r[i]*r[i]*r[i]*r[i]/45.);
    }

    // Calculate quantities h_bar, q, g, g_bar for other r
    for(int i = 3; i < r.size()-i0; i++){
        h_bar.push_back(SimpsonUnequal(r, h, i0, i0+i)/r[i0+i]);
        q.push_back(0);
        //q.push_back((h[i]-h_bar[i])*(h[i]-h_bar[i])/r[i0+i]);
    }
    for(int i = 3; i < r.size()-i0; i++){
        g.push_back(exp(4*M_PI*SimpsonUnequal(r,q,i0,i0+i)));
    }
    for(int i = 3; i < r.size()-i0; i++){
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

    /*int i0 = FindZero(r_aux);
    vector<double> r_aux0(y.begin() + i0, y.begin() + hi_index);
    vector<double> h_aux0(y.begin() + hi_index + i0, y.end());*/


    for(int i = 0; i < F.size(); i++){
        aux = F[i](r_aux, g, g_bar, h_aux, h_bar);
        K.insert(K.end(), aux.begin(), aux.end());
    }
}

void Output(vector<double>& y, vector<double>& h_bar, vector<double>& q, vector<double>& g, vector<double>& g_bar, vector<double>& y_init, ofstream& file, double u_current){
    // Write data to file

    int hi_index = y.size()/2;

    vector<double> r_aux(y.begin(), y.begin() + hi_index);
    vector<double> h_aux(y.begin() + hi_index, y.end());

    // Get linear fit
    pair<double,double> fit = LinearRegression(r_aux,h_aux);
    int i0 = FindZero(r_aux);

    // Get parabola fit
    vector<double> parab_fit = Parabola_fit(r_aux,h_aux);

    Get_hbar_q_g_gbar(y, h_bar, q, g, g_bar);
         
    file << "\"Time = " << u_current << endl;
    for(int i = 0; i < r_aux.size(); i++){
        file << setprecision(16) << r_aux[i] << " " 
        << setprecision(16) << h_aux[i] << " " 
        << setprecision(16) << h_bar[i] << " " 
        << setprecision(16) << q[i] << " " 
        << setprecision(16) << g[i]<< " " 
        << setprecision(16) << g_bar[i] << " "
        << setprecision(16) << 2*y_init[i] << " ";

        if(i < i0 + 4 && i > i0-1){
            //file << setprecision(16) << fit.first + fit.second*r_aux[i] << endl;
            file << setprecision(16) << parab_fit[2] + parab_fit[1]*r_aux[i] + parab_fit[0]*r_aux[i]*r_aux[i] << endl;
        }
        else{
            file << setprecision(16) << h_aux[i] << " " << endl;
        }
    }
    file << endl;
}

void Solver(double step, vector<double>& y, 
vector<function<vector<double>(vector<double>,vector<double>,vector<double>,vector<double>,vector<double>)>>& F,
double u_final, string filename){

    // Initialize variables

    vector<double> K1, K2, K3, K4;

    int iteration = 0;
    double u_current = 0;

    vector<double> h_bar, q, g, g_bar;

    // Get vectors for r and h

    int hi_index = F.size();

    vector<double> y_aux = y;
    vector<double> y_aux2;

    ofstream out_file(filename);

    out_file << "r" << " " << "h" << " h_bar" << " q" << " g" << " g_bar" << " v" << " h_fit" << endl << endl;
    
    while(u_current <= u_final){

        y_aux2 = y_aux;

        // Output
        Output(y_aux, h_bar, q, g, g_bar, y, out_file, u_current);

        // Calculate quantities h_bar, q, g, g_bar
        //Get_hbar_q_g_gbar(y_aux, h_bar, q, g, g_bar);
    
        // first slope
        Get_K(K1, y_aux, g, g_bar, h_bar, F);

        // Compute new y
        for(int i = 0; i < K1.size(); i++){
            y_aux[i] = y_aux2[i] + step*K1[i]/2.;
        }

        // Calculate quantities h_bar, q, g, g_bar with new h and r
        Get_hbar_q_g_gbar(y_aux, h_bar, q, g, g_bar);

        // second slope
        Get_K(K2, y_aux, g, g_bar, h_bar, F);

        // Compute new r and h
        for(int i = 0; i < K2.size(); i++){
            y_aux[i] = y_aux2[i] + step*K2[i]/2.;
        }

        // Calculate quantities h_bar, q, g, g_bar with new h and r
        Get_hbar_q_g_gbar(y_aux, h_bar, q, g, g_bar);

        // third slope
        Get_K(K3, y_aux, g, g_bar, h_bar, F);

        // Compute new r and h
        for(int i = 0; i < K3.size(); i++){
            y_aux[i] = y_aux2[i] + step*K3[i];
        }

        // Calculate quantities h_bar, q, g, g_bar with new h and r
        Get_hbar_q_g_gbar(y_aux, h_bar, q, g, g_bar);

        // fourth slope
        Get_K(K4, y_aux, g, g_bar, h_bar, F);

        // Compute final r and h
        for(int i = 0; i < K1.size(); i++){
            y_aux[i] = y_aux2[i] + step*(K1[i] + 2.*K2[i] + 2.*K3[i] + K4[i])/6.;
        }

        // Advance u step
        iteration += 1;
        u_current = iteration*step;
    }

    
}
