/*
 * devicemng.h
 *
 *  Created on: 16.08.2013
 *      Author: pascal
 */

#ifdef BUILD_KERNEL

#ifndef DEVICEMNG_H_
#define DEVICEMNG_H_

#include "cdi.h"
#include "list.h"
#include "stdint.h"
#include "stddef.h"
#include "vfs.h"
#include "semaphore.h"

typedef struct{
	struct cdi_device *device;
	list_t partitions;
	semaphore_t semaphore;
}device_t;

void dmng_Init(void);
void dmng_registerDevice(struct cdi_device *dev);
size_t dmng_Read(device_t *dev, uint64_t start, size_t size, void *buffer);
size_t dmng_Write(device_t *dev, uint64_t start, size_t size, const void *buffer);

void *dmng_getValue(device_t *dev, vfs_device_function_t function);

#endif /* DEVICEMNG_H_ */

#endif
