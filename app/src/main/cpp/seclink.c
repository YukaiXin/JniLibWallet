
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


#define LOG_TAG "kxyu_jni"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ARRAY_LENGTH 6

const char *RELEASE_SIGN = "3082033130820219a0030201020204596e28f5300d06092a864886f70d01010b05003049310b3009060355040613023836310a30080603550408130161310a30080603550407130161310a3008060355040a130161310a3008060355040b130161310a30080603550403130161301e170d3137303131373039303231365a170d3432303131313039303231365a3049310b3009060355040613023836310a30080603550408130161310a30080603550407130161310a3008060355040a130161310a3008060355040b130161310a3008060355040313016130820122300d06092a864886f70d01010105000382010f003082010a02820101009f1f3731ef4c65ccd6c4a7589eaffe813117d2112cc92279f41a22f210398baa2ddae52fd61c736b51b21c01d4a3233fd34b2b29365723bdb285bf0eddd043b7a9dd2829366974a690aa885b859a2d3fb272baf8c3ab94024f97117b6d6a68b74f2ed35daca41ef601a48c9f3393d92a4c3bb6f26152142e03290ef1d607361b0a2759479a7f0b94425bd885db49bcbb777f7dc10e7d3eff1fa4cc3080b4c8524ca6b761732100347b80d56a9bd5f6e7d503debe5c25c60194bd1c34c54f40172f2add9cf7e934aa7e64467c362d87fc91069fd29afc5e3445f609daf4fb99905c6ec17bea73252f6b264fdbb6963f5822997b36af9caccb2869a8b87a942df50203010001a321301f301d0603551d0e041604144aaa523ada5947919a2f7dbbe8cd3711b8dbb08e300d06092a864886f70d01010b050003820101008e6153b54104503b04a04d2746c35ce094688c2f05cd6f8c7edbcabb0d801a57c55f75930081294e63bbe27af5705511d8b7e5e263f0c6a9af58fd8c87fa43e22358c92ec4378ced89aa164f9770ebde94f865572bb846ce2cdf48ec5f6ddd1e4a733a5faca96244cd8e250cec6c0a16740e5bb7907db19d1db260806b4efd890c264ec59d46135b4f82077d3f233f5b349601b217f28d8392d90ae1fd5f462ec7e5889677bbd6c0054ea680b6dc9746077d8d536d7bc5a39dbb3074658c986a8ca14b6110599808d6f4532e32e179af558df1305880d97599d23eda5f25b0b82f091cfd702d187cfbdffc3f5bbbb9f17ae660683b07c566df5622d6e19462f8";
static jboolean auth = JNI_FALSE;

char *SERVER_KEY =  "MIICdQIBADANBgkqhkiG9w0BAQEFAASCAl8wggJbAgEAAoGBAJDfexhe6i0o3q7/jMRkJ3w5UWgQ\n"
                    "dqgXFUlz3cVBmFWPIWLSvil9TuhbS/TmWTlUpdizkq8nuF2hU+BsLJUkKpOSI5cWiWgf79TCrw3g\n"
                    "O0VTLa3rxCuXFT3xaPIEnJSLK7ZKK4xm+mWWkgQgZS8spm6GWw29PsJMb5lQEE2APcnJAgMBAAEC\n"
                    "gYBUY0lRzd/8xETKjSNTNr/lVYH86zIjAactrSdugqIYnJIm/Xk1s79mC+A33IH2qj8c9qUoECwN\n"
                    "pi60mG5zxahif1Vt1WQaZSEz/NyfHhtZo6phEIkG7oxOki3gpVy8FCG92lEOmkz9/F5aHMi6nWvA\n"
                    "Vmq4MLz5Hc5FqE/1yfK67QJBAOe3lGd3eJbvb34fYpA/44PK8mXGFt0c11eHnwIoF3SgMGRzAjy5\n"
                    "7xEHp3dgn2lgjd2nBFRVnBFBYlGkD04fHTcCQQCgDhQ4nc2l/MYIzOGefgCgkj1FWLBSKsuW7ull\n"
                    "VPRJkmWRSWcCj9wPlJQf0lg8hxTL9MPrahDQVqzrwYkOFdD/AkAKM02oJF09n7qzvaP0H6vfUF7N\n"
                    "ViQJhcyBAOpiCOvHaYnX0ixGChVZFZIssicyV1rHaCPLpp0hMVSnrWYzFTGPAkAbtPQ5LNAZyN07\n"
                    "TsRL3IIdSdODeqvjPXdK5IbQvNkhF0+vy+YiImc050KRE6ekqrml6PQ1f4Hznt2GXlVsc8hfAkAR\n"
                    "DO06vvqdIt1osz7PFFX9Ms7J5Q2U8Bt2v9i0xPmahi25MKb+Nyv/Fdd2syt+b2YP+eAkO6OyUXqe\n"
                    "dGz65rwC";
char *DEVICE_PUBLIC_KEY = "0220b4c720870cc6f5e4d416417fdf91d43e08c19ccc1321552f41a97983b00216";
char *KEY_MODEL_ARRAYS[] = {"036f3dc1195204f9ef94f39b3c8f0a9f82cde6e24324132fab9604beb78c744e43", "024e9c342b05f40b7fee06d21010c37ec36a599d6941394d20c63d20fb22fa282f",
                            "0293466e61ecc3398de8038399157e45775b93881836fc4cc51d1cc0f34651559e", "035f7fc950ede95b0c365a44d7be0cffd96c1600da147dacc87320e33308c24564",
                            "02600744be5ad4cc0864b4da8c1a92870ec4e09f1bbf2f45d31a056de6fdadbe1a", "0247f59687e6453957b0b7b2881c4dcf2999fbf69470b8e225e6ddf1bd81b84e4a"};



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
 * Signature: ([B[B[B[B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_iyich_wallet_lib_jni_SecLinkJni_veritySession
        (JNIEnv *env, jclass class, jbyteArray hPrivateKey, jbyteArray R, jbyteArray encryptedData, jbyteArray essionHash){

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


JNICALL jboolean Java_com_iyich_wallet_lib_jni_SecLinkJni_initLib(JNIEnv *env, jclass class, jobject contextObject) {

    jclass binderClass = (*env)->FindClass(env ,"android/os/Binder");

    jclass contextClass = (*env)->FindClass(env, "android/content/Context");
    jclass signatureClass = (*env)->FindClass(env,"android/content/pm/Signature");
    jclass packageNameClass = (*env)->FindClass(env, "android/content/pm/PackageManager");
    jclass packageInfoClass = (*env)->FindClass(env, "android/content/pm/PackageInfo");

    jmethodID getPackageManagerId = (*env)->GetMethodID(env, contextClass, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jmethodID getPackageNameId = (*env)->GetMethodID(env, contextClass, "getPackageName", "()Ljava/lang/String;");
    jmethodID signToStringId = (*env)->GetMethodID(env, signatureClass, "toCharsString", "()Ljava/lang/String;");
    jmethodID getPackageInfoId = (*env)->GetMethodID(env, packageNameClass, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jmethodID getRunningPackageName = (*env)->GetMethodID(env, packageNameClass, "getNameForUid", "(I)Ljava/lang/String;");
    jmethodID getUid = (*env)->GetStaticMethodID(env, binderClass, "getCallingUid", "()I");

    jint uid = (*env)->CallStaticIntMethod(env, binderClass, getUid);


    jobject packageManagerObject = (*env)->CallObjectMethod(env, contextObject, getPackageManagerId);
    jstring packNameString = (jstring) (*env)->CallObjectMethod(env, contextObject, getPackageNameId);
    jobject packageInfoObject = (*env)->CallObjectMethod(env, packageManagerObject, getPackageInfoId, packNameString, 64);
    jfieldID signaturefieldID = (*env)->GetFieldID(env, packageInfoClass, "signatures", "[Landroid/content/pm/Signature;");
    jobjectArray signatureArray = (jobjectArray) (*env)->GetObjectField(env, packageInfoObject, signaturefieldID);
    jobject signatureObject = (*env)->GetObjectArrayElement(env, signatureArray, 0);

    jstring mRunningPackageName = (jstring) (*env)->CallObjectMethod(packageManagerObject, getRunningPackageName, uid);

    if (mRunningPackageName) {
        const char *runPackageName = (*env)->GetStringUTFChars(env, mRunningPackageName, 0);
        LOGI("rPackageName:%s", runPackageName);
        if (strcmp(runPackageName, "reazerdp.com.mytestso") != 0) {
            return JNI_FALSE;
        }
        (*env)->ReleaseStringUTFChars(env, mRunningPackageName, runPackageName);
    } else {
        LOGI("rPackageName:%s", "is null");
        return JNI_FALSE;
    }

    jstring signatureStr = (jstring) (*env)->CallObjectMethod(env, signatureObject, signToStringId);
    const char *signStrng = (*env)->GetStringUTFChars(env, signatureStr, 0);

    (*env)->DeleteLocalRef(env, binderClass);
    (*env)->DeleteLocalRef(env, contextClass);
    (*env)->DeleteLocalRef(env, signatureClass);
    (*env)->DeleteLocalRef(env, packageNameClass);
    (*env)->DeleteLocalRef(env, packageInfoClass);


    if (strcmp(signStrng, RELEASE_SIGN) == 0) {
        (*env)->ReleaseStringUTFChars(env, signatureStr, signStrng);
        auth = JNI_TRUE;
        return JNI_TRUE;
    } else {
        auth = JNI_FALSE;
        return JNI_FALSE;
    }
}

JNIEXPORT jstring JNICALL Java_com_iyich_wallet_lib_jni_SecLinkJni_getRsaPrivateKey(JNIEnv *env, jclass class){
    if(auth == JNI_TRUE){
        return  (*env) -> NewStringUTF(env, SERVER_KEY);
    } else{
        return  (*env) -> NewStringUTF(env, "12112");
    }
}

JNIEXPORT jstring JNICALL Java_com_iyich_wallet_lib_jni_SecLinkJni_getDevicePublicKey(JNIEnv *env, jclass class){
    if(auth == JNI_TRUE){
        return  (*env) -> NewStringUTF(env, DEVICE_PUBLIC_KEY);
    } else{
        return  (*env) -> NewStringUTF(env, "123123123");
    }
}

/*
 * Class:     com_iyich_wallet_lib_jni_SecLinkJni
 * Method:    getModelKey
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_iyich_wallet_lib_jni_SecLinkJni_getModelKey(JNIEnv *env, jclass class){

    jclass objClass = (*env)->FindClass(env, "java/lang/String");
    jobjectArray modelKeys= (*env)->NewObjectArray(env, (jsize)ARRAY_LENGTH, objClass, 0);
    jstring jstr;
    for (int i = 0; i < ARRAY_LENGTH; i ++) {
        jstr = (*env) ->NewStringUTF(env, KEY_MODEL_ARRAYS[i]);
        (*env) -> SetObjectArrayElement(env, modelKeys, i, jstr);
    }

    if(auth == JNI_TRUE){
        return  modelKeys;
    } else{
        return  NULL;
    }
}

// 结构体，分别是java层的函数名称，签名，对应的函数指针
static JNINativeMethod gMethods[] ={
        {"getRsaPrivateKey", "()Ljava/lang/String", (void*)Java_com_iyich_wallet_lib_jni_SecLinkJni_getRsaPrivateKey},
        {"getDevicePublicKey", "()Ljava/lang/String", (void*)Java_com_iyich_wallet_lib_jni_SecLinkJni_getDevicePublicKey},
        {"veritySession", "([B[B[B[B)[B", (void*)Java_com_iyich_wallet_lib_jni_SecLinkJni_veritySession},
        {"getModelKey", "()[Ljava/lang/String", (void*)Java_com_iyich_wallet_lib_jni_SecLinkJni_getModelKey}
};


jint JNI_OnLoad(JavaVM* vm, void* reserved){
    JNIEnv* env = NULL;
    jint result = -1;

    LOGI(" kxyu==================================   JNI_OnLoad  1111111 !!!   ");

    if ((*vm)->GetEnv(vm,(void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

//    /*
//     * 如果要注册，只需要两步，
//     *    首先FindClass，
//     *    然后RegisterNatives
//     */
//    char className[20] = {"dynamic/HelloWorld"};
//
//    jclass clazz = (*env)->FindClass(env,(const char*)className);
//    if((*env)->RegisterNatives(env, clazz, gMethods, 1)< 0) {
//        return -1;
//    }

    //一定要返回版本号，否则会出错。
    result = JNI_VERSION_1_6;
    return result;
}

