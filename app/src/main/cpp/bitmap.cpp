#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>

#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"JniBitmapDemo",FORMAT,##__VA_ARGS__);
extern "C"
JNIEXPORT void JNICALL
Java_com_wzw_jnilibrary_JniBitmapDemo_passBitmap(JNIEnv *env, jobject thiz, jobject bitmap) {
    if (nullptr == bitmap) {
        LOGE("bitmap is null");
    }

    AndroidBitmapInfo info;
    int result;

    //获取图片信息
    result = AndroidBitmap_getInfo(env, bitmap, &info);

    if (result != ANDROID_BITMAP_RESUT_SUCCESS) {
        LOGE("AndroidBitmap_getInfo failed, result: %d", result);
        return;
    }

    LOGE("bitmap width: %d, height: %d, format: %d, stride: %d", info.width, info.height,
         info.format, info.stride);

    unsigned char *addrPtr;

    // 获取像素信息
    result = AndroidBitmap_lockPixels(env, bitmap, reinterpret_cast<void **>(&addrPtr));

    if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("AndroidBitmap_lockPixels failed, result: %d", result);
        return;
    }

    // 执行图片操作的逻辑
    int length = info.stride * info.height;
    for (int i = 0; i < length; ++i) {
        LOGE("value: %d", addrPtr[i]);
    }

    // 像素信息不再使用后需要解除锁定
    result = AndroidBitmap_unlockPixels(env, bitmap);
    if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("AndroidBitmap_unlockPixels failed, result: %d", result);
    }
}