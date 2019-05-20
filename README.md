adbi - The Android Dynamic Binary Instrumentation Toolkit  
=========================================================

Simple binary instrumentation toolkit for Android ARM + Thumb.

Instrumentation is based on library injection and hooking function entry 
points (in-line hooking).

The toolkit consists of two main components the hijack tool and the base 
library.

**hijack**

 The hijack tool provides the injection functionality. It supports a number of  modes for supporting older and newer Android devices. hijack provides help on the command line.
 
**libbase**

 The base library provides the hooking and unhooking functionality. The base library is compiled as a static library so it can be directly included in the
 actual instrumentation library. This is done so we can keep everything in /data/local/tmp. 

Below we provide and easy to follow step-by-step instructions for howto build and use adbi. The example instrument hijacks ffio_open_whitelist() and logs every call
to a file.

=== Prerequisites ===

Android SDK
Android NDK

=== How to Build ===

= build the hijack tool =
```
cd hijack
cd jni
ndk-build
cd ..
adb push libs/armeabi/hijack /data/local/tmp/
cd ..
```

= build the instrumentation base code =

```
cd instruments
cd base
cd jni
ndk-build
cd ..
cd ..
```

= build instrumentation example =

```
cd example
cd jni
ndk-build
cd ..
adb push libs/armeabi/libexample.so /data/local/tmp/
```

=== How to Run ===

```
adb shell
su
setenforce 0
cd /data/local/tmp
>/sdcard/adbi_example.log
# GET PID from tv.danmaku.bili
./hijack -d -p PID -l /data/local/tmp/libexample.so
cat /sdcard/adbi_example.log
```

output should look similar to:

```
started
hooking:   ffio_open_whitelist = 0xc9d7eead THUMB using 0xc8f21839
Sun Dec  9 13:36:00 2018
 ijkio:cache:ffio:ijkhttphook:http://124.14.12.185/upgcxcode/94/54/66295494/66295494-1-30280.m4s
Sun Dec  9 13:36:02 2018
 ijkio:cache:ffio:ijkhttphook:http://124.14.12.168/upgcxcode/94/54/66295494/66295494-1-30077.m4s
```

=== Extra ===

= build instrumentation example3 =

```
cd example3
cd jni
ndk-build
cd ..
adb push libs/armeabi/libexample3.so /data/local/tmp/
cd demo #open Android Studio to Build APK
cd app/build/outputs/apk/debug #unzip app-debug.apk
adb push classes.dex /sdcard/com.zhenl.demo/classes.dex
adb push lib/armeabi-v7a/libwhale.so /data/local/tmp/
```

=== How to Run ===

```
adb shell
su
setenforce 0
cd /data/local/tmp
# GET PID from com.tencent.mm
./hijack -d -p PID -l /data/local/tmp/libexample3.so
logcat
```

output should look similar to:

```
whale.hook: keep alive is running
rawQueryWithFactory: SELECT * FROM userinfo2 WHERE sid=?;:null
```
