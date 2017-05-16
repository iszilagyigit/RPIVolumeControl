package com.ifactory.gpio;

import java.util.logging.Logger;

/**
 * Created by iszilagyi on 5/7/17.
 *
 * singleton to access the PI GPIOs
 */

public final class WiringPi {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }


    public static native int initGPIOs();
    public static native void releaseGPIOs();

    public static native int digitalRead     (int pin);


/*
    public static native void pinMode             (int pin, int mode) ;
    public static native void pullUpDnControl     (int pin, int pud) ;


    public static native void initRotaryEncoder(int  RoDT_Pin, int  RoCLK_Pin, int  RoMS_Pin);
    public static native void rotaryDeal();
    public static native int readRotaryCounter();
*/
}

