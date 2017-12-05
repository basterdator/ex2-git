// ThreadThatPings.h

#ifndef __CREATETHREADSIMPLE_H__
#define __CREATETHREADSIMPLE_H__

// Includes --------------------------------------------------------------------

#include <windows.h>

// Function Declarations -------------------------------------------------------

/*
* IO thread. This thread is in charge of the user interface.
* Notes:
*   The thread calls the Echo function in a loop.
*   If the user enters "exit", the thread exits.
*   Not all threads are an eternal loop. This is just for
*   for the sake of this example.
*/
static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPDWORD p_thread_id);

#endif // __CREATETHREADSIMPLE__