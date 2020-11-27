//
// Created by wangzhiwei on 2020/11/11.
//
#include <android/log.h>
#include <jni.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>


#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"JniFileOperation",FORMAT,##__VA_ARGS__);

const int SIZE = 20;

extern "C"
JNIEXPORT void JNICALL
Java_com_wzw_jnilibrary_JniFileOperation_createFile(JNIEnv *env, jobject thiz, jstring file_name) {
    const char *fileName = env->GetStringUTFChars(file_name, nullptr);
    FILE *fp = fopen(fileName, "wb"); //创建写文件流
    fputs("0123456789abcdefg", fp);
    fclose(fp); //关闭流
    LOGE("%s", "创建文件成功");
    env->ReleaseStringUTFChars(file_name, fileName);
}

/**
 * 根据文件的路径，获得文件的大小
 */
long get_file_size(const char *path) {
    //rb：只读打开一个二进制文件，允许读数据
    //使用给定的模式 "rb" 打开 path 所指向的文件
    FILE *fp = fopen(path, "rb");
    if (fp == nullptr) {
        LOGE("%s", "文件打开失败");
        return 0;
    }
    //SEEK_SET	文件的开头
    //SEEK_CUR	文件指针的当前位置
    //SEEK_END	文件的末尾
    //设置流 fp 的文件位置为 0， 0 意味着从给定的 SEEK_END 位置查找的字节数。
    fseek(fp, 0, SEEK_END);
    //返回给定流 fp 的当前文件位置。
    return ftell(fp);
}

void print_bin(char n) {
    int l = sizeof(n) * 8;//总位数。
    int i;
    if (i == 0) {
        printf("0");
        return;
    }
    for (i = l - 1; i >= 0; i--)//略去高位0.
    {
        if (n & (1 << i)) break;
    }

    for (; i >= 0; i--)
        LOGE("ii--==== yyy=====%d", (n & (1 << i)) != 0);
}

void splits2(JNIEnv *env, jstring path_, jstring pathPattern_, jint splitCount) {
    char ww = 'a';
    print_bin(ww);
    const char *path = env->GetStringUTFChars(path_, nullptr);
    const char *pathPattern = env->GetStringUTFChars(pathPattern_, nullptr);

    //malloc：分配所需的内存空间，并返回一个指向它的指针。
    char **patches = new char *[splitCount];
    //获取文件长度
    long fileSize = get_file_size(path);
    //获取单个文件长度
    long per_size = fileSize / splitCount;
//    bool isBisection = fileSize % splitCount == 0;  //文件是否等分


    //设置每个子文件的路径
    for (int i = 0; i < splitCount; i++) {
        patches[i] = new char[256];
        sprintf(patches[i], pathPattern, i);
        LOGE("patches[%d] = %s", i, patches[i]);
    }


    //创建fp流读取path对应的文件
    FILE *fp = fopen(path, "rb");
    if (fp == nullptr) {
        LOGE("%s", "文件打开失败");
        return;
    }
    //读取分割文件的流
    FILE *index_fp = nullptr;
    int splitIndex = 0;
    int wIndex = 0;
    while (1) {
        char c = fgetc(fp);

        if (feof(fp)) {
            fclose(fp);
            break;
        }
        if (wIndex % per_size == 0 && splitIndex != splitCount) {
            if (index_fp != nullptr) {
                fclose(index_fp);
                index_fp = nullptr;
            }
            index_fp = fopen(patches[splitIndex], "wb");
            splitIndex++;
        }

        fputc(c, index_fp);
        wIndex++;
    }
    if (index_fp != nullptr) {
        fclose(index_fp);
    }
    //释放内存
    for (int i = 0; i < splitCount; i++) {
        free(patches[i]);
    }
    free(patches);

    env->ReleaseStringUTFChars(path_, path);
    env->ReleaseStringUTFChars(pathPattern_, pathPattern);
}

void splits1(JNIEnv *env, jstring path_, jstring pathPattern_, jint splitCount) {
    LOGE("ii--==== yyy=====%d", 111111);
    char name[20];
    const char *path = env->GetStringUTFChars(path_, nullptr);
    const char *pathPattern = env->GetStringUTFChars(pathPattern_, nullptr);
    FILE *f3 = fopen(path, "rb");
    LOGE("ii--==== yyy=====%d", 22222222);
//    string strBlock = "F:/数据/BLOCK.dat";
//    FILE *f3 = fopen(strBlock.c_str(), "rb+");

    // 获取文件数量
    int iCount = 0;
    fseek(f3, 0, SEEK_SET);
    fread(&iCount, sizeof(int), 1, f3);
    LOGE("ii--==== yyy=====iCount %d", iCount);
    for (int i = 0; i < iCount; i++) {
        memset(name, 0x0, 20);
        LOGE("ii--==== yyy=====%d", 444);
        fread(&name, 20, 1, f3);
        LOGE("ii--==== yyy=====%d", 5555);
        // 输出名称
//        cout << name << endl;
        int iLen = 0;
        // 读取文件长度
        fread(&iLen, sizeof(int), 1, f3);
        LOGE("ii--==== yyy=====%d", 6666);
        char *buff = new char[iLen];
        // 读取文件内容
        fread(buff, iLen, 1, f3);
        char fileLen[10];
        sprintf(fileLen, "%d", iLen);
        LOGE("ii--==== yyy=====%d", 7777);

        char *patches;
        sprintf(patches, pathPattern, i);
        LOGE("ii--==== yyy=====%d", 888);
        // 新建一个文件
        FILE *file = fopen(patches, "wb");
        LOGE("ii--==== yyy=====%d", iLen);
        fwrite(buff, iLen, 1, file);
        LOGE("ii--==== yyy=====%d", 1122);
        fclose(file);
        LOGE("ii--==== yyy=====%d", 1133);
    }

    fclose(f3);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_wzw_jnilibrary_JniFileOperation_split(JNIEnv *env, jobject thiz, jstring path_,
                                               jstring pathPattern_, jint splitCount) {
    splits2(env, path_, pathPattern_, splitCount);
//    const char *path = env->GetStringUTFChars(path_, nullptr);
//    const char *pathPattern = env->GetStringUTFChars(pathPattern_, nullptr);
//
//    //malloc：分配所需的内存空间，并返回一个指向它的指针。
//    char **patches = new char *[splitCount];
//
//    //获取文件长度
//    long fileSize = get_file_size(path);
//
//    //获取单个文件长度
//    long per_size = fileSize / splitCount;
//
//    //设置每个子文件的路径
//    for (int i = 0; i < splitCount; i++) {
//        patches[i] = new char[256];
//        sprintf(patches[i], pathPattern, i);
//        LOGE("patches[%d] = %s", i, patches[i]);
//    }
//
//    //创建fp流读取path对应的文件
//    FILE *fp = fopen(path, "rb");
//    if (fp == nullptr) {
//        LOGE("%s", "文件打开失败");
//        return;
//    }
//    //读取分割文件的流
//    FILE *index_fp = nullptr;
//    int index = 0;
//    for (int i = 0; i < fileSize; i++) {
//        int mmm = i % per_size;
//        LOGE("ii--  mmm  = %d    i= %d    per_size= %d", mmm, i, per_size);
//        if (mmm == 0) {
//            if (index_fp != nullptr) {
//                fclose(index_fp);
//            }
//            index_fp = fopen(patches[index], "wb");
//            index++;
//            if (index_fp == nullptr) {
//                LOGE("文件%s打开失败", patches[index]);
//                return;
//            }
//        }
//        fputc(fgetc(fp), index_fp);
//
//        //读完之后释放流
//        if (i + 1 == fileSize) {
//            fclose(index_fp);
//        }
//    }
//    fclose(fp);
//
//    //释放内存
//    for (int i = 0; i < splitCount; i++) {
//        free(patches[i]);
//    }
//    free(patches);
//
//    env->ReleaseStringUTFChars(path_, path);
//    env->ReleaseStringUTFChars(pathPattern_, pathPattern);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_wzw_jnilibrary_JniFileOperation_merge(JNIEnv *env, jobject thiz, jstring path_merge,
                                               jstring path_pattern, jint count) {
    const char *pathMerge = env->GetStringUTFChars(path_merge, nullptr);
    const char *pathPattern = env->GetStringUTFChars(path_pattern, nullptr);
    LOGE("ii--%s", "=============================================\n");
    FILE *fp = fopen(pathMerge, "wb");    //创建合并文件的写流
    FILE *index_fp = nullptr;
    char *patches;
    for (int i = 0; i < count; i++) {
        patches = new char[256];
        sprintf(patches, pathPattern, i);
        index_fp = fopen(patches, "rb");
        while (1) {
            char c = fgetc(index_fp);
            if (feof(index_fp)) {
                fclose(index_fp);
                break;
            }
            fputc(c, fp);
        }
    }
    free(patches);
    fclose(fp);
    env->ReleaseStringUTFChars(path_merge, pathMerge);
    env->ReleaseStringUTFChars(path_pattern, pathPattern);

}

