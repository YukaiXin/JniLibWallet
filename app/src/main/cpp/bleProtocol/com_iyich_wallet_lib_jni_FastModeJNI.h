/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_iyich_wallet_lib_jni_FastModeJNI */

#ifndef _Included_com_iyich_wallet_lib_jni_FastModeJNI
#define _Included_com_iyich_wallet_lib_jni_FastModeJNI
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_iyich_wallet_lib_jni_FastModeJNI
 * Method:    sendData
 * Signature: ([BI)Z
 */
JNIEXPORT jboolean JNICALL Java_com_iyich_wallet_lib_jni_FastModeJNI_sendData
  (JNIEnv *, jobject, jbyteArray, jint);

/*
 * Class:     com_iyich_wallet_lib_jni_FastModeJNI
 * Method:    onReceive
 * Signature: ([BILjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_iyich_wallet_lib_jni_FastModeJNI_onReceive
  (JNIEnv *, jobject, jbyteArray, jint, jstring);

#ifdef __cplusplus
}
#endif
#endif
