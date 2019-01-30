
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

const char *RELEASE_SIGN = "308203b53082029da00302010202042654b5e3300d06092a864886f70d01010b050030818a311530130603550406130c676f72696c6c57616c6c6574311530130603550408130c676f72696c6c57616c6c6574311530130603550407130c676f72696c6c57616c6c657431153013060355040a130c676f72696c6c57616c6c657431153013060355040b130c676f72696c6c57616c6c6574311530130603550403130c676f72696c6c57616c6c6574301e170d3139303132343033343433345a170d3434303131383033343433345a30818a311530130603550406130c676f72696c6c57616c6c6574311530130603550408130c676f72696c6c57616c6c6574311530130603550407130c676f72696c6c57616c6c657431153013060355040a130c676f72696c6c57616c6c657431153013060355040b130c676f72696c6c57616c6c6574311530130603550403130c676f72696c6c57616c6c657430820122300d06092a864886f70d01010105000382010f003082010a02820101008bdcafa9617d5e5e0dc7e7eea8513c8c779522c28704cb8e03911b4f0991f19351dfc3ce199b1e5150c65d381ca1434d8c9005c700d1c40463c10a9d9d8adcbddde60f42ee3b2df82b5037364de2be03f2fe8198766b0882f676c6f9fa59a8ab5fc6795205efa99ae4ca9caaf8bca840bb1f5082309690c8981aca97188f8296d95dfcd6080806141305f25de4eeac3581f11acff477b20d42a2f7bf3ea61895672e5c4af5e42b38172ab6baf625e3abce79d32bb305a836207e4c30d62a73c9e9b32fbba7187d607fd538207547a81f5085187ba8c45e3f3ea1fb44497e79195e4c72079b54ee633e0ae35cecad20082290ff261f66cb9e63e5b2a6d51a92990203010001a321301f301d0603551d0e04160414bcb630e0d8463b7375020b3bf79d093f8c289139300d06092a864886f70d01010b05000382010100392bbaf1292f1dbd6f02790de8e77ef4c3c5b5c7a4acb720c822c4c298bdf9b9d803d60b2ca7963c487114ab3e5d5b082927cc452ee87ed323ef9f145cb3eeca4396c2b59812a2358c1e10e6df69121e193a21bbc4a235d0eea10d123cc0ee059aa52236662de199be59cd3a4b932c18893ff7c4d51f805bb805382479660d316d321e551aafb3a301ed679ea1674cbabcfafb46b54ba58150467c789fb871628b367f6f382e49b7f4d39bec54ebe7fe33dbfb820920c6d4bb1dbc540a5380f7b65865e90571429ba36ec018b0c009b06d326017cbcc770ceb91cb358a44c3ba105a6b50c80803e10e08a41fed040662af39c2f867265c921362a0b09f6d1e1a";
static jboolean auth = JNI_FALSE;
char *PAKEAGE_NAME = "com.yiku.wallet";
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
char *DEVICE_PUBLIC_KEY = "032cfc710a03b6e4a6141c11926cc24ae430f81c33352f2664c89f289cdf55cdad";
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

//    jclass binderClass = (*env)->FindClass(env ,"android/os/Binder");

    LOGI(" sign :  1111111 ");
//    jclass context_class = (*env)->GetObjectClass(contextObject);
    jclass contextClass = (*env)->FindClass(env, "android/content/Context");
    jclass signatureClass = (*env)->FindClass(env,"android/content/pm/Signature");
    jclass packageNameClass = (*env)->FindClass(env, "android/content/pm/PackageManager");
    jclass packageInfoClass = (*env)->FindClass(env, "android/content/pm/PackageInfo");

    LOGI(" sign :  2222222 ");
    jmethodID getPackageManagerId = (*env)->GetMethodID(env, contextClass, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jmethodID getPackageNameId = (*env)->GetMethodID(env, contextClass, "getPackageName", "()Ljava/lang/String;");
    jmethodID signToStringId = (*env)->GetMethodID(env, signatureClass, "toCharsString", "()Ljava/lang/String;");
    jmethodID getPackageInfoId = (*env)->GetMethodID(env, packageNameClass, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jmethodID getRunningPackageName = (*env)->GetMethodID(env, packageNameClass, "getNameForUid", "(I)Ljava/lang/String;");
//    jmethodID getUid = (*env)->GetStaticMethodID(env, binderClass, "getCallingUid", "()I");

    LOGI(" sign :  33333333 ");
//    jint uid = (*env)->CallStaticIntMethod(env, binderClass, getUid);

//    LOGI(" sign :  UID ： %d ", uid);

    jobject packageManagerObject = (*env)->CallObjectMethod(env, contextObject, getPackageManagerId);
    jstring packNameString = (jstring) (*env)->CallObjectMethod(env, contextObject, getPackageNameId);
    jobject packageInfoObject = (*env)->CallObjectMethod(env, packageManagerObject, getPackageInfoId, packNameString, 64);
    jfieldID signaturefieldID = (*env)->GetFieldID(env, packageInfoClass, "signatures", "[Landroid/content/pm/Signature;");
    jobjectArray signatureArray = (jobjectArray) (*env)->GetObjectField(env, packageInfoObject, signaturefieldID);
    jobject signatureObject = (*env)->GetObjectArrayElement(env, signatureArray, 0);

//    jstring mRunningPackageName = (jstring) (*env)->CallObjectMethod(env, packageManagerObject, getRunningPackageName, uid);



//    if (mRunningPackageName) {
        const char *runPackageName = (*env)->GetStringUTFChars(env, packNameString, 0);
        LOGI("rPackageName:%s", runPackageName);
        if (strcmp(runPackageName, PAKEAGE_NAME) != 0) {
            LOGI(" jni 验证 包名不相同 ");
            return JNI_FALSE;
        } else{
            LOGI(" jni 验证 包名相同 ");
        }
        (*env)->ReleaseStringUTFChars(env, packNameString, runPackageName);

//    } else {
//        LOGI("rPackageName:%s", "is null");
//        return JNI_FALSE;
//    }

    jstring signatureStr = (jstring) (*env)->CallObjectMethod(env, signatureObject, signToStringId);

    const char *signStrng = (*env)->GetStringUTFChars(env, signatureStr, 0);

//    (*env)->DeleteLocalRef(env, binderClass);
    (*env)->DeleteLocalRef(env, contextClass);
    (*env)->DeleteLocalRef(env, signatureClass);
    (*env)->DeleteLocalRef(env, packageNameClass);
    (*env)->DeleteLocalRef(env, packageInfoClass);

    if (strcmp(signStrng, RELEASE_SIGN) == 0) {
        (*env)->ReleaseStringUTFChars(env, signatureStr, signStrng);
        LOGI(" jni 验证   验证签名成功 ");
        auth = JNI_TRUE;
        return JNI_TRUE;
    } else {
        LOGI(" jni 验证  验证签名失败 ");
        auth = JNI_FALSE;
        return JNI_FALSE;
    }
}

JNIEXPORT jstring JNICALL Java_com_iyich_wallet_lib_jni_SecLinkJni_getRsaPrivateKey(JNIEnv *env, jclass class){
    if(auth == JNI_TRUE){
        return  (*env) -> NewStringUTF(env, SERVER_KEY);
    } else{
        return  (*env) -> NewStringUTF(env, "key Error");
    }
}

JNIEXPORT jstring JNICALL Java_com_iyich_wallet_lib_jni_SecLinkJni_getDevicePublicKey(JNIEnv *env, jclass class){
    if(auth == JNI_TRUE){
        return  (*env) -> NewStringUTF(env, DEVICE_PUBLIC_KEY);
    } else{
        return  (*env) -> NewStringUTF(env, "key Error");
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

    LOGI(" kxyu==================================   JNI_OnLoad  22222 !!!   ");

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

