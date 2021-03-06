/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_iyich_wallet_lib_jni_SecLinkJni */

#ifndef _Included_com_iyich_wallet_lib_jni_SecLinkJni
#define _Included_com_iyich_wallet_lib_jni_SecLinkJni
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_iyich_wallet_lib_jni_SecLinkJni
 * Method:    getRsaPrivateKey
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_iyich_wallet_lib_jni_SecLinkJni_getRsaPrivateKey
  (JNIEnv *, jclass);

/*
 * Class:     com_iyich_wallet_lib_jni_SecLinkJni
 * Method:    getDevicePublicKey
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_iyich_wallet_lib_jni_SecLinkJni_getDevicePublicKey
  (JNIEnv *, jclass);

/*
 * Class:     com_iyich_wallet_lib_jni_SecLinkJni
 * Method:    veritySession
 * Signature: ([B[B[B[B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_iyich_wallet_lib_jni_SecLinkJni_veritySession
  (JNIEnv *, jclass, jbyteArray, jbyteArray, jbyteArray, jbyteArray);

/*
 * Class:     com_iyich_wallet_lib_jni_SecLinkJni
 * Method:    getModelKey
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_iyich_wallet_lib_jni_SecLinkJni_getModelKey
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
