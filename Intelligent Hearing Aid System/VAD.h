#ifndef VAD_H
#define VAD_H
#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

/*
typedef struct myVAD {


	int Na, Nf, stepsize, buffsize, Fs, qb, idx, dbufInd;
	int firstRunFlag, vadFlag;
	int interVadDec, noVoiceCount, vadDec;
	float temp, temp1, temp2;
	float tqb, epsqb, alpha, ads, preTqb;
	float Px, Ph, Pl;
	float D, Dw, Dc, Ds, preDs;
	float* Xl;

	int stepsize;
	float* VADBuffer;

} myVAD;

myVAD* getValues(int stepsize,float* inbuffer);	// defined
//myVAD* sample(myVAD* getVal);

*/


//void destroyVAD();

void getValues(float* inbuffer, int vadsize, int F);	// defined
void initializevad();									// defined
int decideVad();										// defined
void convolve();										// defined
void downsample();										// defined
void energy();											// defined
void destroyVAD();
int compare(const void* a, const void* b);

#endif
