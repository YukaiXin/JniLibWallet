//
// Created by 大灯泡 on 2017/1/16.
//

#include <jni.h>
#include <string.h>
#include <assert.h>
#include <android/log.h>

#define   NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#define   LOG_TAG    "MyNativeLib"
#define   LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define   LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define PACKATE_PATH "reazerdp/com/mynative/MyNative"
const char *RELEASE_SIGN = "3082033130820219a0030201020204596e28f5300d06092a864886f70d01010b05003049310b3009060355040613023836310a30080603550408130161310a30080603550407130161310a3008060355040a130161310a3008060355040b130161310a30080603550403130161301e170d3137303131373039303231365a170d3432303131313039303231365a3049310b3009060355040613023836310a30080603550408130161310a30080603550407130161310a3008060355040a130161310a3008060355040b130161310a3008060355040313016130820122300d06092a864886f70d01010105000382010f003082010a02820101009f1f3731ef4c65ccd6c4a7589eaffe813117d2112cc92279f41a22f210398baa2ddae52fd61c736b51b21c01d4a3233fd34b2b29365723bdb285bf0eddd043b7a9dd2829366974a690aa885b859a2d3fb272baf8c3ab94024f97117b6d6a68b74f2ed35daca41ef601a48c9f3393d92a4c3bb6f26152142e03290ef1d607361b0a2759479a7f0b94425bd885db49bcbb777f7dc10e7d3eff1fa4cc3080b4c8524ca6b761732100347b80d56a9bd5f6e7d503debe5c25c60194bd1c34c54f40172f2add9cf7e934aa7e64467c362d87fc91069fd29afc5e3445f609daf4fb99905c6ec17bea73252f6b264fdbb6963f5822997b36af9caccb2869a8b87a942df50203010001a321301f301d0603551d0e041604144aaa523ada5947919a2f7dbbe8cd3711b8dbb08e300d06092a864886f70d01010b050003820101008e6153b54104503b04a04d2746c35ce094688c2f05cd6f8c7edbcabb0d801a57c55f75930081294e63bbe27af5705511d8b7e5e263f0c6a9af58fd8c87fa43e22358c92ec4378ced89aa164f9770ebde94f865572bb846ce2cdf48ec5f6ddd1e4a733a5faca96244cd8e250cec6c0a16740e5bb7907db19d1db260806b4efd890c264ec59d46135b4f82077d3f233f5b349601b217f28d8392d90ae1fd5f462ec7e5889677bbd6c0054ea680b6dc9746077d8d536d7bc5a39dbb3074658c986a8ca14b6110599808d6f4532e32e179af558df1305880d97599d23eda5f25b0b82f091cfd702d187cfbdffc3f5bbbb9f17ae660683b07c566df5622d6e19462f8";
static jboolean auth = JNI_FALSE;

JNICALL jboolean native_initLib(JNIEnv *env, jobject obj, jobject contextObject) {

    jclass binderClass = env->FindClass("android/os/Binder");

    jclass contextClass = env->FindClass("android/content/Context");
    jclass signatureClass = env->FindClass("android/content/pm/Signature");
    jclass packageNameClass = env->FindClass("android/content/pm/PackageManager");
    jclass packageInfoClass = env->FindClass("android/content/pm/PackageInfo");

    jmethodID getPackageManagerId = env->GetMethodID(contextClass, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jmethodID getPackageNameId = env->GetMethodID(contextClass, "getPackageName", "()Ljava/lang/String;");
    jmethodID signToStringId = env->GetMethodID(signatureClass, "toCharsString", "()Ljava/lang/String;");
    jmethodID getPackageInfoId = env->GetMethodID(packageNameClass, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jmethodID getRunningPackageName = env->GetMethodID(packageNameClass, "getNameForUid", "(I)Ljava/lang/String;");
    jmethodID getUid = env->GetStaticMethodID(binderClass, "getCallingUid", "()I");

    jint uid = env->CallStaticIntMethod(binderClass, getUid);


    jobject packageManagerObject = env->CallObjectMethod(contextObject, getPackageManagerId);
    jstring packNameString = (jstring) env->CallObjectMethod(contextObject, getPackageNameId);
    jobject packageInfoObject = env->CallObjectMethod(packageManagerObject, getPackageInfoId, packNameString, 64);
    jfieldID signaturefieldID = env->GetFieldID(packageInfoClass, "signatures", "[Landroid/content/pm/Signature;");
    jobjectArray signatureArray = (jobjectArray) env->GetObjectField(packageInfoObject, signaturefieldID);
    jobject signatureObject = env->GetObjectArrayElement(signatureArray, 0);

    jstring mRunningPackageName = (jstring) env->CallObjectMethod(packageManagerObject, getRunningPackageName, uid);

    if (mRunningPackageName) {
        const char *runPackageName = env->GetStringUTFChars(mRunningPackageName, 0);
        LOGI("rPackageName:%s", runPackageName);
        if (strcmp(runPackageName, "reazerdp.com.mytestso") != 0) {
            return JNI_FALSE;
        }
        env->ReleaseStringUTFChars(mRunningPackageName, runPackageName);
    } else {
        LOGE("rPackageName:%s", "is null");
        return JNI_FALSE;
    }

    jstring signatureStr = (jstring) env->CallObjectMethod(signatureObject, signToStringId);
    const char *signStrng = env->GetStringUTFChars(signatureStr, 0);

    env->DeleteLocalRef(binderClass);
    env->DeleteLocalRef(contextClass);
    env->DeleteLocalRef(signatureClass);
    env->DeleteLocalRef(packageNameClass);
    env->DeleteLocalRef(packageInfoClass);


    if (strcmp(signStrng, RELEASE_SIGN) == 0) {
        env->ReleaseStringUTFChars(signatureStr, signStrng);
        auth = JNI_TRUE;
        return JNI_TRUE;
    } else {
        auth = JNI_FALSE;
        return JNI_FALSE;
    }
}

JNICALL jstring native_getPassword(JNIEnv *env, jobject obj) {
    if (auth) {
        return env->NewStringUTF("返回了一个测试密码:123");
    } else{
        return env->NewStringUTF("验证不通过，不返回密码");
    }
}

static JNINativeMethod nativeMethods[] = {
        {"initLib",     "(Landroid/content/Context;)Z", (void *) native_initLib},
        {"getPassword", "()Ljava/lang/String;",         (void *) native_getPassword}
};

static int registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *gMethods, int numMethods) {
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }

    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

static int registerNatives(JNIEnv *env) {
    return registerNativeMethods(env, PACKATE_PATH, nativeMethods, NELEM(nativeMethods));
}


extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = -1;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("JNI_ONLOAD:%s", "failed");
        return -1;
    }
    assert(env != NULL);

    if (!registerNatives(env)) {//注册
        return -1;
    }

/* success -- return valid version number */

    result = JNI_VERSION_1_4;

    return result;
}//
// Created by apple on 2019/1/29.
//

