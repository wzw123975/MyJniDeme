#include <jni.h>
#include <string>

char *Jstring2CStr(JNIEnv *pEnv, jstring pJstring, jstring pJstring1);

extern "C" JNIEXPORT jstring JNICALL
Java_com_wzw_myjnideme_MainActivity_stringFromJNI(JNIEnv *env, jobject /* this */) {
    std::string hello = "Hello from C--";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_wzw_jnilibrary_Utils_stringFromJNI(JNIEnv *env, jclass clazz) {
    std::string hello = "Hello from My C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C" JNIEXPORT void JNICALL
Java_com_wzw_jnilibrary_Utils_accessField(JNIEnv *env, jobject thiz) {
    jclass jcla = env->GetObjectClass(thiz);//获取类
    jfieldID jfId = env->GetFieldID(jcla, "showText", "Ljava/lang/String;");//获取类的成员变量showText的id
    jstring after = env->NewStringUTF("Hello NDK");
    env->SetObjectField(thiz, jfId, after);   //修改属性id对应的值
}
extern "C" JNIEXPORT void JNICALL
Java_com_wzw_jnilibrary_Utils_accessStaticField(JNIEnv *env, jobject thiz) {
    jclass oClass = env->GetObjectClass(thiz); //获取类
    jfieldID staticFid = env->GetStaticFieldID(oClass, "staticString",
                                               "Ljava/lang/String;");//获取静态变量id
    jstring after = env->NewStringUTF("static field update in jni");  //设置静态变量
    env->SetStaticObjectField(oClass, staticFid, after);
}
extern "C" JNIEXPORT jstring JNICALL
Java_com_wzw_jnilibrary_Utils_accessMethod(JNIEnv *env, jobject thiz) {
    jclass oClass = env->GetObjectClass(thiz);
    jmethodID jmethodId = env->GetMethodID(oClass, "getAuthName",
                                           "(Ljava/lang/String;)Ljava/lang/String;");
    jstring res = env->NewStringUTF("103style");
    jobject objRes = env->CallObjectMethod(thiz, jmethodId, res);
    return static_cast<jstring>(objRes);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_wzw_jnilibrary_Utils_accessStaticMethod(JNIEnv *env, jobject thiz, jstring max) {
    jclass oClass = env->GetObjectClass(thiz);
    jmethodID jmethodId = env->GetStaticMethodID(oClass, "getRandomValue","(Ljava/lang/String;)I");
    int objRes = env->CallStaticIntMethod(oClass, jmethodId, max);
    return objRes;
}extern "C"
JNIEXPORT jobject JNICALL
Java_com_wzw_jnilibrary_Utils_accessConstructor(JNIEnv *env, jobject thiz) {
    jclass oClass = env->GetObjectClass(thiz);
    //获取构造函数的methodId  构造函数为 void函数 对应的方法名为<init>
    jmethodID methodID = env->GetMethodID(oClass, "<init>", "(Z)V");
    jobject jTestClass = env->NewObject(oClass, methodID,false);
    return jTestClass;
}