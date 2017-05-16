package com.example.iszilagyi.myapplication1;

import android.content.Context;
import android.media.AudioManager;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.ifactory.gpio.WiringPi;

import java.util.logging.Logger;

public class MainActivity extends AppCompatActivity {

    private Snackbar msbBar = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        msbBar = Snackbar.make(findViewById(android.R.id.content), "Had a snack at Snackbar", Snackbar.LENGTH_LONG);


        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        int GPIOReady = WiringPi.initGPIOs();
        Logger.getGlobal().info("GPIO setup "  + GPIOReady);

        final int DT =  13; //a
        final int CLK = 19; //b
        final int MS =  6; //s (reset)

        tv.setText("Example text " + GPIOReady);

        final AudioManager  am = (AudioManager) getApplicationContext().getSystemService(Context.AUDIO_SERVICE);
        Logger.getGlobal().info("Audio manager "  + am);

        new Thread(new Runnable() {
            @Override
            public void run() {
               Logger.getGlobal().info("Thread started");

                int lastClkStatus = -1;
                int currentClkStatus = -1;
                boolean rotaryListen = false;
                int flag = 0;
                boolean listen = false;
                while (true) {
                     if (WiringPi.digitalRead(MS) == 0 ) {
                         listen = ! listen;
                         Logger.getGlobal().info("rtencoder - reset pressed " + listen);

                         msbBar.setText(listen ? "ROTARY ENCODER IS ON." : "ROTARY ENCODER IS OFF");
                         msbBar.show();
                         //am.adjustStreamVolume(AudioManager.STREAM_MUSIC, AudioManager.ADJUST_RAISE, AudioManager.FLAG_SHOW_UI);
                         delayMS(5000);
                         msbBar.dismiss();
                     }
                     if (listen) {
                       delayMS(3);
                     }else {
                         delayMS(3000);
                     }

                    lastClkStatus = WiringPi.digitalRead(CLK);
                    //Logger.getGlobal().info("MS val "  + msval);
                    while ( WiringPi.digitalRead(DT) == 0) {
                        currentClkStatus = WiringPi.digitalRead(CLK);
                        flag = 1;
                    }
                    if (flag == 1) {
                        flag = 0;
                        if (lastClkStatus == 0 && currentClkStatus == 1) {
                            am.adjustStreamVolume(AudioManager.STREAM_MUSIC, AudioManager.ADJUST_RAISE, AudioManager.FLAG_SHOW_UI);
                            Logger.getGlobal().info(" Volume up");
                        }
                        if (lastClkStatus == 1 && currentClkStatus == 0) {
                            am.adjustStreamVolume(AudioManager.STREAM_MUSIC, AudioManager.ADJUST_LOWER, AudioManager.FLAG_SHOW_UI);
                            Logger.getGlobal().info(" Volume down");
                        }
                    }
                 }
            }
        }).start();
    }
    private void delayMS(int ms) {
        try {
            Thread.sleep(ms);
        }catch (Exception ex) {
            Logger.getGlobal().severe(" Error " + ex.getMessage());
        }
    }
    public void sendMessage(View view) {
        Logger.getGlobal().info("exiting application");
        WiringPi.releaseGPIOs();
        System.exit(0);
    }

    @Override
    protected void onDestroy() {
        // TODO stop thread.
        super.onDestroy();
    }

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
}
