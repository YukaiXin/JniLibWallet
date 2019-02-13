
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
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ARRAY_LENGTH 6

const char *RELEASE_SIGN = "3082033f30820227a00302010202044c9e908d300d06092a864886f70d01010b050030503111300f060355040713087368616e67686169310d300b060355040a130479696b7531153013060355040b130c676f72696c6c77616c6c6574311530130603550403130c676f72696c6c77616c6c6574301e170d3139303231333036333630355a170d3434303230373036333630355a30503111300f060355040713087368616e67686169310d300b060355040a130479696b7531153013060355040b130c676f72696c6c77616c6c6574311530130603550403130c676f72696c6c77616c6c657430820122300d06092a864886f70d01010105000382010f003082010a028201010085883ad469f938e56ddf02c5a029aa5ddf746d4a4f408885add10ee0d9adaa444f68e64273c6e4f2fe367d13b9bf53fe662ae5e215e275cf99b7965f603457df92464fe1c2ac01b1267d84bb1e2bf037846208d9576a1040b5992f5fd4b83bd12e09d129317a39b1dc82759606c085133f34413effcb3f769eb136570d11f5f21b14a41f7326de93fa08bb7f36d49024d032bdfc8b34ff22a2a35d34964d6fb99852afc3e29211352adc4ba34c44ba6bc2d6fc3852c1305e223b8ea281337269a5217610d17b605a9e586aeef1870e503270f815c12b7989a80474661a5e6591136414106c511725c2c32c4faba9ecfdd940905268747bf3090de80cf7ec92690203010001a321301f301d0603551d0e04160414086372525c4f8b733b13fafe3e94574771621d85300d06092a864886f70d01010b0500038201010030898ac958b1f6656c836511d0d96b1be0fab93cbee828d44aee86bb0d87ce5467d348d189a742464eb2b3671c6c4e9529989334fdee483d029bf72e21f200a59cac8acaa42c7279a62407bcd1a37c3cfca50d874ce3a85825d4a54ba837723624142e5985222305b7c5fb089e5355cdcb523ce255cb3a9c6fea6afad0c4e740430fa2e911d3a4aa769d5d23011553d8be3e4dff11bbb3826f817fec50a35deaa29af7287f0c05cd0c579d39ac8b34a67d6cbded7f2ef65ef3a37aae727aec14ac42d68d3e3228327e1f8a7a6056bdd19ca2186430601588f8bd0a146908e6b020f3386f6d11f52d3b4adbb705d80794d9d79e4e3241b02aa23cfeb3b02849f9";
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

    jclass contextClass = (*env)->FindClass(env, "android/content/Context");
    jclass signatureClass = (*env)->FindClass(env,"android/content/pm/Signature");
    jclass packageNameClass = (*env)->FindClass(env, "android/content/pm/PackageManager");
    jclass packageInfoClass = (*env)->FindClass(env, "android/content/pm/PackageInfo");

    jmethodID getPackageManagerId = (*env)->GetMethodID(env, contextClass, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jmethodID getPackageNameId = (*env)->GetMethodID(env, contextClass, "getPackageName", "()Ljava/lang/String;");
    jmethodID signToStringId = (*env)->GetMethodID(env, signatureClass, "toCharsString", "()Ljava/lang/String;");
    jmethodID getPackageInfoId = (*env)->GetMethodID(env, packageNameClass, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");

    jobject packageManagerObject = (*env)->CallObjectMethod(env, contextObject, getPackageManagerId);
    jstring packNameString = (jstring) (*env)->CallObjectMethod(env, contextObject, getPackageNameId);
    jobject packageInfoObject = (*env)->CallObjectMethod(env, packageManagerObject, getPackageInfoId, packNameString, 64);
    jfieldID signaturefieldID = (*env)->GetFieldID(env, packageInfoClass, "signatures", "[Landroid/content/pm/Signature;");
    jobjectArray signatureArray = (jobjectArray) (*env)->GetObjectField(env, packageInfoObject, signaturefieldID);
    jobject signatureObject = (*env)->GetObjectArrayElement(env, signatureArray, 0);

        const char *runPackageName = (*env)->GetStringUTFChars(env, packNameString, 0);
        if (strcmp(runPackageName, PAKEAGE_NAME) != 0) {
//            LOGI(" jni 验证 包名不相同 ");
            return JNI_FALSE;
        } else{
//            LOGI(" jni 验证 包名相同 ");
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
//        LOGI(" jni 验证   验证签名成功 ");
        auth = JNI_TRUE;
        return JNI_TRUE;
    } else {
//        LOGI(" jni 验证  验证签名失败 ");
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

