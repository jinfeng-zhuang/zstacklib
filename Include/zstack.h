#ifndef ZSTACK_H
#define ZSTACK_H

#include <zstack/config.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <zstack/algo.h>
#include <zstack/log.h>
#include <zstack/misc.h>
#include <zstack/yuv.h>
#include <zstack/ringbuf.h>

#include <zstack/vp9.h>
#include <zstack/h264.h>
#include <zstack/h265.h>

#ifdef _WIN32
#include <zstack/platform.h>
#include <zstack/window_class.h>
#include <Windows.h>
#endif

#include <zstack/codec.h>

#include <zstack/list.h>

extern int window_class_register(void);

#endif
