package com.wzw.jnilibrary;

import android.util.Log;

import java.util.Random;

public class Utils {

    static {
        System.loadLibrary("native-lib");
    }

    private static String staticString = "static string";

    public String showText = "Hello World By Java";

    public Utils() {
    }

    public Utils(String showText) {
        this.showText = showText;
    }
    public Utils(boolean b) {
        if(!b){
            this.showText = "*-+++++++++";
        }
       Log.i("ii--","ccccccccccccccccc");
    }

    public static native String stringFromJNI();

    public native void accessField();

    public native void accessStaticField();

    public native String accessMethod();

    public native int accessStaticMethod(String s);

    public native Utils accessConstructor();

    public String getAuthName(String name) {
        Log.e("ii--", "name = " + name);
        return name;
    }

    private static int  getRandomValue(String max) {
//        int i = new Random().nextInt(Integer.parseInt(max));
        Log.e("ii--", "imax = " + max);
        return 11;
    }


}
