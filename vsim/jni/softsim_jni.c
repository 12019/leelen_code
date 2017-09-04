#define __REMOTE_SERVICE__

#include "./softsim/softsim_include.h"
#if defined(__REMOTE_SERVICE__)
#include "com_tgt_device_manager_remote_RemoteVSIMService.h"
#endif
#include "stdio.h"
#include "memory.h"
#include "jni.h"

#ifdef __cplusplus
extern "C" { 
#endif


#if defined(__REMOTE_SERVICE__)
/*
 * Class:     com_tgt_device_manager_remote_RemoteVSIMService
 * Method:    GetVSIMVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_tgt_device_manager_remote_RemoteVSIMService_GetVSIMVersion
  (JNIEnv *env, jobject object)
{
	jint ret = 0;
	
	softsim_cos_init();

	#if defined(__SOFTSIM_MONITOR__)
		if ((ret = socket_status_monitor_Start(env,object)) < 0)
		{
			return ret;
		}
	#endif
	
	return (jint)softsim_get_version_number();
}

/*
 * Class:     com_tgt_device_manager_remote_RemoteVSIMService
 * Method:    VSIMSendSIMData
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_com_tgt_device_manager_remote_RemoteVSIMService_VSIMSendSIMData
  (JNIEnv *env, jobject object, jstring ssid, jstring iccid, jstring imsi, jstring plmn_list, jstring fplmn_list, jstring mnc_len, jint sim_id, jstring ip, jint port)
{
	jint ret = 0;
	
	const char *ssid_p = (*env)->GetStringUTFChars(env, ssid, NULL);
	const char *imsi_p = (*env)->GetStringUTFChars(env, imsi, NULL);
	const char *iccid_p = (*env)->GetStringUTFChars(env, iccid, NULL);
	const char *plmn_list_p = (*env)->GetStringUTFChars(env, plmn_list, NULL);
	const char *fplmn_list_p = (*env)->GetStringUTFChars(env, fplmn_list, NULL);
	const char *mnc_len_p = (*env)->GetStringUTFChars(env, mnc_len, NULL);
	const char *ip_p = (*env)->GetStringUTFChars(env, ip, NULL);

	ret = softsim_cos_data_load(ssid_p, iccid_p, imsi_p, plmn_list_p, fplmn_list_p, mnc_len_p, sim_id, ip_p, port);

	(*env)->ReleaseStringUTFChars(env, ssid, ssid_p);
	(*env)->ReleaseStringUTFChars(env, imsi, imsi_p);
	(*env)->ReleaseStringUTFChars(env, iccid, iccid_p);
	(*env)->ReleaseStringUTFChars(env, plmn_list, plmn_list_p);
	(*env)->ReleaseStringUTFChars(env, fplmn_list, fplmn_list_p);
	(*env)->ReleaseStringUTFChars(env, mnc_len, mnc_len_p);
	(*env)->ReleaseStringUTFChars(env, ip, ip_p);
		
	return ret;
}

/*
 * Class:     com_tgt_device_manager_remote_RemoteVSIMService
 * Method:    VSIMUpdateStatus
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_com_tgt_device_manager_remote_RemoteVSIMService_VSIMUpdateStatus
  (JNIEnv *env, jobject object, jint ps_rt, jint vs_rt)
{
	return softsim_cos_channel_status_update(ps_rt, vs_rt);
}

/*
 * Class:     com_tgt_device_manager_remote_RemoteVSIMService
 * Method:    VSIMStart
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_tgt_device_manager_remote_RemoteVSIMService_VSIMStart
  (JNIEnv *env, jobject object, jstring plmn)
{
	jint ret = 0;

	/*#if defined(__SOFTSIM_MONITOR__)
		socket_status_monitor_Init(env,object);
		socket_status_monitor_Start();
	#endif*/

	const char *plmn_p = (*env)->GetStringUTFChars(env, plmn, NULL);
	ret = softsim_cos_start(plmn_p);
	
	(*env)->ReleaseStringUTFChars(env, plmn, plmn_p);
	
	return ret;
}

/*
 * Class:     com_tgt_device_manager_remote_RemoteVSIMService
 * Method:    VSIMStop
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_tgt_device_manager_remote_RemoteVSIMService_VSIMStop
  (JNIEnv *env, jobject object, jint cause_value)
{
	#if defined(__SOFTSIM_MONITOR__)
		//socket_status_monitor_Stop();
	#endif
	
	return (jint)softsim_cos_stop(cause_value);
}

/*
 * Class:     com_tgt_device_manager_remote_RemoteVSIMService
 * Method:    VSIMSendSignal
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_tgt_device_manager_remote_RemoteVSIMService_VSIMSendSignal
  (JNIEnv *env, jobject object, jint signal)
{
	#if defined(__SOFTSIM_MONITOR__)
		return softsim_send_signal(signal);
	#else
		return 0;
	#endif
}
#endif

#ifdef __cplusplus
}
#endif



