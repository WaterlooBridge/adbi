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
 ijkio:cache:ffio:ijkhttphook:http://124.14.12.185/upgcxcode/94/54/66295494/66295494-1-30280.m4s?e=ig8euxZM2rNcNbdlhoNvNC8BqJIzNbfqXBvEuENvNC8aNEVEtEvE9IMvXBvE2ENvNCImNEVEIj0Y2J_aug859r1qXg8xNEVE5XREto8GuFGv2U7SuxI72X6fTr859IB_&deadline=1544340959&gen=playurl&nbs=1&oi=1710530337&os=acache&platform=android&trid=646bae680a54420ea3db1428194658b3&uipk=5&upsig=b92d006fb988313c30bfa5d4d5608887
Sun Dec  9 13:36:02 2018
 ijkio:cache:ffio:ijkhttphook:http://124.14.12.168/upgcxcode/94/54/66295494/66295494-1-30077.m4s?e=ig8euxZM2rNcNbdlhoNvNC8BqJIzNbfqXBvEuENvNC8aNEVEtEvE9IMvXBvE2ENvNCImNEVEIj0Y2J_aug859r1qXg8xNEVE5XREto8GuFGv2U7SuxI72X6fTr859IB_&deadline=1544340959&gen=playurl&nbs=1&oi=1710530337&os=acache&platform=android&trid=646bae680a54420ea3db1428194658b3&uipk=5&upsig=ba99a360216dd7deb79883c3b4a81ae2
```

