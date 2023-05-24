#include <libcrt.h>

struct signal_struct_t
{
    int   code;
    char *message;
};

struct signal_struct_t sign_str[] = {
  	{SIGINT	    , "Interactive attention signal.  "},
	{SIGILL		, "Illegal instruction.  "},
	{SIGABRT		, "Abnormal termination.  "},
	{SIGFPE		, "Erroneous arithmetic operation.  "},
	{SIGSEGV		, "Invalid access to storage.  "},
	{SIGTERM		, "Termination request.  "},

	{SIGHUP		, "Hangup.  "},
	{SIGQUIT		, "Quit.  "},
	{SIGTRAP		, "Trace/breakpoint trap.  "},
	{SIGKILL		, "Killed.  "},
    {SIGBUS		, "Bus error. "},
	{SIGSYS		, "Bad system call.  "},
	{SIGPIPE		, "Broken pipe.  "},
	{SIGALRM		, "Alarm clock.  "},

	{SIGURG		, "Urgent data is available at a socket.  "},
	{SIGSTOP		, "Stop, unblockable.  "},
	{SIGTSTP		, "Keyboard stop.  "},
	{SIGCONT		, "Continue.  "},
	{SIGCHLD		, "Child terminated or stopped.  "},
	{SIGTTIN		, "Background read from control terminal.  "},
	{SIGTTOU		, "Background write to control terminal. "},
	{SIGPOLL		, "Pollable event occurred (System V).  "},
	{SIGXCPU		, "CPU time limit exceeded.  "},
	{SIGXFSZ		, "File size limit exceeded.  "},
	{SIGVTALRM	, "Virtual timer expired.  "},
	{SIGPROF		, "Profiling timer expired.  "},
	{SIGUSR1		, "User-defined signal 1.  "},
	{SIGUSR2		, "User-defined signal 2.  "}
};

CRT_FUNCDECL(void, libcrt_signal_handle,(int sign))
{
    uint32_t i=0;
    for(i=0;i<28;i++){
    
        if(sign == sign_str[i].code){
            printf("SIGNAL: %s \n", sign_str[i].message);
            break;
        }
    }
    
    exit(0);
    return ;
}
