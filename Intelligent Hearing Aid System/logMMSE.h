#ifndef MFCC_H
#define MFCC_H
#define _USE_MATH_DEFINES

#define MAXIT 100 //Maximum allowed number of iterations.
#define EULER 0.5772156649 //Euler's constant
#define FPMIN 1.0e-30 //Close to smallest representable floating-point number.
#define EPS 1.0e-7 //Desired relative error, not smaller than the machine precision.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void initLogMMSE(int inFs,int innFFT);
void initNoiseD2(float *fftmag);
void setNoiseD2(int N);
float* Supress(float *Insig);
void SetSafetyNetPmin(int ind);
//void freqsmooth();
void InitGD2(int classdec);
float expint(int n, float in);
void destroylogMMSE();

#endif
