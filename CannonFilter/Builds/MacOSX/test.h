//
//  test.h
//  CannonFilter
//
//  Created by Lindley P Cannon Jr on 11/6/16.
//
//

#ifndef test_h
#define test_h

#include <stdio.h>
#include <cmath>

using namespace std;

void calculateCoeff()
{
    

    double fs = 44100; //sampling frequency
    double f0 = 600;
    double dBgain = 0;
    double Q = 0.707; // Q is constant for 2nd order Butterworth//intermediate parameters
    double wo = 2*M_PI*f0/fs;
    double cosW = cos(wo);
    double sinW = sin(wo);
    double alpha = sinW/(2*Q);
    //low pass filter coefficients
    double b0 = 10*sqrt((dBgain/20)*((1 - cosW)/2));
    double b1 = 10*sqrt((dBgain/20)*(1 - cosW));
    double b2 = 10*sqrt((dBgain/20)*((1 - cosW)/2));
    double a0 = (1+alpha);
    double a1 = -2*cosW;
    double a2 = (1-alpha);
    //Normalize so that A0 = 1
    double B0 = b0/a0;
    double B1 = b1/(2*a0);
    double B2 = b2/a0;
    double A1 = a1/(-2*a0);
    double A2 = a2/(-a0);
    double Mx = max(B0, B1, B2);

    double B0new;
    double B1new;
    double B2new;

    if (Mx > 1){
        B0new = B0/Mx;
        B1new = B1/Mx;
        B2new = B2/Mx;
    } else {
    B0new = B0;
    B1new = B1;
    B2new = B2;
    }

    double NB = 16; // number of bits
    double Range = 2*sqrt((NB-1)-1);
    double N0 = floor(B0new*Range);
    double N1 = floor(B1new*Range);
    double N2 = floor(B2new*Range);
    double D1 = floor(A1*Range);
    double D2 = floor(A2*Range);
    
}
#endif /* test_h */
