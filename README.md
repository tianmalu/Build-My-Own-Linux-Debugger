# Build-My-Own-Linux-Debugger
Build My Own Linux Debugger 

## ptrace
ptrace allows us to observe and control the execution of another process by reading registers, reading memory, single stepping and more

Ptrace 可以让父进程控制子进程运行，并可以检查和改变子进程的核心image的功能（Peek and poke 在系统编程中是很知名的叫法，指的是直接读写内存内容）。ptrace主要跟踪的是进程运行时的状态，直到收到一个终止信号结束进程，这里的信号如果是我们给程序设置的断点，则进程被中止，并且通知其父进程，在进程中止的状态下，进程的内存空间可以被读写。当然父进程还可以使子进程继续执行，并选择是否忽略引起中止的信号，ptrace可以让一个进程监视和控制另一个进程的执行,并且修改被监视进程的内存、寄存器等,主要应用于断点调试和系统调用跟踪，strace和gdb工具就是基于ptrace编写的！

ptrace()其实是linux的一种系统调用，所以当我们用gdb进行attach其他进程的时候，需要root权限。

'''
long ptrace(enum __ptrace_request request, pid_t pid,
            void *addr, void *data);
'''

'pid' is the process ID of the traced process; 'addr' is a memory address, which is used in some calls to designate an address in the tracee; and 'data' is some request-specific resource. The return value often gives error information.