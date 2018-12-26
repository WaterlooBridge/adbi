/*
 *  Collin's Binary Instrumentation Tool/Framework for Android
 *  Collin Mulliner <collin[at]mulliner.org>
 *  http://www.mulliner.org/android/
 *
 *  (c) 2012,2013
 *
 *  License: LGPL v2.1
 *
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <string.h>
#include <termios.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <time.h>

#include <jni.h>
#include <stdlib.h>
#include <android/log.h>

#include "../base/hook.h"
#include "../base/base.h"

#undef log

#define log(...) \
        {FILE *fp = fopen("/sdcard/adbi_example.log", "a+"); if (fp) {\
        fprintf(fp, __VA_ARGS__);\
        fclose(fp);}}


// this file is going to be compiled into a thumb mode binary

void __attribute__ ((constructor)) my_init(void);

static struct hook_t eph;

// arm version of hook
extern int my_epoll_wait_arm(void *mp, char *url, int flags,
	void *p1, void *p2, void *p3, void *p4);

/*  
 *  log function to pass to the hooking library to implement central loggin
 *
 *  see: set_logfunction() in base.h
 */
static void my_log(char *msg)
{
	log("%s", msg)
}

int my_epoll_wait(void *mp, char *url, int flags,
	void *p1, void *p2, void *p3, void *p4)
{
	int (*orig_epoll_wait)(
	void *mp, char *url, int flags,
	void *p1, void *p2, void *p3, void *p4);
	orig_epoll_wait = (void*)eph.orig;

	hook_precall(&eph);
	int res = orig_epoll_wait(mp, url, flags, p1, p2, p3, p4);
	hook_postcall(&eph);
	time_t t;
	struct tm *timeinfo;  //结构体
	time(&t);
	timeinfo = localtime(&t);
	log("%s %s\n", asctime(timeinfo), url);
	__android_log_print(ANDROID_LOG_INFO, "JNITag", "url %s", url);
        
	return res;
}

void my_init(void)
{
	__android_log_print(ANDROID_LOG_INFO, "JNITag", "%s started\n", __FILE__);
	log("%s started\n", __FILE__);
 
	set_logfunction(my_log);

	hook(&eph, getpid(), "libijkffmpeg.", "ffio_open_whitelist", my_epoll_wait_arm, my_epoll_wait);
}

