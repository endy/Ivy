
#include <windows.h>
#include <iostream>

static const int MaxThreads = 64;

typedef struct _ThreadData {
    UINT threadNum;
    HANDLE hStdoutMutex;
} ThreadData;


DWORD WINAPI threadBegin( LPVOID lparam )
{
    ThreadData* pThreadData = reinterpret_cast<ThreadData*>(lparam);

    srand(pThreadData->threadNum);

    bool runThread = true;
    while (runThread)
    {
        UINT randomNum = rand();

        DWORD waitResult = 0;
        waitResult = WaitForSingleObject( pThreadData->hStdoutMutex,    // handle to mutex
            INFINITE);                                                  // no time-out interval

        switch (waitResult) 
        {
            // The thread got ownership of the mutex
        case WAIT_OBJECT_0: 

            std::cout << "Thread " << pThreadData->threadNum << " Number: " 
                << randomNum << std::endl;

            ReleaseMutex(pThreadData->hStdoutMutex);

            break; 

            // The thread got ownership of an abandoned mutex
            // The database is in an indeterminate state
        case WAIT_ABANDONED: 
            runThread = FALSE;
            break; 
        default:
            // wut?
            break;
        }

    }

    return 0;
}

int main()
{
    ThreadData* pThreadData[MaxThreads];
    DWORD threadIdArray[MaxThreads];
    HANDLE hThreadArray[MaxThreads];

    // Create a mutex with no initial owner
    HANDLE hStdoutMutex = CreateMutex( NULL,	// default security attributes
        FALSE,   // initially not owned
        NULL);   // unnamed mutex

    if (hStdoutMutex == NULL) 
    {
        printf("CreateMutex error: %d\n", GetLastError());
        return 1;
    }


    for (int i=0; i < MaxThreads; ++i)
    {
        // Allocate thread data
        LPVOID pData = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(ThreadData));
        pThreadData[i] = reinterpret_cast<ThreadData*>(pData);

        if (pThreadData[i] == NULL)
        {
            // out of memory -- exit process entirely
            ExitProcess(EXIT_FAILURE);
        }

        pThreadData[i]->threadNum = i;
        pThreadData[i]->hStdoutMutex = hStdoutMutex;

        // Create the thread to begin execution on its own.

        hThreadArray[i] = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            threadBegin,       		// thread function name
            pThreadData[i],         // argument to thread function 
            0,                      // use default creation flags 
            &threadIdArray[i]);     // returns the thread identifier 

        if (hThreadArray[i] == NULL) 
        {
            // unable to create thread
            ExitProcess(EXIT_FAILURE);
        }
    }

    // Wait until all threads have terminated.
    DWORD waitReturn = WaitForMultipleObjects(MaxThreads, hThreadArray, TRUE, INFINITE);

    // Close all thread handles and free memory allocations

    if (waitReturn) 
    {
        DWORD lastError = GetLastError();
        if (lastError == ERROR_INVALID_PARAMETER)
        {
            printf("invalid param");
        }
    }

    for (int i=0; i < MaxThreads; i++)
    {
        CloseHandle(hThreadArray[i]);
        hThreadArray[i] = NULL;

        if(pThreadData[i] != NULL)
        {
            HeapFree(GetProcessHeap(), 0, pThreadData[i]);
            pThreadData[i] = NULL;    // Ensure address is not reused.
        }
    }

    // Close mutex handle
    CloseHandle(hStdoutMutex);
    hStdoutMutex = NULL;

    return 0;
}