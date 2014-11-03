/*
 * tss.c
 *
 *  Created on: 02.11.2012
 *      Author: pascal
 */

#ifdef BUILD_KERNEL

#include "tss.h"
#include "gdt.h"

#define SELECTOR	5

static tss_entry_t tss;

void TSS_Init()
{
	//GDT-Eintrag erstellen
	GDT_SetSystemDescriptor(SELECTOR, (uintptr_t)&tss, sizeof(tss), 0x89, 0x0);

	//Task-Segment Selector
	tr_t tr;
	tr.Selector = SELECTOR << 3;

	//TSS initialisieren
	tss.MapBaseAddress = 0x96;
	memset(tss.IOPD, 0, 8192);

	//Taskergister laden
	asm volatile("ltr %0" : : "m"(tr));
}

void TSS_setStack(void *stack)
{
	tss.rsp0 = stack;
}

#endif
