package net.iszilagyi.gpio;

/**
 * Created by iszilagyi on 5/7/17.
 *
 * singleton to access the PI GPIOs (based on wiringPi)
 */

public final class WiringPi {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    /**
     * Initialize GPIO library. Must be called before the first access.
     * Create and store file descriptors to /sys/class/gpio/...
     *
     * @return 0 if initialization worked.
     */
    public static native int initGPIOs();

    /**
     * Release the file descriptors created by initGPIOs
     */
    public static native void releaseGPIOs();

    /**
     * Read a PIN from raspberry pi.
     * @param pin
     * @return
     */
    public static native int digitalRead(int pin);
}

