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
 * Method:    ecdhMultipy
 * Signature: ([B[B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_iyich_wallet_lib_jni_SecLinkJni_ecdhMultipy
  (JNIEnv *, jobject, jbyteArray, jbyteArray);

/*
 * Class:     com_iyich_wallet_lib_jni_SecLinkJni
 * Method:    veritySession
 * Signature: ([B[B)Z
 */
JNIEXPORT jboolean JNICALL Java_com_iyich_wallet_lib_jni_SecLinkJni_veritySession
  (JNIEnv *, jobject, jbyteArray, jbyteArray);

#ifdef __cplusplus
}
#endif
#endif