#ifndef MFCC_H
#define MFCC_H
#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void getMFCCValues(float* inputBuffer, int stepsize, int windowsize, int F);
void initializeMFCC();
void hanning();
void destroyMFCC();
void MFCCFFT();
void PSD2Freq();
float hz2Mel(float hz);
float mel2Hz(float mel);
void melPoints();
void createBanknfilterxPSD();
void genDCTOut(float* DCTOut);


#endif
