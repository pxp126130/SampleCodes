#include <jni.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include "Timer.h"
#include "Transforms.h"

typedef struct Variables {
	Timer* timer;
	Transform* transform;
	int frameSize;
	int offset;
	float a;
	float* inputBuffer;
} Variables;

static const float maxShort = 1/32768.0f;

jfloatArray
compute(JNIEnv *env, jobject thiz,  jlong memoryPointer, jshortArray input)
{
	Variables* inParam = (Variables*) memoryPointer;
	start(inParam->timer);

	int i;
	short *_in = (*env)->GetShortArrayElements(env, input, NULL);
	for(i=0; i<inParam->offset; i++) {
		inParam->inputBuffer[i] = inParam->inputBuffer[inParam->frameSize + i];
	}
	for(i=0; i<inParam->frameSize; i++) {
		inParam->inputBuffer[inParam->offset + i] = _in[i]*maxShort;
	}
	(*env)->ReleaseShortArrayElements(env, input, _in, 0);
	jfloatArray output = (*env)->NewFloatArray(env, inParam->frameSize);
	float *_output = (*env)->GetFloatArrayElements(env, output, NULL);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
	 *
	 * difference equation and/or frequency domain method here.
	 * dft/fft calling convention: inParam->transform->doTransform(inParam->transform, inParam->inputBuffer)
	 *
	 * You will need to modify the Transforms.c code to accept complex input for the inverse operation.
	 *
	 * Set offset = 0 when doing the frequency domain method.
	 *
	 */

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	(*env)->ReleaseFloatArrayElements(env, output, _output, 0);
	stop(inParam->timer);
	return output;
}

jlong
initialize(JNIEnv* env, jobject thiz, jint framesize, jfloat a, jint z)
{
	int transform = 1; // 0 = dft, 1 = fft

	Variables* inParam = (Variables*) malloc(sizeof(Variables));
	inParam->timer = newTimer();
	inParam->frameSize = framesize;
	inParam->a = a;
	inParam->offset = z;
	inParam->transform = newTransform(transform, framesize);
	inParam->inputBuffer = (float*) calloc((inParam->offset)+framesize,sizeof(float));

	__android_log_print(ANDROID_LOG_ERROR, "FreqTrans", "Alpha: %f, Delay: %d, Frame: %d", a, z, framesize);

	return (jlong)inParam;
}

void
finish(JNIEnv* env, jobject thiz, jlong memoryPointer)
{
	Variables* inParam = (Variables*) memoryPointer;
	if(inParam != NULL){
		if(inParam->inputBuffer != NULL){
			free(inParam->inputBuffer);
			inParam->inputBuffer = NULL;
		}
		tellTime(inParam->timer);
		destroy(&(inParam->timer));
		destroyTransform(&(inParam->transform));
		free(inParam);
		inParam = NULL;
	}
}

float
getTime(JNIEnv* env, jobject thiz, jlong memoryPointer)
{
	Variables* inParam = (Variables*) memoryPointer;
	return getMS(inParam->timer);
}

////////////////////////////////////////////////////////////////////////////////////////////
// JNI Setup
////////////////////////////////////////////////////////////////////////////////////////////

static JNINativeMethod nativeMethods[] =
	{//		Name							Signature			Pointer
			{"compute", 					"(J[S)[F",			(void *)&compute				},
			{"initialize",					"(IFI)J",			(void *)&initialize				},
			{"finish",						"(J)V",				(void *)&finish					},
			{"getTime",						"(J)F",				(void *)&getTime				}
	};

jint
JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env;
	jint result;
	//get a hook to the environment
	result = (*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_6);
	if (result == JNI_OK) {
		//find the java class to hook the native methods to
		jclass filters = (*env)->FindClass(env, "com/dsp/transforms/FreqTrans");
		if (filters != NULL) {
			result = (*env)->RegisterNatives(env, filters, nativeMethods, sizeof(nativeMethods)/sizeof(nativeMethods[0]));
			(*env)->DeleteLocalRef(env, filters);
			if(result == JNI_OK){
				return JNI_VERSION_1_6;
			} else {
				//something went wrong with the method registration
				return JNI_ERR;
			}
		} else {
			//class wasn't found
			return JNI_ERR;
		}
	} else {
		//could not get environment
		return JNI_ERR;
	}
}
