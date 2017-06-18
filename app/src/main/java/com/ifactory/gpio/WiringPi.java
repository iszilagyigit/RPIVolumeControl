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
}

