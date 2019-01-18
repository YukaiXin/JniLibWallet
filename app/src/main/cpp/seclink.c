
#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <android/log.h>
#include <string.h>
#include "sec/ecdsa.h"
#include "sec/secp256k1.h"
#include "sec/pbkdf2.h"
#include "sec/sing/aes/aes.h"


#define LOG_TAG "System.out"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define FROMHEX_MAXLEN 512



int len(char const *p)
{
    int i=0;
    while(*p++)i++;
    return i;
}

void hex_str(unsigned char *inchar, unsigned int len, unsigned char *outtxt)
{
    unsigned char hbit,lbit;
    unsigned int i;
    for(i=0;i<len;i++)
    {
        hbit = (*(inchar+i)&0xf0)>>4;
        lbit = *(inchar+i)&0x0f;
        if (hbit>9) outtxt[2*i]='A'+hbit-10;
        else outtxt[2*i]='0'+hbit;
        if (lbit>9) outtxt[2*i+1]='A'+lbit-10;
        else    outtxt[2*i+1]='0'+lbit;
    }
    outtxt[2*i] = 0;
}



const uint8_t *fromhex(const char *str)
{
    static uint8_t buf[FROMHEX_MAXLEN];
    size_t len = strlen(str) / 2;
    if (len > FROMHEX_MAXLEN) len = FROMHEX_MAXLEN;
    for (size_t i = 0; i < len; i++) {
        uint8_t c = 0;
        if (str[i * 2] >= '0' && str[i*2] <= '9') c += (str[i * 2] - '0') << 4;
        if ((str[i * 2] & ~0x20) >= 'A' && (str[i*2] & ~0x20) <= 'F') c += (10 + (str[i * 2] & ~0x20) - 'A') << 4;
        if (str[i * 2 + 1] >= '0' && str[i * 2 + 1] <= '9') c += (str[i * 2 + 1] - '0');
        if ((str[i * 2 + 1] & ~0x20) >= 'A' && (str[i * 2 + 1] & ~0x20) <= 'F') c += (10 + (str[i * 2 + 1] & ~0x20) - 'A');
        buf[i] = c;
    }
    return buf;
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

    pbkdf2_hmac_sha256((uint8_t *)S, 65, salt, len(salt), 10, aes_key);//得到AES key
    aes_decrypt_key256(aes_key, &ctxd);
    memcpy(iv, encryptedDataP, 16);
    memcpy(ibuf, encryptedDataP + 16, 32);
    aes_cbc_decrypt(ibuf, obuf, 32, iv, &ctxd);


    jbyteArray aesDataArray =  (*env) -> NewByteArray(env, 32);
    (*env) -> SetByteArrayRegion(env, aesDataArray, 0, 32, aes_key);
    jbyte * aesJbyteP = (*env)->GetByteArrayElements(env, aesDataArray, 0);

    jbyteArray dataArray =  (*env) -> NewByteArray(env, 32);
    (*env) -> SetByteArrayRegion(env, dataArray, 0, 32, obuf);
    jbyte * jbyteP = (*env)->GetByteArrayElements(env, dataArray, 0);

    jclass fastClass = (*env) ->GetObjectClass(env, jobj);

    jmethodID jmethodID1 = (*env)->GetMethodID(env, fastClass, "setAesKey", "([B[B)V");
    (*env)->CallVoidMethod(env,jobj, jmethodID1, aesDataArray, dataArray);

    (*env) ->ReleaseByteArrayElements(env, aesDataArray, aesJbyteP, JNI_ABORT);
    (*env) ->ReleaseByteArrayElements(env, dataArray, jbyteP, JNI_ABORT);


    //TODO: sha256(obuf) ==  essionHash


//    sha256_Init()


    return NULL;
};

