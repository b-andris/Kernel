/*
 * sycalls.c
 *
 *  Created on: 17.07.2012
 *      Author: pascal
 */

#ifdef BUILD_KERNEL

#include "syscalls.h"
#include "stddef.h"
#include "stdbool.h"
#include "isr.h"
#include "cmos.h"
#include "pm.h"
#include "vfs.h"
#include "loader.h"
#include "pit.h"
#include "system.h"
#include "cpu.h"
#include "scheduler.h"
#include "cleaner.h"
#include "assert.h"

#define STAR	0xC0000081
#define LSTAR	0xC0000082
#define CSTAR	0xC0000083
#define SFMASK	0xC0000084

//extern uintptr_t mm_SysAlloc(uint64_t);
//extern void mm_SysFree(uintptr_t, uint64_t);
extern void setColor(uint8_t);
extern void setCursor(uint16_t, uint16_t);
extern void isr_syscall();

static void nop();
static uint64_t createThreadHandler(void *entry);
static void exitThreadHandler();
static void sleepHandler(uint64_t msec);

typedef uint64_t(*syscall)(uint64_t arg, ...);

static syscall syscalls[] = {
		(syscall)&mm_Alloc,				//0
		(syscall)&mm_Free,				//1
		(syscall)&vmm_unusePages,		//2
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,

		(syscall)&loader_load,			//10
		(syscall)&pm_ExitTask,			//11
		(syscall)&createThreadHandler,	//12
		(syscall)&exitThreadHandler,	//13
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,

		(syscall)&nop,					//20
		(syscall)&nop,					//21
		(syscall)&nop,
		(syscall)&setColor,				//23
		(syscall)&setCursor,			//24
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,

		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,

		(syscall)&vfs_syscall_open,		//40
		(syscall)&vfs_syscall_close,	//41
		(syscall)&vfs_syscall_read,		//42
		(syscall)&vfs_syscall_write,	//43
		(syscall)&vfs_syscall_getFileinfo,		//44
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,

		(syscall)&cmos_GetTime,			//50
		(syscall)&cmos_GetDate,			//51
		(syscall)&sleepHandler,			//52
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,
		(syscall)&nop,

		(syscall)&getSystemInformation	//60
};

void syscall_Init()
{
	//Prüfen, ob syscall/sysret unterstützt wird
	if(cpuInfo.syscall)
	{
		cpu_MSRwrite(STAR, (0x8ul << 32) | (0x13ul << 48));	//Segementregister
		cpu_MSRwrite(LSTAR, (uintptr_t)isr_syscall);		//Einsprungspunkt
		cpu_MSRwrite(SFMASK, 0);							//Wir setzen keine Bits zurück (Interrupts bleiben auch aktiviert)

		//Syscall-Instruktion aktivieren (ansonsten #UD)
		//Bit 0
		cpu_MSRwrite(0xC0000080, cpu_MSRread(0xC0000080) | 1);
	}
}

uint64_t syscall_syscallHandler(uint64_t func, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5)
{
	//FIXME
	assert(func < sizeof(syscalls) / sizeof(syscall));
	return syscalls[func](arg1, arg2, arg3, arg4, arg5);
}

/*
 * Funktionsnummer wird im Register rdi übergeben
 */
ihs_t *syscall_Handler(ihs_t *ihs)
{
	ihs->rax = syscall_syscallHandler(ihs->rdi, ihs->rsi, ihs->rdx, ihs->rcx, ihs->r8, ihs->r9);
	return ihs;
}

static void nop()
{
	asm volatile("nop");
}

static uint64_t createThreadHandler(void *entry)
{
	thread_t *thread = thread_create(currentProcess, entry, 0, NULL, false);
	thread_unblock(thread);
	return thread->tid;
}

static void exitThreadHandler()
{
	cleaner_cleanThread(currentThread);
	yield();
}

static void sleepHandler(uint64_t msec)
{
	pit_RegisterTimer(currentThread, msec);
}

#endif
