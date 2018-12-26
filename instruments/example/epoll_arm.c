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

#include <sys/types.h>
#include <sys/epoll.h>
#include <jni.h>

extern int my_epoll_wait(void *mp, char *url, int flags,
	void *p1, void *p2, void *p3, void *p4);

int my_epoll_wait_arm(void *mp, char *url, int flags,
	void *p1, void *p2, void *p3, void *p4)
{
	return my_epoll_wait(mp, url, flags, p1, p2, p3, p4);
}
