package com.wzw.jnilibrary;

import android.util.Log;

public class JniArrayOperation {
    static {
        System.loadLibrary("array_operation");
    }
    public native int[] getIntArray(int length);
    public native void sortIntArray(int[] arr);
    public void test() {
        //获取随机的20个数
        int[] array = getIntArray(20);
        for (int i : array) {
            Log.i("ii--",i + ", ");
        }

        //对数组进行排序
        sortIntArray(array);
        Log.i("ii--","\nAfter sort:\n");
        for (int i : array) {
            Log.i("ii--",i + ", ");
        }
    }
}