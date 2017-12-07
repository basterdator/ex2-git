#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "Ping.h"


#define TIMEOUT_IN_MILLISECONDS INFINITE
#define BRUTAL_TERMINATION_CODE 0x55

BOOL CreateProcessSimple(char *CommandLine, PROCESS_INFORMATION *ProcessInfoPtr);
void CreateProcessSimpleMain(char *name);


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



void CreateProcessSimpleMain(website *p_w)
{
	website w = *p_w;
	PROCESS_INFORMATION procinfo;
	DWORD				waitcode;
	DWORD				exitcode;
	BOOL				retVal;
	char				command[] = "ping -n 1 ";
	char *p_str = NULL;
	p_str = malloc((strlen(command) + strlen(w.name) )* sizeof(char) + 1);
 	strcpy(p_str, command);
	strcat(p_str,w.name);


																					
																					/* <ISP> TCHAR is a win32  */
																					/* generic char which may be either a simple (ANSI) char or a unicode char, */
																					/* depending on behind-the-scenes operating system definitions. Type LPTSTR */
																					/* is a string of TCHARs. Type LPCTSTR is a const string of TCHARs. */

																					/*  Start the child process. */
	retVal = CreateProcessSimple(p_str, &procinfo);
	free(p_str);

	if (retVal == 0)
	{
		printf("Process Creation Failed!\n");
		return;
	}


	waitcode = WaitForSingleObject(
		procinfo.hProcess,
		TIMEOUT_IN_MILLISECONDS); /* Waiting 5 secs for the process to end */

	printf("WaitForSingleObject output: ");
	switch (waitcode)
	{
	case WAIT_TIMEOUT:
		printf("WAIT_TIMEOUT\n"); break;
	case WAIT_OBJECT_0:
		printf("WAIT_OBJECT_0\n"); break;
	default:
		printf("0x%x\n", waitcode);
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

	printf("The exit code for the process is 0x%x\n", exitcode);
	if (exitcode == 0)
	{
		(*p_w).reachable = 1;
	}
	else
	{
		(*p_w).reachable = 0;
	}
	printf("reachable is %d\n", (*p_w).reachable);
	CloseHandle(procinfo.hProcess); /* Closing the handle to the process */
	CloseHandle(procinfo.hThread); /* Closing the handle to the main thread of the process */
}

BOOL CreateProcessSimple(char *CommandLine, PROCESS_INFORMATION *ProcessInfoPtr)
{
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 }; /* <ISP> here we */
															  /* initialize a "Neutral" STARTUPINFO variable. Supplying this to */
															  /* CreateProcess() means we have no special interest in this parameter. */
															  /* This is equivalent to what we are doing by supplying NULL to most other */
															  /* parameters of CreateProcess(). */

	return CreateProcess(NULL, /*  No module name (use command line). */
		CommandLine,			/*  Command line. */
		NULL,					/*  Process handle not inheritable. */
		NULL,					/*  Thread handle not inheritable. */
		FALSE,					/*  Set handle inheritance to FALSE. */
		NORMAL_PRIORITY_CLASS,	/*  creation/priority flags. */
		NULL,					/*  Use parent's environment block. */
		NULL,					/*  Use parent's starting directory. */
		&startinfo,				/*  Pointer to STARTUPINFO structure. */
		ProcessInfoPtr			/*  Pointer to PROCESS_INFORMATION structure. */
	);
}