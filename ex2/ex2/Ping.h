/* ==============================================
Introduction to Systems Programming
Winter 2017-2018
TEL-AVIV UNIVERSITY
Exercise 2
Uri Cohen                 302825807
Anton Chaplianka          310224209
============================================== */

#ifndef __PING_H__
#define __PING_H__

// Includes --------------------------------------------------------------------
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

// Structs ---------------------------------------------------------------------
typedef struct website_s {
	char *name;
	int reachable;
}website;

// Function Declarations -------------------------------------------------------

DWORD WINAPI Ping(LPVOID lpParam);

#endif // __PING_H__
