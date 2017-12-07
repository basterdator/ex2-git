#ifndef __PING_H__
#define __PING_H__
// Includes --------------------------------------------------------------------

#include <windows.h>
// Structs ---------------------------------------------------------------------
typedef struct website_s {
	char *name;
	int reachable;
}website;

typedef enum
{
	MATH_THREAD__CODE_SUCCESS,
	MATH_THREAD__CODE_NULL_PTR,
} MATH_THREAD__return_code_t;

// Function Declarations -------------------------------------------------------



DWORD WINAPI Ping(LPVOID lpParam);

#endif // __PING_H__
