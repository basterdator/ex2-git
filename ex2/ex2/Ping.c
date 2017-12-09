/* ==============================================
Introduction to Systems Programming
Winter 2017-2018
TEL-AVIV UNIVERSITY
Exercise 2
Uri Cohen                 302825807
Anton Chaplianka          310224209
============================================== */

// No secure warnings, so we could use strcpy and strcat -----------------------
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
// Includes --------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "Ping.h"

// Global Variables ------------------------------------------------------------

#define TIMEOUT_IN_MILLISECONDS 10000
#define BRUTAL_TERMINATION_CODE 0x55

// Declerations  --------------------------------------------------------------
BOOL CreateProcessSimple(char *CommandLine, PROCESS_INFORMATION *ProcessInfoPtr);
void CreateProcessSimpleMain(website *p_w);

// The function Ping itself  ---------------------------------------------------

DWORD WINAPI Ping(LPVOID lpParam)
{
	website *p_w;
	p_w = (website *)lpParam;
	CreateProcessSimpleMain(p_w);
	if ((*p_w).reachable == 0)
	{
		CreateProcessSimpleMain(p_w);
	}
	return 0;
}

// Sub-functions ---------------------------------------------------------------

void CreateProcessSimpleMain(website *p_w) // A wrapper for CreateProcess()
{
	website w = *p_w;
	PROCESS_INFORMATION procinfo;
	DWORD				waitcode;
	DWORD				exitcode;
	BOOL				retVal;
	char				command[] = "ping -n 1 ";
	char *p_str = NULL;
	p_str = malloc((strlen(command) + strlen(w.name)) * sizeof(char) + 1); /* we concatenate the two strings that create the command ==>
																		   "ping" + "wbsite" */
	strcpy(p_str, command);
	strcat(p_str, w.name);

	/*  Start the child process. */
	retVal = CreateProcessSimple(p_str, &procinfo);
	free(p_str);			/* The process has ended, we don't need the string anymore so we free the allocated memory */

	if (retVal == 0)
	{
		printf("Process Creation Failed!\n");
		return;
	}


	waitcode = WaitForSingleObject(
		procinfo.hProcess,
		TIMEOUT_IN_MILLISECONDS); /* Waiting 10 secs for the process to end */

	switch (waitcode)
	{
	case WAIT_TIMEOUT:
		printf("WAIT_TIMEOUT\n"); break;
	case WAIT_OBJECT_0:
		break;
	}

	if (waitcode == WAIT_TIMEOUT) /* Process is still alive */
	{
		printf("Process was not terminated before timeout!\n"
			"Terminating brutally!\n");
		TerminateProcess(
			procinfo.hProcess,
			BRUTAL_TERMINATION_CODE); /* Terminating process with an exit code of 55h */
		Sleep(10); /* Waiting a few milliseconds for the process to terminate */
	}

	GetExitCodeProcess(procinfo.hProcess, &exitcode);

	if (exitcode == 0)
	{
		(*p_w).reachable = 1; /* We update the struct directly */
	}
	else
	{
		(*p_w).reachable = 0;
	}
	CloseHandle(procinfo.hProcess); /* Closing the handle to the process */
	CloseHandle(procinfo.hThread); /* Closing the handle to the main thread of the process */
}

BOOL CreateProcessSimple(char *CommandLine, PROCESS_INFORMATION *ProcessInfoPtr)
{
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 };

	return CreateProcess(NULL,
		CommandLine,			/*  Command line. */
		NULL,
		NULL,
		FALSE,
		CREATE_NO_WINDOW,	/*  We use this flag to disable pings prints */
		NULL,
		NULL,
		&startinfo,				/*  Pointer to STARTUPINFO structure. */
		ProcessInfoPtr			/*  Pointer to PROCESS_INFORMATION structure. */
	);
}