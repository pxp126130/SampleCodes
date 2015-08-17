#include <jni.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include "Timer.h"
#include "Transforms.h"
#include "VAD.h"
#include "MFCC.h"
#include "GMM.h"
#include "logMMSE.h"

int count = 1;
int framecnt = 0;

typedef struct Variables {
	Timer* timer;
	Transform* transform;
	float* inputBuffer;
	float* outputBuffer;
	float* vadBuffer;

	float* hanningBuffer;
	float* fftBuffer;
	float* fftMagBuffer;

	float* mfccOutBuffer;

	float* first;
	float* hannfirst;
	float* second;
	float* hannsecond;
	float* third;
	float* hannthird;
	float* out;
	float* hannout;

	float* weights;

	int frequency;
	int stepSize;
	int windowSize;
	int vadSize;
	int overlap;
	int points;
	int psd;
} Variables;

static void
compute(JNIEnv *env, jobject thiz, jlong memoryPointer,	jshortArray input)
{
	Variables* inParam = (Variables*) memoryPointer;
	start(inParam->timer);

	short *_in = (*env)->GetShortArrayElements(env, input, NULL);
	int i, j;
	static int order = 12;
	int classDec;

	//////////////////////////////  input buffer initialization //////////////////////////////

	for(i=0; i<inParam->overlap; i++)
	{
		inParam->inputBuffer[i] = inParam->inputBuffer[inParam->stepSize + i];
	}

	for (i=0; i<inParam->stepSize; i++)
	{
		inParam->inputBuffer[inParam->overlap + i] = _in[i]/32768.0;
	}

	//////////////////////////////  vad buffer initialization //////////////////////////////
	i = order - 1;
	do
	{
		inParam->vadBuffer[i - 1] = inParam->vadBuffer[inParam->stepSize+ i - 1];
		i--;
	}while(i!=0);

	i = inParam->stepSize;
	do
	{
		inParam->vadBuffer[i + order - 2] = _in[i - 1]/32768.0f;
		i--;
	}while(i!=0);

	(*env)->ReleaseShortArrayElements(env, input, _in, 0);

	//////////////////////////////// Hanning operation /////////////////////////////////////////
	i = inParam->windowSize/2;
	do
	{
		inParam->hanningBuffer[i-1] = 0.5 * (1 - cos(2*M_PI*(i)/(inParam->windowSize+1)));
	}while(--i!=0);

	i = inParam->windowSize/2;
	do
	{
		inParam->hanningBuffer[i+(inParam->windowSize/2)- 1] = inParam->hanningBuffer[(inParam->windowSize/2) - i];
	}while(--i!=0);

	////////////////////////////////////// FFT operation////////////////////////////////////////
	i = inParam->windowSize;
	do
	{
	inParam->fftBuffer[i-1] = inParam->inputBuffer[i-1] * inParam->hanningBuffer[i-1];
	}while(--i!=0);
	inParam->transform->doTransform(inParam->transform, inParam->fftBuffer);

	i = inParam->points;
	do
	{
	float tempr = 0;
	float tempi = 0;
	tempr = inParam->transform->real[i-1] * inParam->transform->real[i-1];
	tempi = inParam->transform->imaginary[i-1] * inParam->transform->imaginary[i-1];
	inParam->fftMagBuffer[i-1] = tempr + tempi;
	}while(--i!=0);


	/////////////////////////////// VAD //////////////////////////////////////
//	getValues(inParam->vadBuffer, inParam->vadSize, inParam->frequency);
//	initializevad();
//	inParam->outputBuffer[0] = decideVad();

	/////////////////////////////// MFCC ////////////////////////////////////////

	getMFCCValues(inParam->inputBuffer, inParam->stepSize, inParam->windowSize, inParam->frequency);
	MFCCFFT();
	melPoints();
	createBanknfilterxPSD();
	genDCTOut(inParam->mfccOutBuffer);

	////////////////////////////////// GMM /////////////////////////////////////
	//classdec = computeProb(inParam->mfccOutBuffer);

	classDec = 0;

	//////////////////////////////// logMMSE ///////////////////////////////////


	if(classDec == -1)
		classDec = 0;

	InitGD2(0);
	__android_log_print(ANDROID_LOG_INFO, "mytag","back from initGD2");

	if(framecnt < 6)
	{
		initNoiseD2(inParam->fftMagBuffer);
		__android_log_print(ANDROID_LOG_INFO, "mytag","back from initNoiseD2 %d", framecnt);
	}



	if(framecnt == 6)
	{
		setNoiseD2(framecnt);
		__android_log_print(ANDROID_LOG_INFO, "mytag","back from setNoiseD2 for 7th");
		inParam->weights = Supress(inParam->fftMagBuffer);
		__android_log_print(ANDROID_LOG_INFO, "mytag","back from supress for 7th");
	}




	if(framecnt > 6)
	{
		inParam->weights = Supress(inParam->fftMagBuffer);
		__android_log_print(ANDROID_LOG_INFO, "mytag","back from supress %d",framecnt);
	}

	__android_log_print(ANDROID_LOG_ERROR, "mytag", "Count %d", framecnt);



	for(i=0;i<inParam->points;i++)
	{
				inParam->transform->real[i] *= inParam->weights[i];
				inParam->transform->imaginary[i] *=  inParam->weights[i];
	}



	//////////////////////////////// synthesis of noise filtered signal /////////////////////
	IFFT(inParam->transform);
	int overlap = inParam->overlap;
	int stepsize = inParam->stepSize;
	int windowsize = inParam->windowSize;
	int t = overlap-stepsize;
	if(count==1){
		for(i=0;i<windowsize;i++){
			inParam->first[i] = inParam->transform->real[i];
			inParam->hannfirst[i] = inParam->hanningBuffer[i];
		}
	}
	else if(count==2){
		count =0;
		for(i=0;i<windowsize;i++){
			inParam->second[i] = inParam->transform->real[i];
			inParam->hannsecond[i] = inParam->hanningBuffer[i];
		}
	}
	else{
		for(i=0;i<windowsize;i++){
			inParam->third[i] = inParam->transform->real[i];
			inParam->hannthird[i] = inParam->hanningBuffer[i];
		}

		for(i=0;i<stepsize;i++){
			inParam->out[i] = inParam->first[i+overlap] + inParam->second[i+t];
			inParam->hannout[i] = inParam->hannfirst[i+overlap] + inParam->hannsecond[i+t];
		}
		for(i=0;i<t;i++){
			inParam->out[i+stepsize-t] += inParam->third[i];
			inParam->hannout[i+stepsize-t] += inParam->hannthird[i];
		}
		for(i=0;i<windowsize;i++){
			inParam->first[i] = inParam->second[i];
			inParam->hannfirst[i] = inParam->hannsecond[i];
			inParam->second[i] = inParam->third[i];
			inParam->hannsecond[i] = inParam->hannthird[i];
		}
	}

	for(i=0;i<stepsize;i++){
		inParam->outputBuffer[i] = inParam->out[i] * (1.0/inParam->hannout[i]);
	}
	if(count==1) count = 2;

	framecnt++;
	if(framecnt == 6000)
		framecnt = 0;
	stop(inParam->timer);
}

static jlong initialize(JNIEnv* env, jobject thiz, jint freq, jint stepsize, jint windowsize)
{
	Variables* inParam = (Variables*) malloc(sizeof(Variables));
	inParam->timer = newTimer();
	inParam->frequency = freq;
	inParam->stepSize = stepsize;
	inParam->points = pow(2,ceil((log(windowsize)/log(2))));
	inParam->psd = (inParam->points/2) + 1;
	inParam->windowSize = windowsize;
	inParam->vadSize = stepsize + 11;
	inParam->overlap = windowsize - stepsize;
	inParam->inputBuffer = (float*) calloc(inParam->windowSize, sizeof(float));
	inParam->outputBuffer = (float*) calloc(inParam->stepSize, sizeof(float));
	inParam->vadBuffer = (float*) calloc(inParam->vadSize, sizeof(float));

	inParam->hanningBuffer = (float*) calloc(inParam->windowSize, sizeof(float));
	inParam->fftBuffer = (float*) calloc(inParam->points, sizeof(float));
	inParam->fftMagBuffer = (float*) calloc(inParam->points, sizeof(float));

	inParam->mfccOutBuffer = (float*) calloc(13, sizeof(float));
	inParam->weights = (float*) calloc(inParam->points, sizeof(float));

	inParam->first = (float*) calloc(inParam->windowSize, sizeof(float));
	inParam->second = (float*) calloc(inParam->windowSize, sizeof(float));
	inParam->third = (float*) calloc(inParam->windowSize, sizeof(float));
	inParam->hannfirst = (float*) calloc(inParam->windowSize, sizeof(float));
	inParam->hannsecond = (float*) calloc(inParam->windowSize, sizeof(float));
	inParam->hannthird = (float*) calloc(inParam->windowSize, sizeof(float));

	inParam->out = (float*) calloc(inParam->stepSize, sizeof(float));
	inParam->hannout = (float*) calloc(inParam->stepSize, sizeof(float));

	inParam->transform = newTransform(inParam->points);

	initLogMMSE(inParam->frequency,inParam->points);

	return (jlong) inParam;
}

static void
finish(JNIEnv* env, jobject thiz, jlong memoryPointer)
{
	//destroyMFCC();
	__android_log_print(ANDROID_LOG_INFO, "mytag","inside sp finish begin");
	Variables* inParam = (Variables*) memoryPointer;
	//cleanup memory
	if (inParam != NULL) {

		tellTime(inParam->timer);
		destroy(&(inParam->timer));
		__android_log_print(ANDROID_LOG_INFO, "mytag","inside timer finish");

		free(inParam->inputBuffer);
		inParam->inputBuffer = NULL;
		__android_log_print(ANDROID_LOG_INFO, "mytag","inside input buffer finish");

		free(inParam->fftBuffer);
		inParam->fftBuffer = NULL;
		__android_log_print(ANDROID_LOG_INFO, "mytag","inside fft buffer finish");

		free(inParam->outputBuffer);
		inParam->outputBuffer = NULL;
		__android_log_print(ANDROID_LOG_INFO, "mytag","inside output buffer finish");

		free(inParam->vadBuffer);
		__android_log_print(ANDROID_LOG_INFO, "mytag","inside vad finish");

		free(inParam->hanningBuffer);
		__android_log_print(ANDROID_LOG_INFO, "mytag","inside hann finish");

		free(inParam);
		inParam = NULL;
	}
	__android_log_print(ANDROID_LOG_INFO, "mytag","inside sp finish end");
}

static jfloat getTime(JNIEnv* env, jobject thiz, jlong memoryPointer) {
	Variables* inParam = (Variables*) memoryPointer;
	return getMS(inParam->timer);
}

static jshortArray getOutput(JNIEnv* env, jobject thiz, jlong memoryPointer,
		jint outputSelect) {
	Variables* inParam = (Variables*) memoryPointer;
	int i;

	// check below statement for windowSize
	jshortArray output = (*env)->NewShortArray(env, inParam->windowSize);
	short *_output = (*env)->GetShortArrayElements(env, output, NULL);

	if(outputSelect == 0) { //Case 1 - Original input signal
	 for(i=0;i<inParam->stepSize;i++)
	 {
		 _output[i] = inParam->inputBuffer[inParam->overlap+i]*32768;
	 }
	}
	 else {				//Case 2 - Processed output signal
		 for(i = 0 ; i < inParam->stepSize; i++)
		 {
			 _output[i] = inParam->outputBuffer[i]*32768;
		 }
	 }
	(*env)->ReleaseShortArrayElements(env, output, _output, 0);
	return output;
}

static jfloatArray getDebug(JNIEnv* env, jobject thiz, jlong memoryPointer, jint debugSelect) {
	Variables* inParam = (Variables*) memoryPointer;

	__android_log_print(ANDROID_LOG_INFO, "mytag","inside getDebug");
		jfloatArray debugOutput = NULL;
	int i;
	if (debugSelect == 0)  //Test Case 1 - inputBuffer contents
	{
		debugOutput = (*env)->NewFloatArray(env,inParam->windowSize);
		float *_debugOutput = (*env)->GetFloatArrayElements(env, debugOutput, NULL);

		for (i = 0; i < inParam->windowSize; i++)
		{
			_debugOutput[i] = inParam->inputBuffer[i];
		}

		(*env)->ReleaseFloatArrayElements(env, debugOutput, _debugOutput, 0);
	}
	else if (debugSelect == 1)  //Test Case 2 - outputBuffer contents
	{
		//debugOutput = (*env)->NewFloatArray(env, inParam->psd);
		debugOutput = (*env)->NewFloatArray(env,13);
		float *_debugOutput = (*env)->GetFloatArrayElements(env, debugOutput, NULL);

		for (i = 0; i < 13 ; i++)
		{
			_debugOutput[i] = inParam->outputBuffer[i];
		}

		(*env)->ReleaseFloatArrayElements(env, debugOutput, _debugOutput, 0);
	}

	//Add additional cases to output other data that may be needed.
	return debugOutput;
}

////////////////////////////////////////////////////////////////////////////////////////////
// JNI Setup - Functions and OnLoad
////////////////////////////////////////////////////////////////////////////////////////////

static JNINativeMethod nativeMethods[] =
	{//		Name							Signature			Pointer
			{ "compute", 					"(J[S)V",			(void *) &compute 			},
			{ "initialize", 				"(III)J",			(void *) &initialize 		},
			{ "finish", 					"(J)V", 			(void *) &finish 			},
			{ "getTime", 					"(J)F", 			(void *) &getTime 			},
			{ "getOutput",					"(JI)[S", 			(void *) &getOutput 		},
			{ "getDebug", 					"(JI)[F",			(void *) &getDebug 			}
	};

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	__android_log_print(ANDROID_LOG_INFO, "mytag","inside error wala part1");
	JNIEnv* env;
	jint result;
	//get a hook to the environment
	result = (*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_6);
	if (result == JNI_OK)
	{
		__android_log_print(ANDROID_LOG_INFO, "ertag","inside error wala part2");
		//find the java class to hook the native methods to
		jclass filters = (*env)->FindClass(env,"com/dsp/speechpipeline/SpeechProcessing");
		if (filters != NULL)
		{
			__android_log_print(ANDROID_LOG_INFO, "ertag","inside error wala part3");
			result = (*env)->RegisterNatives(env, filters, nativeMethods, sizeof(nativeMethods) / sizeof(nativeMethods[0]));
			(*env)->DeleteLocalRef(env, filters);
			if (result == JNI_OK)
			{
				__android_log_print(ANDROID_LOG_INFO, "ertag","inside error wala part4");
				return JNI_VERSION_1_6;
			}
			else
			{
				__android_log_print(ANDROID_LOG_INFO, "ertag","inside error wala part5");
				//something went wrong with the method registration
				return JNI_ERR;
			}
		}
		else
		{
			__android_log_print(ANDROID_LOG_INFO, "ertag","inside error wala part6");
			//class wasn't found
			return JNI_ERR;
		}
	}
	else
	{
		__android_log_print(ANDROID_LOG_INFO, "ertag","inside error wala part7");
		//could not get environment
		return JNI_ERR;
	}
}
