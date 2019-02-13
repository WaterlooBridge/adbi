#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>

#include <jni.h>
#include <stdlib.h>
#include <android/log.h>

#define LOG_TAG "JNITag"
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)

// this file is going to be compiled into a thumb mode binary

void __attribute__ ((constructor)) my_init(void);

jobject getApplication(JNIEnv *env) {
    jobject application = NULL;
    jclass activity_thread_clz = (*env)->FindClass(env, "android/app/ActivityThread");
    if (activity_thread_clz != NULL) {
        jmethodID get_Application = (*env)->GetStaticMethodID(env, activity_thread_clz,
                                                           "currentActivityThread",
                                                           "()Landroid/app/ActivityThread;");
        if (get_Application != NULL) {
            jobject currentActivityThread = (*env)->CallStaticObjectMethod(env, activity_thread_clz,
                                                                        get_Application);
            jmethodID getal = (*env)->GetMethodID(env, activity_thread_clz, "getApplication",
                                               "()Landroid/app/Application;");
            application = (*env)->CallObjectMethod(env, currentActivityThread, getal);
        }
        return application;
    }
    return application;
}


jstring getPackageName(JNIEnv *env) {
    jobject context = getApplication(env);
    if (context == NULL) {
        LOGE("context is null!");
        return NULL;
    }
    jclass activity = (*env)->GetObjectClass(env, context);
    jmethodID methodId_pack = (*env)->GetMethodID(env, activity, "getPackageName", "()Ljava/lang/String;");
    jstring name_str = (*env)->CallObjectMethod(env, context, methodId_pack);
    return name_str;
}

char* jstringToChar(JNIEnv* env, jstring jstr) {
    char* rtn = NULL;
    jclass clsstring = (*env)->FindClass(env, "java/lang/String");
    jstring strencode = (*env)->NewStringUTF(env, "utf-8");
    jmethodID mid = (*env)->GetMethodID(env,clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) (*env)->CallObjectMethod(env, jstr, mid, strencode);
    jsize alen = (*env)->GetArrayLength(env, barr);
    jbyte* ba = (*env)->GetByteArrayElements(env, barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char*) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    (*env)->ReleaseByteArrayElements(env, barr, ba, 0);
    return rtn;
}

void my_init(void)
{
	LOGD("inject dex started\n");
    const char* dexPath;
    char* dexOptDir;
    const char* className;
    const char* methodName;
    const char* packageName;

    JNIEnv *(*getJNIEnv)();
    void *handle = dlopen( "system/lib/libandroid_runtime.so", RTLD_NOW );
    getJNIEnv = dlsym( handle, "_ZN7android14AndroidRuntime9getJNIEnvEv" );
    if ( !getJNIEnv ) {
        LOGE("can not find getJNIEnv!");
        return;
    }

    JNIEnv *env = getJNIEnv();

    jint ver;
    ver = (*env)->GetVersion( env );
    switch ( ver ) {
    
    case 0x00010001:
    LOGD("JNI version is JNI_VERSION_1_1");break;

    case 0x00010002:
    LOGD("JNI version is JNI_VERSION_1_2");break;

    case 0x00010004:
    LOGD("JNI version is JNI_VERSION_1_4");break;

    case 0x00010006:
    LOGD("JNI version is JNI_VERSION_1_6");break;

    default:
    LOGD("Unknown JNI_VERSION:0x%x",ver);break;
    }

    jclass stringClass, classLoaderClass, dexClassLoaderClass, targetClass;
    jmethodID getSystemClassLoaderMethod, dexClassLoaderContructor, loadClassMethod, targetMethod;
    jobject systemClassLoaderObject, dexClassLoaderObject;
    jstring dexPathString, dexOptDirString, classNameString, tmpString;    
    jobjectArray stringArray;

    LOGD("-------------- now begin dex injection --------------");

    /* set dex dir */
    LOGD("step1: setting dex dir and opt dir...");
    char* temp = "/data/data/%s/cache";
    packageName = jstringToChar(env, getPackageName(env));
    dexPath = "/sdcard/com.zhenl.demo/classes.dex";
    dexOptDir = (char*) malloc(strlen(temp) + strlen(packageName));
    sprintf(dexOptDir, temp, packageName);
    LOGD("dexOptDir: %s\n", dexOptDir);
    className = "com.zhenl.demo.Main";
    methodName = "init"; 
    LOGD("step1 finished!\n");


    /* Get SystemClasLoader */
    LOGD("step2: getting systemClassLoader method and invoke it to get systemClassLoader obeject...");
    stringClass = (*env)->FindClass(env, "java/lang/String");//获取String类
    classLoaderClass = (*env)->FindClass(env, "java/lang/ClassLoader");//获取classLoader类
    getSystemClassLoaderMethod = (*env)->GetStaticMethodID(env, classLoaderClass, "getSystemClassLoader", "()Ljava/lang/ClassLoader;");//获取classLoader中的getSystemClassLoader静态方法
    systemClassLoaderObject = (*env)->CallStaticObjectMethod(env, classLoaderClass, getSystemClassLoaderMethod);//调用getSystemClassLoader静态方法来获取所属对象systemClassLoaderObject
    if (! systemClassLoaderObject) {
        LOGE("Failed to call systemClassLoaderObject");
        return;
    }
    LOGD("step2 finished!\n");

    /* Create DexClassLoader */
    LOGD("step3: using dexClassLoader class to create dexClassLoader object...");
    dexClassLoaderClass = (*env)->FindClass(env, "dalvik/system/DexClassLoader");//获取dexClassLoader类
    dexClassLoaderContructor = (*env)->GetMethodID(env, dexClassLoaderClass, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V");//获取dexClassLoader的Contructor
    dexPathString = (*env)->NewStringUTF(env, dexPath);//将char*转换成jString
    dexOptDirString = (*env)->NewStringUTF(env, dexOptDir);//将char*转换成jString
    dexClassLoaderObject = (*env)->NewObject(env, dexClassLoaderClass, dexClassLoaderContructor, dexPathString, dexOptDirString, NULL, systemClassLoaderObject);//生成自定义的dexClassLoader对象
    LOGD("step3 finished!\n");

    /* Use DexClassLoader to load target class */
    LOGD("step4: using dexClassLoader class to find [loadClass] method and using dexClassLoader object created above to load target class:[%s]...", className);
    loadClassMethod = (*env)->GetMethodID(env, dexClassLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");//获取dexClassLoader类中的“loadClass”方法
    classNameString = (*env)->NewStringUTF(env, className);////将char*转换成jString
    targetClass = (jclass)(*env)->CallObjectMethod(env, dexClassLoaderObject, loadClassMethod, classNameString); //调用“loadClass”方法来获取目标类
    if (!targetClass) {
        LOGE("Failed to load target class [%s]", className);
        return;
    }
    LOGD("step4 finished!\n");

    /* Invoke target method */
    LOGD("step5: using [%s] class loaded above to find [%s] method and invoke it...", className, methodName);
    targetMethod = (*env)->GetStaticMethodID(env, targetClass, methodName, "()V");//获取目标类中的静态方法：methodToBeInvoked
    if (!targetMethod) {
        LOGE("Failed to load target method [%s]", methodName);
        return;
    }
    (*env)->CallStaticVoidMethod(env, targetClass, targetMethod);//调用目标静态方法
    LOGD("step5 finished, invoking [%s] method succeeded!", methodName);
}