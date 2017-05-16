//
// Created by iszilagyi on 5/7/17.
// /home/iszilagyi/work/arm7-ndk/wiringPi/

#include <stdio.h>
#include <fcntl.h>



#include <jni.h>
#include <android/log.h>
#include <errno.h>


#define	LOW			 0
#define	HIGH		 1
#define	NA		    -1

// file descriptiors for "/sys/class/gpio/gpio22/value s
static int sysFds [64] =
        {
                NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,
                NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,
                NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,
                NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,
        };
// open modes from filedescriptors
static int sysFdsMode [64] =
        {
                NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,
                NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,
                NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,
                NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA,
        };
/*
 * Read all file descriptors
 */
extern "C" JNIEXPORT jint JNICALL  Java_com_ifactory_gpio_WiringPi_initGPIOs(
        JNIEnv* env,
        jobject /* this */) {
    //return wiringPiSetupSys();

    __android_log_write(ANDROID_LOG_VERBOSE, "WiringPi", "initGPIOs");
    char fName [128] ;
    for (int pin = 0 ; pin < 64 ; ++pin)  { // only if it is not initialized
        if (sysFds [pin] == NA) {
            sprintf(fName, "/sys/class/gpio/gpio%d/value", pin);
            sysFds[pin] = open(fName, O_RDWR);
            if (sysFds[pin] != NA) {
                sysFdsMode[pin] = O_RDWR;
            } else {
                if (errno == EACCES) {
                    __android_log_print(ANDROID_LOG_VERBOSE, "WiringPi", " O_RDWR failed %d - PermisionDenied. ", pin);
                }
                sysFds[pin] = open(fName, O_WRONLY);
                if (sysFds[pin] != NA) {
                    sysFdsMode[pin] = O_WRONLY;
                } else {
                    sysFds[pin] = open(fName, O_RDONLY);
                    if (sysFds[pin] != NA) {
                        sysFdsMode[pin] = O_RDONLY;
                    }
                }
            }
        }
    }
    // log initialized file descriptors and modes.
    for (int i= 0 ; i < 64 ; ++i){
        if (sysFds [i] != NA) {
            __android_log_print(ANDROID_LOG_VERBOSE, "WiringPi", "pin %d FDMode: %d FD: %d ",
                                i, sysFdsMode[i], sysFds[i]);
        }
    }
    return 0;
}

extern "C" JNIEXPORT void JNICALL  Java_com_ifactory_gpio_WiringPi_releaseGPIOs(
        JNIEnv* env,
        jobject /* this */) {
    __android_log_write(ANDROID_LOG_VERBOSE, "WiringPi", "releaseGPIOs");
    for (int pin = 0 ; pin < 64 ; ++pin) {
        int fd = sysFds [pin]; //file descr.
        if (fd!= NA) {
            if (close(fd) == 0) {
                sysFds[pin] = NA;
            }else {
                __android_log_print(ANDROID_LOG_VERBOSE, "WiringPi", "close error %d " , errno);
            }
            sysFdsMode[pin] = NA;
        }
    }
}
extern "C" JNIEXPORT jint JNICALL
Java_com_ifactory_gpio_WiringPi_digitalRead (JNIEnv *env,  jobject callingObject,
                                             jint jpin )  {

    int pin = (int) jpin;
    if (sysFds [pin] == NA) {
        return NA;
    }
    char c;
    lseek  (sysFds [pin], 0L, SEEK_SET) ;
    read   (sysFds [pin], &c, 1) ;
    return (c == '0') ? LOW : HIGH ;
}

 /*

extern "C" JNIEXPORT void JNICALL
Java_com_ifactory_gpio_WiringPi_initRotaryEncoder(JNIEnv *env,  jobject callingObject,
                                                  jint  RoDT, jint  RoCLK, jint  RoMS) {
    RoDT_Pin = RoDT;
    RoCLK_Pin = RoCLK;
    RoMS_Pin = RoMS;

    pinMode(RoDT_Pin,INPUT);
    pinMode(RoCLK_Pin, INPUT);
    pinMode(RoMS_Pin, INPUT);

    pullUpDnControl(RoMS_Pin, PUD_UP);

}

extern "C" JNIEXPORT jint JNICALL
  Java_com_ifactory_gpio_WiringPi_readRotaryCounter(JNIEnv *env,  jobject callingObject) {
    return ROTARY_COUNTER;
}


extern "C" JNIEXPORT void JNICALL
 Java_com_ifactory_gpio_WiringPi_rotaryDeal(JNIEnv *env,  jobject callingObject)
{

    ROTARY_LISTEN ? delay(5) : delay(3000); // 3ms by listen, 3s by sleep

    if(digitalRead(RoMS_Pin) == 0) {
        ROTARY_LISTEN = ! ROTARY_LISTEN;
        //printf("ROTARY_LISTEN : %d\n",ROTARY_LISTEN);
        delay(1000);//wait to be able to release
    }
    Last_RoB_Status = digitalRead(RoCLK_Pin);
    while(!digitalRead(RoDT_Pin)){
        Current_RoB_Status = digitalRead(RoCLK_Pin);

        if((Last_RoB_Status == 0)&&(Current_RoB_Status == 1)){
            ROTARY_COUNTER ++;
        }
        if((Last_RoB_Status == 1)&&(Current_RoB_Status == 0)){
            ROTARY_COUNTER --;
        }
        if ( ROTARY_COUNTER != lastGlobalCounter ) {
            lastGlobalCounter = ROTARY_COUNTER;
            //printf("ROTARY_COUNTER : %d\n",ROTARY_COUNTER);
            delay(250); // 250ms - to avoid more then one increment at once.
        }
    }

}
*/