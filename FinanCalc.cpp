#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <exception>

using namespace std;

double NetPresentValue(double rate,vector<double> cashflows) {
    double result = 0.0;
    for (unsigned int i = 0; i < cashflows.size(); ++i) {
        result += cashflows.at(i) / pow(1 + rate, i);
    }
    return result;
}

double InternalRateofReturn(vector<double> cashflows) {
    double guess = 0.0;
    double highguess, lowguess;
    const double PRECISION = 0.000000001;
    const int MAX_TRIES = 50;
    int tries = 0;
    while (NetPresentValue(guess, cashflows) != 0) {
        if (NetPresentValue(guess, cashflows) > 0) {
            lowguess = guess;
            highguess = 1;
            guess = (lowguess + highguess) / 2;
            
            while ((tries < MAX_TRIES) && (NetPresentValue(highguess, cashflows) > 0)) {//shifts guess values up if IRR > 100%
                highguess +=1;
                lowguess +=1;
                guess +=1;
                tries +=1;
            }  
        }
        else if (NetPresentValue(guess, cashflows) < 0) {
            highguess = guess;
            lowguess = -0.4;//not -1 because of division by 0 error, also values around -1 may cause overflow(?)
            guess = (lowguess + highguess) / 2;

            while ((tries < MAX_TRIES) && (NetPresentValue(lowguess, cashflows) < 0)) {//shifts guess values down if IRR < -40%
                highguess -=0.4;
                lowguess -=0.4;
                guess -=0.4;
                tries +=1;
            }  
        }

        while(((tries < MAX_TRIES) && abs(NetPresentValue(guess, cashflows)) > PRECISION)) {
            if (NetPresentValue(guess, cashflows) > 0) {
                lowguess = guess;
                guess = (lowguess + highguess) / 2;
                tries++;
            }
            if (NetPresentValue(guess, cashflows) < 0) {
                highguess = guess;
                guess = (lowguess + highguess) / 2;
                tries++;
            }
        }
        // cout << tries << "tries" << endl;
        // cout << abs(NetPresentValue(guess, cashflows)) << endl;
        if (abs(NetPresentValue(guess, cashflows)) > PRECISION) {
            throw invalid_argument("Unable to find IRR for provided cashflows.");
        }
        return guess;
    }
    return guess;
}

int main() {
    vector<double> test1 = {-35000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000};
    // cout << "R1 = 0. NPV 1: " << fixed << setprecision(8) << NetPresentValue(0, test1) << endl;
    // cout << "R2 = 0.5. NPV 2: " << fixed << setprecision(8) << NetPresentValue(0.5, test1) << endl;
    // cout << "R3 = -0.5. NPV 3: " << fixed << setprecision(8) << NetPresentValue(-0.5, test1) << endl;
    cout << "IRR1: " << fixed << setprecision(8) << InternalRateofReturn(test1) << endl;
    vector<double> test2 = {-30000, 100, 200, 100, 300, 100, 100, 500};
    cout << "IRR2: " << fixed << setprecision(8) << InternalRateofReturn(test2) << endl;
    vector<double> test3 = {-100, 900, 900, 100, 300, 100, 100, 500, 300, 800, 700};
    cout << "IRR3: " << fixed << setprecision(8) << InternalRateofReturn(test3) << endl;
    vector<double> test4 = {-1000, 300, -100};
    cout << "IRR4: " << fixed << setprecision(8) << InternalRateofReturn(test4) << endl;
    //IRR does not exist in IRR4!
    return 0;
}