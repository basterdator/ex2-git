#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "Ping.h"




#define NUM_THREADS 1
#define BRUTAL_TERMINATION_CODE 0x55
#define ERROR_CODE ((int)(-1))
#define SUCCESS_CODE ((int)(0))

static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	website *lpParameter,
	LPDWORD p_thread_id);

DWORD WINAPI Ping(LPVOID lpParam);



int main() 
{
	char argvlike[] = "ynet.co.il";
	website webby;
	webby.name = &argvlike;
	webby.reachable = 0;

	HANDLE p_thread_handles[NUM_THREADS];
	DWORD p_thread_ids[NUM_THREADS];
	DWORD wait_code;
	BOOL ret_val;
	size_t i;


	/* Prepare parameters for thread */


	p_thread_handles[0] = CreateThreadSimple(Ping, &webby, &p_thread_ids[0]);

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
	printf("reachable is %d\n", webby.reachable);

}


static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	website *lpParameter,
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
		lpParameter, /*  argument to thread function */
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */

	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
		exit(ERROR_CODE);
	}

	return thread_handle;
}