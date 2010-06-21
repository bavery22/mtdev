/*****************************************************************************
 *
 * mtdev - MT device event converter (MIT license)
 *
 * Copyright (C) 2010 Henrik Rydberg <rydberg@euromail.se>
 * Copyright (C) 2010 Canonical Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 ****************************************************************************/

#ifndef _MTDEV_H
#define _MTDEV_H

#include <linux/input.h>

/* includes available in 2.6.30-rc5 */
#ifndef BTN_TOOL_QUADTAP
#define BTN_TOOL_QUADTAP	0x14f	/* Four fingers on trackpad */
#define ABS_MT_TOUCH_MAJOR	0x30	/* Major axis of touching ellipse */
#define ABS_MT_TOUCH_MINOR	0x31	/* Minor axis (omit if circular) */
#define ABS_MT_WIDTH_MAJOR	0x32	/* Major axis of approaching ellipse */
#define ABS_MT_WIDTH_MINOR	0x33	/* Minor axis (omit if circular) */
#define ABS_MT_ORIENTATION	0x34	/* Ellipse orientation */
#define ABS_MT_POSITION_X	0x35	/* Center X ellipse position */
#define ABS_MT_POSITION_Y	0x36	/* Center Y ellipse position */
#define ABS_MT_TOOL_TYPE	0x37	/* Type of touching device */
#define ABS_MT_BLOB_ID		0x38	/* Group a set of packets as a blob */
#define ABS_MT_TRACKING_ID	0x39	/* Unique ID of initiated contact */
#define SYN_MT_REPORT		2
#define MT_TOOL_FINGER		0
#define MT_TOOL_PEN		1
#endif

/* includes available in 2.6.33 */
#ifndef ABS_MT_PRESSURE
#define ABS_MT_PRESSURE		0x3a	/* Pressure on contact area */
#endif

/* includes available in 2.6.36 */
#ifndef ABS_MT_SLOT
#define ABS_MT_SLOT		0x2f	/* MT slot being modified */
#define MT_SLOT_ABS_EVENTS {	\
	ABS_MT_TOUCH_MAJOR,	\
	ABS_MT_TOUCH_MINOR,	\
	ABS_MT_WIDTH_MAJOR,	\
	ABS_MT_WIDTH_MINOR,	\
	ABS_MT_ORIENTATION,	\
	ABS_MT_POSITION_X,	\
	ABS_MT_POSITION_Y,	\
	ABS_MT_TOOL_TYPE,	\
	ABS_MT_BLOB_ID,		\
	ABS_MT_TRACKING_ID,	\
	ABS_MT_PRESSURE,	\
}
#endif

#define MT_ABS_SIZE 11

#define MT_ID_NULL	(-1)
#define MT_ID_MIN	0
#define MT_ID_MAX	65535

/**
 * struct mt_caps - protocol capabilities of kernel device
 * @has_mtdata: true if the device has MT capabilities
 * @has_slot: true if the device sends MT slots
 * @slot: slot event properties
 * @abs: ABS_MT event properties
 */
struct mtdev_caps {
	int has_mtdata;
	int has_slot;
	int has_abs[MT_ABS_SIZE];
	struct input_absinfo slot;
	struct input_absinfo abs[MT_ABS_SIZE];
};

/**
 * struct mtdev - represents an input MT device
 * @caps: the kernel device protocol capabilities
 * @state: internal mtdev parsing state
 *
 * The mtdev structure represents a kernel MT device type B, emitting
 * MT slot events. The events put into mtdev may be from any MT
 * device, specifically type A without contact tracking, type A with
 * contact tracking, or type B with contact tracking. See the kernel
 * documentation for further details.
 *
 */
struct mtdev {
	struct mtdev_caps caps;
	struct mtdev_state *state;
};

/**
 * mtdev_open - open an mtdev converter
 * @dev: the mtdev to open
 * @fd: file descriptor of the kernel device
 *
 * Initialize the mtdev structure and configure it by reading
 * the protocol capabilities through the file descriptor.
 *
 * Returns zero on success, negative error number otherwise.
 *
 * This call combines the plumbing functions mtdev_init() and
 * mtdev_configure().
 */
int mtdev_open(struct mtdev *dev, int fd);

/**
 * mtdev_idle - check state of kernel device
 * @dev: the mtdev in use
 * @fd: file descriptor of the kernel device
 * @ms: number of milliseconds to wait for activity
 *
 * Returns true if the device is idle, i.e., there are no fetched
 * events in the pipe and there is nothing to fetch from the device.
 */
int mtdev_idle(struct mtdev *dev, int fd, int ms);

/**
 * mtdev_get - get processed events from mtdev
 * @dev: the mtdev in use
 * @fd: file descriptor of the kernel device
 * @ev: array of input events to fill
 * @ev_max: maximum number of events to read
 *
 * Get a processed event from mtdev. The events appear as if they came
 * from a type B device emitting MT slot events.
 *
 * The read operations involved behave as dictated by the file
 * descriptor; if O_NONBLOCK is not set, mtdev_get() will block until
 * the specified number of processed events are available.
 *
 * On success, returns the number of events read. Otherwise,
 * a standard negative error number is returned.
 *
 * This call combines the plumbing functions mtdev_fetch_event(),
 * mtdev_put_event() and mtdev_get_event().
 */
int mtdev_get(struct mtdev *dev, int fd, struct input_event* ev, int ev_max);

/**
 * mtdev_close - close the mtdev converter
 * @dev: the mtdev to close
 *
 * Deallocates all memory associated with mtdev, and clears the mtdev
 * structure.
 */
void mtdev_close(struct mtdev *dev);

#endif
