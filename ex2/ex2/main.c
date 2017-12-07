#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "CreateThreadSimple.h"
#include "Ping.h"

typedef struct website_s {
	char *name;
	int *reachable;
}website;



#define NUM_THREADS 1
#define BRUTAL_TERMINATION_CODE 0x55
#define ERROR_CODE ((int)(-1))

static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPDWORD p_thread_id);

int main() 
{
	HANDLE p_thread_handles[NUM_THREADS];
	DWORD p_thread_ids[NUM_THREADS];
	DWORD wait_code;
	BOOL ret_val;
	size_t i;

	p_thread_handles[0] = CreateThreadSimple(Ping, &p_thread_ids[0]);

	wait_code = WaitForSingleObject(p_thread_handles[0], INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting");
		return ERROR_CODE;
	}

	for (i = 0; i < NUM_THREADS; i++)
	{
		ret_val = CloseHandle(p_thread_handles[i]);
		if (false == ret_val)
		{
			printf("Error when closing\n");
			return ERROR_CODE;
		}
	}

}


static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPDWORD p_thread_id)
{
	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	thread_handle = CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		p_start_routine, /*  thread function */
		NULL,            /*  argument to thread function */
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */

	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
		exit(ERROR_CODE);
	}

	return thread_handle;
}