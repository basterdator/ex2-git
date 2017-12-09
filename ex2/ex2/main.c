/* ==============================================
Introduction to Systems Programming
Winter 2017-2018
TEL-AVIV UNIVERSITY
Exercise 2
Uri Cohen                 302825807
Anton Chaplianka          310224209
============================================== */

// Includes --------------------------------------------------------------------
#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#include <limits.h>
#include "Ping.h"

// Global Variables -----------------------------------------------------------
#define TIMEOUT_IN_MILI_SEC 10000

// Declerations  --------------------------------------------------------------
static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, LPDWORD p_thread_id, website *lpParameter);

// Main  ----------------------------------------------------------------------
int main(int argc, char *argv[]) {
	if (argc < 2)
	{
		printf("ERROR: Not enough input arguments");
		return -1;
	}
	int num_of_threads = argc - 1;
	HANDLE *p_thread_handles = malloc(sizeof(HANDLE)*num_of_threads);
	DWORD *p_thread_ids = malloc(sizeof(DWORD)*num_of_threads);
	website *websites_array = malloc(sizeof(website)*num_of_threads);
	DWORD *exitcode_array = malloc(sizeof(DWORD)*num_of_threads);
	int i;
	for (i = 0; i < num_of_threads; i++) { // initializing websites array
		websites_array[i].name = argv[i + 1];
		websites_array[i].reachable = 0;
	}
	for (i = 0; i < num_of_threads; i++) {
		p_thread_handles[i] = CreateThreadSimple(Ping, &p_thread_ids[i], &websites_array[i]);
	}
	DWORD waitcode;
	waitcode = WaitForMultipleObjects(num_of_threads, p_thread_handles, 1, TIMEOUT_IN_MILI_SEC);
	if (waitcode != WAIT_OBJECT_0) {
		printf("error wait for multiple objects");
		for (i = 0; i < num_of_threads; i++) /* If one of the thread dies we print the return codes of all threads in order to know exactly
											 which thread caused the main process to crash*/
		{
			GetExitCodeThread(p_thread_handles[i], &exitcode_array[i]);
			printf("Thread %d finished with exitcode 0x%x\n", i, exitcode_array[i]);
		}
		return -1;
	}

	for (i = 0; i < num_of_threads; i++) {  /* The print process itself, it happens outside the threads
											and has a mechanism that prevents a print of an extra line in the end of the print. */
		if (websites_array[i].reachable == 0) {
			if (i < num_of_threads - 1) {
				printf("%s unreachable\n", websites_array[i].name);
			}
			else {
				printf("%s unreachable", websites_array[i].name);
			}
		}
		else
		{
			if (i < num_of_threads - 1) {
				printf("%s reachable\n", websites_array[i].name);
			}
			else {
				printf("%s reachable", websites_array[i].name);
			}
		}
	}
	free(p_thread_handles); //  Freeing the allocated memory
	free(p_thread_ids);
	free(websites_array);
	free(exitcode_array);
	return 0;
}


static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPDWORD p_thread_id, website *lpParameter) // A wrapper to CreateThread() that also gets an input parameter
{
	return CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		p_start_routine, /*  thread function */
		lpParameter,     /*  argument to thread function */
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */
}