/* 
 * OpenVirtualization: 
 * For additional details and support contact developer@sierraware.com.
 * Additional documentation can be found at www.openvirtualization.org
 * 
 * Copyright (C) 2011 SierraWare
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * syscalls implementation
 *
 */

#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/errno.h>
#include "fat32.h"
#include <sw_string_functions.h>
#include <mem_mng.h>
#include <sw_buddy.h>
#include <sw_mem_functions.h>
#include <sw_debug.h>
#include <unused.h>


#define SEEK_SET    0    /* Seek from beginning of file.  */
#define SEEK_CUR    1    /* Seek from current position.  */
#define SEEK_END    2    /* Seek from end of file.  */


#define   FILE_CNTR_START_INDEX   3
#define   MAX_OPEN_FILES   (32+FILE_CNTR_START_INDEX)
#define   MAX_FILENAME_LENGTH   255

char *__env[1] = {0};
char **environ = __env;
 
typedef struct 
{
	int fd;
}FILE;

FILE file_pointers[MAX_OPEN_FILES];
u8 file_names[MAX_OPEN_FILES][MAX_FILENAME_LENGTH];

/**
 * @brief function  will  flushes  the stream pointed to by fd
 *
 * @param file file descriptor of the file
 *
 * @return Upon successful completion 0 is returned.
 *                      Otherwise, EOF is returned and errno is
 *                      set  to  indicate the  error
 */
int _close(int file) 
{
#ifdef CONFIG_FILESYSTEM_SUPPORT
    int retVal = 0;
    if(file_names[file][0] == '\0') {
        errno = EBADF;
        return(-1);
    }
    retVal = file_close(file_pointers[file].fd);
    file_names[file][0] = '\0';
    if(retVal != -1) {
        errno = 0;
        return(0);
    } else {
        /*todo set errno */
        return(-1);
    }
#else
        sw_printk("file system configuration is not defined. \
                Enable CONFIG_FILESYSTEM_SUPPORT.\n");


        errno= EBADF;
        return (-1);
#endif
}

/**
 * @brief These functions return information about a file 
 *
 * @param file file descriptor of the file
 * @param st file pointed to by path and fills in st structure
 *
 * @return On success, zero is returned.  On error, -1 is returned,
 *                      and errno is set appropriately
 */

int _fstat(int file, struct stat *st) 
{
    /*FILINFO file_info;

    if((file == SW_STDERR) ||(file == SW_STDOUT) || (file == SW_STDIN)) {
        st->st_mode = S_IFCHR;
    } else {
        if(file_names[file][0] == '\0') {
            errno = EBADF;
            return(-1);
        }
#ifdef NOT_IMPLEMENTED
        file_fstat(file_names[file],&file_info);
        st->st_mode = S_IFREG;
        st->st_size = file_info.fsize;
        st->st_mtime = file_info.ftime;
#endif
    }
    errno = 0;*/
    return(0);
}

/**
 * @brief function tests whether fd is an open file descriptor
 *                                                                      referring to a terminal 
 *
 * @param file file descriptor of the file
 *
 * @return returns 1 if fd is an open file descriptor referring to
 *                      a terminal; otherwise 0 is returned,and errno is set to
 *                      indicate the error 
 */
int _isatty(int file) 
{
    if((file == SW_STDERR) ||(file == SW_STDOUT) || (file == SW_STDIN)) {
        errno = 0;
        return(1);
    }
    if(file < MAX_OPEN_FILES){
        errno = ENOTTY;
        return(0);
    }
    errno = EBADF;
    return(0);
}

/**
 * @brief function repositions the offset of the open file associated
 *                      with the file descriptor fd 
 *
 * @param file file descriptor of the file
 * @param offset offset assigned according to whence as follows
 *                                      SEEK_SET,SEEK_CUR and SEEK_END
 * @param flags Moving position from offset 
 *
 * @return Upon successful completion, lseek() returns the resulting
 *                      offset location as measured in  bytes  from the beginning
 *                      of the file.  Otherwise, a value of (off_t) -1 is returned
 *                      and errno is set to indicate the error 
 */
off_t _lseek(int file, off_t offset, int flags)
{
    off_t local_offset = offset;
    if((file_pointers[file].fd == SW_STDERR) ||
			(file_pointers[file].fd == SW_STDOUT) || 
			(file_pointers[file].fd == SW_STDIN)) {
        return(offset);
    }
#ifdef CONFIG_FILESYSTEM_SUPPORT
    if(file_names[file][0] == '\0') {
        errno = EBADF;
        return(-1);
    }
    if((local_offset = file_seek(file_pointers[file].fd,(int)offset, 
								flags)) == -1) {
        /*todo set errno appropriately */
        return(-1);
    }
#else
        sw_printk("file system configuration is not defined. \
                Enable CONFIG_FILESYSTEM_SUPPORT.\n");

        errno= EBADF;
        return (-1);
#endif
    errno = 0;
    return(local_offset);
}


/**
 * @brief Using This function get current free
 *                      FILE structure  
 *
 * @return returns current free index of FILE structure
 */
static int get_free_index()
{
	int i = FILE_CNTR_START_INDEX;

	while(i < MAX_OPEN_FILES) {
		if(file_names[i][0] == '\0') {
			return(i);
		}
		i += 1;
	}
	return(0);
}

/**
 * @brief function  opens  the  file whose name is the string
 *                      pointed to by path and associates a stream with it 
 *
 * @param name name of the file to be open
 * @param flags Mode Translated flag value for file to be open
 * @param ... optional third argument
 *
 * @return Returns the Index the of the FILE structure Otherwise,
 *                      NULL is returned and errno is set to indicate the error
 */
int _open(const char *name, int flags, ...) 
{ 
#ifdef CONFIG_FILESYSTEM_SUPPORT
    int free_index = 0;
    u8 mode_val;

    if((free_index = get_free_index()) == 0) {
        errno = EMFILE;
        return(-1);
    }
	switch(flags) {
        case 0: 
            mode_val = FILE_READ;
            break;
        case 2: 
            mode_val = FILE_READ | FILE_WRITE;
            break;
        case 1537:
            mode_val = FILE_WRITE | FILE_CREATE_ALWAYS | FILE_CREATE_NEW;
            break;
        case 1538:
            mode_val = FILE_READ | FILE_WRITE | FILE_CREATE_ALWAYS | 
					   FILE_CREATE_NEW;
            break;
        default:
            errno = EINVAL;
            return(-1);
    }
	
    if((file_pointers[free_index].fd = file_open(name, mode_val)) == -1) {
        /*todo map the return code to an error number */
        return(-1);
    }
    sw_strcpy((char*)file_names[free_index],(char*)name);
    errno = 0;
    return(free_index);
#else
        sw_printk("file system configuration is not defined. \
                Enable CONFIG_FILESYSTEM_SUPPORT.\n");

        errno= EBADF;
        return (-1);
#endif
}


/**
 * @brief reads nmemb elements of data, each size bytes long,
 *                      from the stream pointed to by stream, storing them
 *                      at the location given by fd 
 *
 * @param file file descriptor of the file
 * @param ptr readed data stores from memory location of ptr
 * @param len length of data to be read 
 *
 * @return the number of items successfully read
 *                      If an error occurs, or the end-of-file is reached
 *                      the return value  is  a  short  item count (or zero) 
 */
int _read(int file, char *ptr, int len)
{
	int bytes_read = 0;

	if(len == 0)
		return 0;
	if((file_pointers[file].fd == SW_STDERR) ||
			(file_pointers[file].fd == SW_STDOUT) || 
			(file_pointers[file].fd == SW_STDIN)) {
		/*while(UART_FR(UART0_ADDR) & UART_FR_RXFE);
		*ptr++ = UART_DR(UART0_ADDR);
		for(bytes_read = 1; bytes_read < len; bytes_read++) {
			if(UART_FR(UART0_ADDR) & UART_FR_RXFE) {
				break;
			}
			*ptr++ = UART_DR(UART0_ADDR);
		}*/
	} else {
#ifdef CONFIG_FILESYSTEM_SUPPORT
	    if(file_names[file][0] == '\0') {
	        errno = EBADF;
	        return(-1);
	    }
	    bytes_read=file_read(file_pointers[file].fd,ptr,len);
#else
        sw_printk("file system configuration is not defined. \
                Enable CONFIG_FILESYSTEM_SUPPORT.\n");

        errno= EBADF;
        return (-1);
#endif
	}
	errno = 0;
	return(bytes_read);
}

unsigned char *heap_end = 0;
unsigned char *heap_low = 0,*heap_top = 0;

/**
 * @brief sbrk system call implementation
 *
 * @param incr address of the location of the program break,
 *                              which defines the end of the process's data segment
 *
 * @return On success,returns  zero.
 *                      On error, -1 is returned, and errno is set.
 */
caddr_t sbrk(int incr) 
{
	unsigned char *prev_heap_end; 
	if(heap_low == 0) {
		heap_low = (unsigned char*)sw_vir_page_alloc(1<<20);
		heap_top = heap_low + (1<<20);
	}
	if (heap_end == 0) {
		heap_end = heap_low;
	}
	prev_heap_end = heap_end;
	if (heap_end + incr > heap_top) {
		/* Heap and stack collision */
		return (caddr_t)0;
	} 
	heap_end += incr;
	return (caddr_t) prev_heap_end;
}

/**
 * @brief change data segment size 
 *
 * @param incr change of the location of the program break,
 *              which defines the end of the process's data segment
 *
 * @return On success,returns  zero.
 *          On error, -1 is returned, and errno is set.
 */
caddr_t _sbrk(int incr) 
{
	return sbrk(incr);
}

/**
 * @brief writes nmemb elements of data, each size bytes long,
 *                      to the stream  pointed  to by stream, obtaining them
 *                      from the location given by ptr
 *
 * @param file file descriptor of the file
 * @param ptr contains data to be write in a file
 * @param len length of data to be read
 *
 * @return  the number of items successfully read
 *          If an error occurs, or the end-of-file is reached
 *          the return value  is  a  short  item count (or zero)
 */
int _write(int file, char *ptr, int len) 
{
    int todo = 0;
    u8 *local_buff = NULL;

    if((file_pointers[file].fd == SW_STDERR) || 
			(file_pointers[file].fd == SW_STDOUT) || 
			(file_pointers[file].fd == SW_STDIN)) {
        if((local_buff = (u8*)sw_malloc((len+1)*sizeof(u8))) != NULL) {
            sw_memcpy(local_buff,ptr,len);
            local_buff[len] = '\0';
            sw_printf((char*)local_buff);
            sw_free(local_buff);
        }
        /*for (todo = 0; todo < len; todo++) {
            UART_DR(UART0_ADDR) = *ptr++;
        }*/
        todo = len;
    } else {
#ifdef CONFIG_FILESYSTEM_SUPPORT
        if(file_names[file][0] == '\0') {
            errno = EBADF;
            return(-1);
        }
        todo=file_write(file_pointers[file].fd,ptr,len);
#else
		sw_printk("file system configuration is not defined. \
				Enable CONFIG_FILESYSTEM_SUPPORT.\n");

		errno= EBADF;
		return (-1);
#endif
}
    errno = 0;
    return(todo);
}

/**
 * @brief the command does nothing beyond expanding arguments
 *              and performing  any  specified redirections
 *
 * @param val file descriptor of the file
 *
 * @return  A zero exit code is returned 
 */
void _exit(int val)
{
UNUSED(val);
while(1);
}

/**
 * @brief executes  the program pointed to by filename 
 *
 * @param name : file name of the file
 * @param argv : is an array of argument strings passed to the new program
 * @param env : an array of strings, conven‐tionally of the form
 *                      key=value, which are passed as
 *                      environment to the new program
 *
 * @return On success, _execve() does not return, on error -1 is returned,
 *                      and errno is set appropriately 
 */
int _execve(char *name, char **argv, char **env)
{
	errno = ENOMEM;
	return -1;
}

#define __MYPID 1

/**
 * @brief returns the process ID of the calling process 
 *
 * @return returns the process ID of the parent of the calling process 
 */
int _getpid()
{
  return __MYPID;
}


/**
 * @brief creates a new process by duplicating the calling process 
 *
 * @return On success, the PID of the child process is returned in the parent,
 *                      and 0 is returned in  the  child.On  failure,  -1  is  returned
 *                      in the parent, no child process is created, and errno is set
 *                      appropriately 
 */
int _fork(void)
{
	errno = ENOTSUP;
	return -1;
}

/**
 * @brief terminate a process.Sends the specified signal to
 *                      the specified process or process group 
 *
 * @param pid Specify the list of processes that kill should signal
 * @param sig Specify the signal to send.  The signal may be given
 *                              as a signal name or number
 *
 * @return If sig is 0, then no signal is sent,
 *                      but error checking is still performed 
 */
int _kill(int pid, int sig)
{
	if(pid == __MYPID)
		_exit(sig);
	errno = EINVAL;
	return -1; 
}

/**
 * @brief The parent process may then issue a wait system call,
 *                      which suspends the execution of the parent process
 *                              while the child executes 
 *
 * @param status status of the process
 *
 * @return On success, _wait() does not return, on error -1 is returned,
 *          and errno is set appropriately
 */
int _wait(int *status)
{ 
	errno = ECHILD;
	return -1;
}

/**
 * @brief call the link function to create a link to a file 
 *
 * @param old old existing file
 * @param new New file to be link with old one
 *
 * @return On success, _wait() does not return, on error -1 is returned,
 *          and errno is set appropriately
 */
int _link(char *old, char *new)
{
	errno = EMLINK;
	return -1;
}

/**
 * @brief These functions return information about a file 
 *
 * @param file file descriptor of the file
 * @param st file pointed to by path and fills in st structure
 *
 * @return On success, zero is returned.  On error, -1 is returned,
 *          and errno is set appropriately
 */
int _stat(const char* file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

/**
 * @brief  call the unlink function to remove the specified file
 *
 * @param name name of the file to be unlink
 *
 * @return On success,return 0, on error -1 is returned,
 *          and errno is set appropriately
 */
int _unlink(char *name)
{
#ifdef CONFIG_FILESYSTEM_SUPPORT
	int ul_val;
	ul_val=file_remove(name);
	if(ul_val!=-1)
	{
		sw_printf("file is unlink from file system successfully\n");
		return 0;
	}
	errno = ENOENT;
	sw_printf("file is not unlink from file system successfully\n");
	return -1;
#else
        sw_printk("file system configuration is not defined. \
                Enable CONFIG_FILESYSTEM_SUPPORT.\n");

        errno= EBADF;
        return (-1);
#endif
}

/**
 * @brief giving timing statistics about this program run 
 *
 * @param buf timezone structure contains information of the program
 *                                      runtime in term of seconds and microseconds
 *
 * @return On success, _times() does not return, on error -1 is returned,
 *          and errno is set appropriately
 *
 */
clock_t _times(struct tms *buf)
{
	errno = ENOTSUP;
	return -1;
}

/**
 * @brief get time of the day
 *
 * @param buf timezone structure contains information of the program
 *                                      runtime in term of seconds and microseconds
 * @param buf2  corresponding timezone structure contains information of
 *                              the program runtime in term of minutes west of Greenwich
 *                              with type of DST correction
 *
 * @return  On success, _times() does not return, on error -1 is returned,
 *          and errno is set appropriately 
 */
int _gettimeofday(struct timeval *buf, void *buf2)
{
	errno = ENOTSUP;
	return -1;
}

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
int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
	errno = ENOTSUP;
	return(-1);
}

