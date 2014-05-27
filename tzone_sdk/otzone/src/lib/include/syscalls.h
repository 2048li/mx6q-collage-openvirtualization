#ifndef SYSCALLS_H
#define SYSCALLS_H

/* 
 * This Header file for NEWLIB_SUPPORT functions that needed by apps 
 */

/**
 * @brief function  will  flushes  the stream pointed to by fd
 *
 * @param file file descriptor of the file
 *
 * @return Upon successful completion 0 is returned.
 *			Otherwise, EOF is returned and errno is
 *			set  to  indicate the  error
 */
int _close(int file);

/**
 * @brief These functions return information about a file 
 *
 * @param file file descriptor of the file
 * @param st file pointed to by path and fills in st structure
 *
 * @return On success, zero is returned.  On error, -1 is returned,
 *			and errno is set appropriately
 */
int _fstat(int file, struct stat *st);

/**
 * @brief function tests whether fd is an open file descriptor
 *									referring to a terminal 
 *
 * @param file file descriptor of the file
 *
 * @return returns 1 if fd is an open file descriptor referring to
 *			a terminal; otherwise 0 is returned,and errno is set to
 *			indicate the error 
 */
int _isatty(int file);

/**
 * @brief function repositions the offset of the open file associated
 *			with the file descriptor fd 
 *
 * @param file file descriptor of the file
 * @param offset offset assigned according to whence as follows
 *					SEEK_SET,SEEK_CUR and SEEK_END
 * @param flags Moving position from offset 
 *
 * @return Upon successful completion, lseek() returns the resulting
 *			offset location as measured in  bytes  from the beginning
 *			of the file.  Otherwise, a value of (off_t) -1 is returned
 *			and errno is set to indicate the error 
 */
off_t _lseek(int file, off_t offset, int flags);

/**
 * @brief function  opens  the  file whose name is the string
 *			pointed to by path and associates a stream with it 
 *
 * @param name name of the file to be open
 * @param flags Mode Translated flag value for file to be open
 * @param ... optional third argument
 *
 * @return Returns the Index the of the FILE structure Otherwise,
 *			NULL is returned and errno is set to indicate the error
 */
int _open(const char *name, int flags, ...);

/**
 * @brief reads nmemb elements of data, each size bytes long,
 *			from the stream pointed to by stream, storing them
 *			at the location given by fd 
 *
 * @param file file descriptor of the file
 * @param ptr readed data stores from memory location of ptr
 * @param len length of data to be read 
 *
 * @return the number of items successfully read
 *			If an error occurs, or the end-of-file is reached
 *			the return value  is  a  short  item count (or zero) 
 */
int _read(int file, char *ptr, int len);

/**
 * @brief change data segment size 
 *
 * @param incr change of the location of the program break,
 *              which defines the end of the process's data segment
 *
 * @return On success,returns  zero.
 *          On error, -1 is returned, and errno is set.
 */
caddr_t _sbrk(int incr);

/**
 * @brief writes nmemb elements of data, each size bytes long,
 *			to the stream  pointed  to by stream, obtaining them
 *			from the location given by ptr
 *
 * @param file file descriptor of the file
 * @param ptr contains data to be write in a file
 * @param len length of data to be read
 *
 * @return  the number of items successfully read
 *          If an error occurs, or the end-of-file is reached
 *          the return value  is  a  short  item count (or zero)
 */
int _write(int file, char *ptr, int len);

/**
 * @brief the command does nothing beyond expanding arguments
 *              and performing  any  specified redirections
 *
 * @param val file descriptor of the file
 *
 * @return  A zero exit code is returned 
 */
void _exit(int val) __attribute__ ((noreturn));

/**
 * @brief executes  the program pointed to by filename 
 *
 * @param name : file name of the file
 * @param argv : an array of argument strings passed to the new program
 * @param env : an array of strings, conven‐tionally of the form
 *			key=value, which are passed as
 *			environment to the new program
 *
 * @return On success, _execve() does not return, on error -1 is returned,
 *			and errno is set appropriately 
 */
int _execve(char *name, char **argv, char **env);

/**
 * @brief returns the process ID of the calling process 
 *
 * @return returns the process ID of the parent of the calling process 
 */
int _getpid();

/**
 * @brief creates a new process by duplicating the calling process 
 *
 * @return On success, the PID of the child process is returned in the parent,
 *			and 0 is returned in  the  child.On  failure,  -1  is  returned
 *			in the parent, no child process is created, and errno is set
 *			appropriately 
 */
int _fork(void);

/**
 * @brief terminate a process.Sends the specified signal to
 *			the specified process or process group 
 *
 * @param pid Specify the list of processes that kill should signal
 * @param sig Specify the signal to send.  The signal may be given
 *				as a signal name or number
 *
 * @return If sig is 0, then no signal is sent,
 *			but error checking is still performed 
 */
int _kill(int pid, int sig);

/**
 * @brief The parent process may then issue a wait system call,
 *			which suspends the execution of the parent process
 *				while the child executes 
 *
 * @param status status of the process
 *
 * @return On success, _wait() does not return, on error -1 is returned,
 *          and errno is set appropriately
 */
int _wait(int *status);

/**
 * @brief call the link function to create a link to a file 
 *
 * @param old old existing file
 * @param new New file to be link with old one
 *
 * @return On success, _wait() does not return, on error -1 is returned,
 *          and errno is set appropriately
 */
int _link(char *old, char *new);

/**
 * @brief These functions return information about a file 
 *
 * @param file file descriptor of the file
 * @param st file pointed to by path and fills in st structure
 *
 * @return On success, zero is returned.  On error, -1 is returned,
 *          and errno is set appropriately
 */
int _stat(const char* file, struct stat *st);

/**
 * @brief  call the unlink function to remove the specified file
 *
 * @param name name of the file to be unlink
 *
 * @return On success,return 0, on error -1 is returned,
 *          and errno is set appropriately
 */
int _unlink(char *name);

/**
 * @brief giving timing statistics about this program run 
 *
 * @param buf timezone structure contains information of the program
 *					runtime in term of seconds and microseconds
 *
 * @return On success, _times() does not return, on error -1 is returned,
 *          and errno is set appropriately
 *
 */
clock_t _times(struct tms *buf);

/**
 * @brief get time of the day
 *
 * @param buf timezone structure contains information of the program
 *					runtime in term of seconds and microseconds
 * @param buf2  corresponding timezone structure contains information of
 *				the program runtime in term of minutes west of Greenwich
 *				with type of DST correction
 *
 * @return  On success, _times() does not return, on error -1 is returned,
 *          and errno is set appropriately 
 */
int _gettimeofday(struct timeval *buf, void *buf2);

/**
 * @brief finds the resolution (precision) of the specified clock clk_id
 *
 * @param clk_id structue contains id of specific clock
 * @param tp structue  contains information of the program
 *                  runtime in term of seconds and nanoseconds 
 *
 * @return  On success, clock_gettime() does not return, on error -1 is
 *                      returned and errno is set appropriately 
 *
 */
int clock_gettime(clockid_t clk_id, struct timespec *tp);

#endif
