#include "MFCC.h"
#include "Transforms.h"
#include <android/log.h>

int stepSize;
int windowSize;
int points;
int psdl;
int Fs;
int noFilters;
float* MFCCinBuffer;
float* hanningBuffer;
float* FFTBuffer;
float* PSDBuffer;
float* freqMap;		// the array containing mapping of n(0 -> N) to corresponding frequencies for PSD
float* filterBankFreq;
float** filterBank;
float* filterxPSD;
float** DCTBuffer;
float* DCTOut;
Transform* transform;

void getMFCCValues(float* inputBuffer, int stepsize, int windowsize, int F)
{
	stepSize = stepsize;
	windowSize = windowsize;
	Fs = F;
	initializeMFCC();
	MFCCinBuffer = inputBuffer;
}

void initializeMFCC()
{
		int i;
		points = pow(2,ceil((log(windowSize)/log(2))));
		psdl = (points/2) + 1;
		noFilters = 40;
		MFCCinBuffer = (float*) calloc(windowSize, sizeof(float));
		hanningBuffer = (float*) calloc(windowSize, sizeof(float));
		FFTBuffer = (float*) calloc(points, sizeof(float));
		PSDBuffer = (float*) calloc(psdl, sizeof(float));
		freqMap = (float*) calloc(psdl, sizeof(float));
		filterBankFreq = (float*) calloc(noFilters + 2, sizeof(float));

		filterBank = (float**) calloc(noFilters, sizeof(float*));
		for(i = 0; i < noFilters; i++)
			{
				filterBank[i] = (float*) calloc(psdl, sizeof(float));
			}

		filterxPSD = (float*) calloc(noFilters, sizeof(float));

		DCTBuffer = (float**) calloc(13, sizeof(float*));
			for(i = 0; i < 13; i++)
			{
				DCTBuffer[i] = (float*) calloc(noFilters, sizeof(float));
			}
		DCTOut = (float*) calloc(13, sizeof(float));
		transform = newTransform(points);
}

void hanning()
{
		int i;

		i = windowSize/2;
		do
		{
			hanningBuffer[i-1] = 0.5 * (1 - cos(2*M_PI*(i)/(windowSize+1)));
			i--;
		}while(i!=0);

		i = windowSize/2;
		do
		{
			hanningBuffer[i+(windowSize/2)- 1] = hanningBuffer[(windowSize/2) - i];
			i--;
		}while(i!=0);
}

void MFCCFFT()
{
	int i;
	hanning();
	i = windowSize;
	do
	{
		FFTBuffer[i-1] = MFCCinBuffer[i-1] * hanningBuffer[i-1];
		i--;
	}while(i!=0);

	transform->doTransform(transform, FFTBuffer);
	i = psdl;
	do
	{
		float tempr = 0;
		float tempi = 0;
		tempr = transform->real[i-1] * transform->real[i-1];
		tempi = transform->imaginary[i-1] * transform->imaginary[i-1];
		PSDBuffer[i-1] = tempr + tempi;
		i--;
	}while(i!=0);
	PSD2Freq();
}


void PSD2Freq()
{
	int i;
	float mel;
	i = psdl;
	do
	{
		freqMap[i-1] = (float)((i-1)*Fs)/points;
		i--;
	}while(i!=0);
}


float hz2Mel(float hz)
{
	float m;
	float breakfreq = 1000.0;
	float breakpoint = 15.0;
	float logstep = (float) exp(logf(6.4)/27.0);
	if(hz <= breakfreq)
	{
		m = (float) hz*(3.0/200.0);
	}
	else
	{
		m = (float) (breakpoint + (logf(hz/breakfreq)/logf(logstep)));
	}
	return m;
}

float mel2Hz(float mel)
{
	float f;
	float breakfreq = 1000.0;
	float breakpoint = 15.0;
	float logstep = exp(logf(6.4)/27.0);
	if(mel <= breakpoint)
	{
		f = (float) mel*(200.0/3.0);
	}
	else
	{
		f = (float) (breakfreq * exp(logf(logstep) * (mel - breakpoint)));
	}
	return f;
}

void melPoints()
{
	int i;
	float temp = 0.0;
	float melMax = hz2Mel(Fs/2);
	float melMin = 0.0;
	float diff = (melMax - melMin)/(noFilters + 1);

	for(i = 1; i < (noFilters + 3); i++)
	{
		temp = (float)(melMin + (i-1)*diff);
		filterBankFreq[i-1] = mel2Hz(temp);
	}
}

void createBanknfilterxPSD()
{
	int i,j;
	float one,two,three;
	float slopeh,slopel;
	float wt1,wt,temp;

	for(i = 0; i < noFilters; i++)
	{
		one = filterBankFreq[i];
		two = filterBankFreq[i+1];
		three = filterBankFreq[i+2];
		wt = (float) (2.0/(three - one));
		//__android_log_print(ANDROID_LOG_INFO, "wttag","wt is %f", wt);

		temp = 0.0;
		for(j = 0; j< psdl; j++)
		{
			slopel = (float) ((freqMap[j] - one)/(two - one));
			slopeh = (float) ((three - freqMap[j])/(three - two));

			if(slopel <= slopeh)
				wt1 = slopel;
			if(slopeh <= slopel)
				wt1 = slopeh;
			if(wt1 < 0.0)
				wt1 = 0.0;

			filterBank[i][j] = wt1 * wt;
			//__android_log_print(ANDROID_LOG_INFO, "wttag","wt for row %d is %f", i, filterBank[i][j]);
			temp = temp + (float) filterBank[i][j] * PSDBuffer[j];
		}
		filterxPSD[i] = logf(temp);
	}
}

void genDCTOut(float* DCTOut)
{
	int i,j;
	float p = 0.223606797;
	float m = 1.414213562;
	float lifter[13] = {1.0000, 1.0000, 1.5157, 1.9332, 2.2974, 2.6265, 2.9302, 3.2141, 3.4822, 3.7372, 3.9811, 4.2154, 4.4413};

	for(i = 1; i < 14; i++)
	{
		for(j = 1; j < noFilters + 1; j++)
		{
	        DCTBuffer[i-1][j-1] =  (float) cos((M_PI * (2*j-1) * (i-1)) / (80)) * p;
		}
	}

	for(i = 0; i < 40; i++)
	{
		DCTBuffer[0][i] = (float) DCTBuffer[0][i] / m;
	}

	for(i = 0; i < 13; i++)
	{
	    for(j = 0; j < noFilters; j++)
	    {
	    	DCTOut[i] += DCTBuffer[i][j] * filterxPSD[j];
	    }
	    DCTOut[i] = DCTOut[i]* lifter[i];
	}

	//return DCTOut;

}



void destroyMFCC()
{
	int i;
	__android_log_print(ANDROID_LOG_INFO, "mytag","MFCC destroy begin");
	free(MFCCinBuffer);
	free(hanningBuffer);
	free(FFTBuffer);
	free(PSDBuffer);
	free(freqMap);
	free(filterBankFreq);
	for(i = 0; i< noFilters; i++)
	{
		free(filterBank[i]);
	}
	free(filterBank);
	free(filterxPSD);

	for(i = 0; i< 13; i++)
	{
		free(DCTBuffer[i]);
	}
	free(DCTBuffer);
	free(DCTOut);

	__android_log_print(ANDROID_LOG_INFO, "mytag","MFCC destroy end");
}
