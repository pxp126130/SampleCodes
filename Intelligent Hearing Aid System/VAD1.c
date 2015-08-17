//VAD definition
#include "VAD.h"
#include <android/log.h>
#include <math.h>

static int order = 12;
static float HD[] = {0.00780070832503415, 0.00176771186424280, -0.0447249017706658, -0.0210602925123006, 0.0726375227864625, 0.337929421727622, -0.787641141030194, 0.491055941926747, 0.0483117425856330, -0.117990111148191, -0.00349071208421747, 0.0154041093270274};
static float LD[] = {0.0154041093270274, 0.00349071208421747, -0.117990111148191, -0.0483117425856330, 0.491055941926747, 0.787641141030194, 0.337929421727622, -0.0726375227864625, -0.0210602925123006, 0.0447249017706658, 0.00176771186424280, -0.00780070832503415};

int Na, Nf, buffSize, stepSize, Fs, qb;
int dbufInd;
int firstRunFlag = 1;
int firstInitFlag = 1;
int vadFlag;
int interVadDec, noVoiceCount;
float  vadDec, temp;
float tqb, epsqb, alpha, ads;
float preTqb;
float Px, Ph, Pl;
float D, Dw, Dc, Ds;
float preDs;
float* Xl;
float* Xld;
float* Xh;
float* Xhd;
float* vadBuffer;
float* dbuf;
float* dbufSort;

void getValues(float* inbuffer, int vadsize, int F)
{
	int i;
	buffSize = vadsize; // stepsize + 11;
	initializevad();
	vadBuffer = inbuffer;
	Fs = F;
}

void initializevad()
{
	stepSize = buffSize - 11;//	 pass step size ( do padding temp here not in sp.c)
	//Nf = (Fs/stepSize)/2;
	Nf = 100;
	Na = stepSize/2;
	if(firstInitFlag == 1)
	{
		vadBuffer = (float*) calloc(buffSize, sizeof(float));
		dbuf = (float*) calloc(Nf, sizeof(float));
		dbufSort = (float*) calloc(Nf, sizeof(float));
		Xl = (float*) calloc(stepSize, sizeof(float));
		Xh = (float*) calloc(stepSize, sizeof(float));
		Xld =  (float*) calloc(Na, sizeof(float));
		Xhd =  (float*) calloc(Na, sizeof(float));
		dbufInd = 0;
		preTqb = 0;
		preDs = 0;
		vadFlag = 0;
		noVoiceCount = 0;
		vadDec = 0.0;
		D = 0.0;
		Ds = 0.0;
		firstInitFlag = 0;

	}
	epsqb = 0.001;
	alpha = 0.975;
	ads = 0.65;

	Px = 0;
	Pl = 0;
	Ph = 0;

	Dw = 0;
	Dc = 0;
}

void convolve()
{
	//__android_log_print(ANDROID_LOG_INFO, "TESTING func call","inside convolve");
	int i,j;
	int idx = 0;
	for(i = 0;i < stepSize;i++)
	{
		float temp1 = 0;
		float temp2 = 0;
		for(j = 0;j < order;j++)
		{
			idx = order + (i - j) - 1 ;
			temp1 += vadBuffer[idx]*LD[j];
			temp2 += vadBuffer[idx]*HD[j];
		}
		Xl[i] = temp1;
		Xh[i] = temp2;
	}
}

void downsample()
{
	//__android_log_print(ANDROID_LOG_INFO, "TESTING func call","inside downsample");
	int i = Na-1;
	do
	{
		Xld[i] = Xl[2*i];
		Xhd[i] = Xh[2*i];
		i--;
	}while(i>=0);
}

void energy()
{
	int i;
	i = stepSize+10;
	do
	{
		Px = Px + (vadBuffer[i] * vadBuffer[i]);
		i--;
	}while(i>=11);

	i = Na - 1;
	do
	{
		Pl = Pl + Xld[i] * Xld[i];
		Ph = Ph + Xhd[i] * Xhd[i];
		i--;
	}while(i>=0);
}



void sortDbuf()
{

	int i,j;
	float temp = 0.0;

	for (i = 0; i < Nf; i++)
	{
		dbufSort[i] = dbuf[i];
	}

	for (i = Nf - 1; i > 0; i--)
	{
		for(j = 0;j < i; j++)
		{
			if(dbufSort[j] > dbufSort[j+1])
			{
				temp = dbufSort[j];
				dbufSort[j] = dbufSort[j+1];
				dbufSort[j+1] = temp;
			}
		}
	}
}

int compare(const void* a, const void* b)
{
	if(*(float*)a < *(float*)b)
	{
		return -1;
	}
	if(*(float*)a < *(float*)b)
	{
			return 1;
	}
	return 0;
}


int decideVad()
{
	int i;
	convolve();
	downsample();
	energy();
	D = (float) (fabsf(Ph-Pl) / Na);
	Dw = D * ( 0.5 + ( 16 / logf( 2 )) * log( 1 + 2*Px ));
	Dw = -2.0 * Dw;
	Dc = ( 1 - exp( Dw )) / ( 1 + exp( Dw ));
	Ds = Dc + ( preDs * 0.65 );
	preDs = Ds;
	dbuf[dbufInd] = Ds;
	__android_log_print(ANDROID_LOG_INFO, "Print","left sort %f",dbuf[dbufInd]);
	dbufInd++;

	if(dbufInd>=Nf)		// limit index of Dbuf to Nf
	{
		dbufInd = 0;
	}

	//sort Dbuf
	sortDbuf();

	qb = 4;
	while((( dbufSort[qb] - dbufSort[qb - 4]) < 0.001) && (qb < Nf - 1))
	{
		qb++;

	}

	tqb = dbufSort[qb];

	if(firstRunFlag == 0) //define globally
	{
		firstRunFlag = 1;
	}
	else
	{
		tqb = alpha * preTqb + (1 - alpha) * tqb;
	}

	preTqb = tqb; // not to be made zero for each frame

	if(Ds>tqb)
	{
		interVadDec = 1;
	}
	else
	{
		interVadDec = 0;
	}

	if(interVadDec == 1)
	{
		noVoiceCount = 0;
		vadFlag = 0;
		vadDec = 1;
	}
	else
	{
		if(vadFlag == 1)
		{
			vadDec = 0;
		}
		else
		{
			vadDec = 1;
			noVoiceCount++;
			if(noVoiceCount>40)
			vadFlag = 1;
		}
	}
	return vadDec;
}


void destroyVAD()
{
 	 __android_log_print(ANDROID_LOG_INFO, "TESTING func call","init destroy");
	free(vadBuffer);
	free(Xl);
	free(Xld);
	free(Xh);
	free(Xhd);
	free(dbuf);
	free(dbufSort);
	__android_log_print(ANDROID_LOG_INFO, "TESTING func call","freed memory");
}
