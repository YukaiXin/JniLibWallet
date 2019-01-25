
#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <android/log.h>
#include <string.h>
#include "sec/ecdsa.h"
#include "sec/secp256k1.h"
#include "sec/pbkdf2.h"
#include "sec/sing/aes/aes.h"
#include "bleProtocol/fastmode.h"


#define LOG_TAG "System.out"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

char *APK_SIGN = "";
char *SERVER_KEY = "MIICdQIBADANBgkqhkiG9w0BAQEFAASCAl8wggJbAgEAAoGBAJOW1A7cat7vRIXu\nGXVjul5MzftF8o9vNmoc3QhkqvC4WzJ0H8LyT7iDVdjGWgsiGez2zntm/UlsWjC4\nHWB2Hr43EXMxzxhD82ktRoxWcpJrdVxa3xA8BVxwo6vw038Mi11+KvJvggDSb+7U\n9H+PGTkG5StFhqArEFeWXu2IWNxDAgMBAAECgYBydTFFySjUdYiVuT5jLJFXGodQ\nDEH9fMkdZ/7+yFtNThvjt+z92b97n9cOw10Hb03d2NXvG8LokzAqwk5+BXz254OX\nQxHlRFhZ0f8A9Htzbz9Qrd30JCGeXS06UfjljabYMfkh7HR07IPKadDDRbQZ3Y4P\n0h7rCgflCh36lz8KGQJBAMzfObp853qEybDXULGe1pKzFtlxdZAlnelbwzqRxdNz\nOSXhacImQ2wkvWNc9dJy+aWNdgQhp4uVaHsBAkukEQcCQQC4a/F+5AWhKgJ22sF4\nu2qEfjF5EuPFd7hJ7G0vq37fd0Jn7NXhM3+KqqM2xniqHzDHsASYe8wtfZu2BkdJ\nAhflAkBJFkmt8elSmBSr9eRfuV6rLyCzqfpa1lCjc40OU/rTGIQGs4fbL3NWHTVD\nQWKhFUtieDjl+GuhLEf9ubpr6bf5AkBWVAEp2udft+CLbn9eWqzT9JaIDHWevBoC\nULATvn2XcaYeK9k6n/a2HIZmX6WgTRVyhEzCbED5knDPVkUMQfSZAkBQYcujE8Mc\n7WIrP0LQaxUjkTcmwXIdR/iWb+KdVL21PKTk8nfdue1TkZfCkqsP9lnBoZx9REmD\nQ+/7FeKFzccV";
char *DEVICE_PUBLIC_KEY = "0220b4c720870cc6f5e4d416417fdf91d43e08c19ccc1321552f41a97983b00216";

///* Header for class com_iyich_wallet_lib_jni_FastModeJNI */
//JavaVM *jvm;
//jobject gobal_obj;
//
//void callBackRecvBuff(JNIEnv* _env, int pos, unsigned char *data, int len){
//    jbyteArray dataArray =  (*_env) -> NewByteArray(_env, len);
//    (*_env) -> SetByteArrayRegion(_env, dataArray, 0, len, (jbyte *)data);
//    jbyte * jbyteP = (*_env) -> GetByteArrayElements(_env, dataArray, 0);
//    jclass fastClass = (*_env) -> GetObjectClass(_env, gobal_obj);
//
//    jmethodID jmethodID1 = (*_env) -> GetMethodID(_env, fastClass, "RecieveBuff", "([BI)V");
//
//    (*_env)->CallVoidMethod(_env, gobal_obj, jmethodID1, dataArray, len);
//    (*_env) ->ReleaseByteArrayElements(_env, dataArray, jbyteP,  JNI_ABORT);
//}
//
//bool receiveBuff(int pos,unsigned char *data, int len){
//
//    JNIEnv* _env;
//    int envStatus = (*jvm)->GetEnv(jvm, (void**)&_env, JNI_VERSION_1_6);
//    if(envStatus == JNI_EDETACHED){
//        LOGI("============== GetEnv: not attached  ");
//        if ((*jvm)->AttachCurrentThread(jvm, &_env, NULL) != 0) {
//            LOGI(" ================================== Attach fail ! !!!  ");
//            return false;
//        }
//
//        LOGI("============== 回叫 接受 buffer  ");
//        callBackRecvBuff(_env, pos, data, len);
//    } else if(envStatus == JNI_OK){
//        callBackRecvBuff(_env, pos, data, len);
//    } else if(envStatus == JNI_EVERSION){
//        return false;
//    }
//
//    return true;
//}
//
//void callBackSendBuff(JNIEnv *_env,unsigned char*data, int len){
//
//    LOGI("==============callBackSendBuff  111");
//    jbyteArray dataArray =  (*_env) -> NewByteArray(_env, len);
//    LOGI("==============callBackSendBuff  222");
//    (*_env) -> SetByteArrayRegion(_env, dataArray, 0, len, (const jbyte *)(data));
//    LOGI("==============callBackSendBuff  333");
//    jbyte * jbyteP = (*_env) -> GetByteArrayElements(_env, dataArray, 0);
//
//    LOGI("==============callBackSendBuff  444");
//    jclass fastClass = (*_env) -> GetObjectClass(_env, gobal_obj);
//    LOGI("==============callBackSendBuff  555");
//    jmethodID jmethodID1 = (*_env) -> GetMethodID(_env, fastClass, "SendBuff", "([B)V");
//    LOGI("==============callBackSendBuff  666");
//    (*_env)->CallVoidMethod(gobal_obj, jmethodID1, dataArray);
//    LOGI("==============callBackSendBuff  777");
//    (*_env) ->ReleaseByteArrayElements(_env, dataArray, jbyteP,  JNI_ABORT);
//    LOGI("==============callBackSendBuff  888");
//
//}
//
//bool sendBuff(unsigned char*data, int len){
//
//    JNIEnv* _env;
//    int envStatus = (*jvm)->GetEnv(jvm,(void **)&_env, JNI_VERSION_1_6);
//    if(envStatus == JNI_EDETACHED){
//        LOGI(" ==============GetEnv: not attached  ");
//        if ((*jvm)->AttachCurrentThread(jvm, &_env, NULL) != 0) {
//            LOGI(" ================================== Attach fail ! !!!  ");
//            return false;
//        }
//
//        callBackSendBuff(_env, data, len);
//    } else if(envStatus == JNI_OK){
//        LOGI("============== 发送 buffer  ");
//        callBackSendBuff(_env, data, len);
//    } else if(envStatus == JNI_EVERSION){
//        LOGI(" ==================================   JNI Version not support !!!   ");
//        return false;
//    }
//
//    return true;
//}
//
//
///*
// * Class:     com_iyich_wallet_lib_jni_FastModeJNI
// * Method:    sendData
// * Signature: ([BI)Z
// */
//JNIEXPORT jboolean JNICALL Java_com_iyich_wallet_lib_jni_SecLinkJni_sendData
//        (JNIEnv *env, jobject jobj, jbyteArray data, jint len){
//
//    (*env) -> GetJavaVM(env, &jvm);
//    gobal_obj = (*env) -> NewGlobalRef(env, jobj);
//
//    unsigned char* buffer = (unsigned char *)((*env)->GetByteArrayElements(env, data, 0));
//    return onSendBlock(buffer, len, sendBuff);
//}
//
//
//
///*
// * Class:     com_iyich_wallet_lib_jni_FastModeJNI
// * Method:    onReceive
// * Signature: ([BI)Z
// */
//JNIEXPORT jboolean JNICALL Java_com_iyich_wallet_lib_jni_SecLinkJni_onReceive
//        (JNIEnv *env, jobject jobj, jbyteArray data, jint len){
//    gobal_obj = (*env) -> NewGlobalRef(env, jobj);
//    unsigned char* buffer = (unsigned char *)((*env)->GetByteArrayElements(env, data, 0));
//    return onMRecieve(buffer, len, sendBuff, receiveBuff);
//}



int len(char const *p)
{
    int i=0;
    while(*p++)i++;
    return i;
}

/*
 * Class:     com_iyich_wallet_lib_jni_SecLinkJni
 * Method:    veritySession
 */
JNIEXPORT jbyteArray JNICALL Java_com_iyich_wallet_lib_jni_SecLinkJni_veritySession
        (JNIEnv *env, jobject jobj, jbyteArray hPrivateKey, jbyteArray R, jbyteArray encryptedData, jbyteArray essionHash){

    char const * salt = "EDCH key salt9759";
    uint8_t S[65];

    uint8_t* cPriKey = (uint8_t *) (*env)->GetByteArrayElements(env, hPrivateKey, NULL);
    uint8_t* cR = (uint8_t *) (*env)->GetByteArrayElements(env, R, NULL);
    ecdh_multiply(&secp256k1, cPriKey, cR, S);//得到 S

    aes_decrypt_ctx ctxd;
    uint8_t aes_key[32];
    uint8_t ibuf[32], obuf[16], iv[16];
    uint8_t* encryptedDataP = (uint8_t *) (*env)->GetByteArrayElements(env, encryptedData, NULL);
    uint8_t* essionHashP = (uint8_t *) (*env)->GetByteArrayElements(env, essionHash, NULL);

    pbkdf2_hmac_sha256((uint8_t *)S, 65, salt, len(salt), 10, aes_key);//得到AES key
    aes_decrypt_key256(aes_key, &ctxd);
    memcpy(iv, encryptedDataP, 16);
    memcpy(ibuf, encryptedDataP + 16, 32);
    aes_cbc_decrypt(ibuf, obuf, 32, iv, &ctxd);

    jbyteArray aesDataArray =  (*env) -> NewByteArray(env, 32);
    (*env) -> SetByteArrayRegion(env, aesDataArray, 0, 32, aes_key);

    /*** sha 256 ***/
    SHA256_CTX ctx1;
    unsigned char buf2[SHA256_DIGEST_LENGTH];
    sha256_Init(&ctx1);
    sha256_Update(&ctx1, obuf, 32);
    sha256_Final(&ctx1, buf2);

    int pass = memcmp(essionHashP, buf2, SHA256_DIGEST_LENGTH);
    if(pass == 0){
        return aesDataArray;
    } else{
        return NULL;
    }


};

JNIEXPORT jstring JNICALL Java_com_iyich_wallet_lib_jni_SecLinkJni_getRsaPrivateKey(JNIEnv *env, jobject jobj){

    return  (*env) -> NewStringUTF(env, SERVER_KEY);
}

JNIEXPORT jstring JNICALL Java_com_iyich_wallet_lib_jni_SecLinkJni_getDevicePublicKey(JNIEnv *env, jobject jobj){

    return  (*env) -> NewStringUTF(env, DEVICE_PUBLIC_KEY);
}

