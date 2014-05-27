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
 */

/* This file contains those libc functions that may be needed by other apps */
#include "libc_sos.h"
#include "sw_types.h"
#include "sw_buddy.h"
#include "fat32.h"
#include "sw_mem_functions.h"
#include "sw_string_functions.h"
#include <elf_loader.h>
#include "sw_semaphores.h"
#include "sw_debug.h"
#include "otz_api.h"
#include "sw_syscall.h"
#include "sw_timer_functions.h"
#define MIN(x,y) ((y) ^ (((x) ^ (y)) & -((x) < (y))))

/*
* @brief copies count characters from src to dest 
*
* @param dest
* @param src
* @param count
*
* @return 
*/
void* memmove(void *dest, const void *src, u32 count)
{
	return(sw_memmove(dest,src,count));
}

/**
* @brief copies the character c to the first n characters of the string pointed to dest
*
* @param dest
* @param c
* @param count
*
* @return 
*/
void* memset(void *dest, u32 c, u32 count)
{
	return(sw_memset(dest,c,count));
}

/**
* @brief copies count characters from memory area src to dest
*
* @param dst
* @param src
* @param count
*
* @return 
*/
void* memcpy(void *dst, const void *src, u32 count)
{
	return(sw_memcpy(dst,src,count));
}

/**
* @brief allocates the requested size of memory and returns a pointer to it
*
* @param size
*
* @return 
*/
void* malloc(u32 size)
{
	return(sw_malloc(size));
}

/**
* @brief Resizes the memory block pointed to by ptr to size 
*	that was previously allocated by malloc or calloc
* @param ptr
* @param size
*
* @return 
*/
void* realloc(void* ptr, u32 size)
{
	void* local_ptr = NULL;
	u32 old_size = 0;

	if(ptr == NULL) {
		return(sw_malloc(size));
	}
	if((size == 0) && (ptr != NULL)) {
		sw_free(ptr);
		return(NULL);
	}
	local_ptr = sw_malloc(size);
	old_size = get_ptr_size(ptr);
	sw_memcpy(local_ptr,ptr,MIN(old_size,size));
	sw_free(ptr);
	return(local_ptr);
}

/**
* @brief allocates the requested memory and returns a pointer to it 
*		and sets the memory to zero
* @param num_elements
* @param element_size
*
* @return 
*/
void* calloc(u32 num_elements, u32 element_size)
{
	void* old_ptr = sw_malloc(num_elements*element_size);
	if(old_ptr == NULL) {
		return(NULL);
	}
	sw_memset(old_ptr,0,num_elements*element_size);
	return(old_ptr);
}

/**
* @brief Deallocates the memory previously allocated 
*
* @param ptr
*/
void free(void* ptr)
{
	sw_free(ptr);
}

/**
* @brief compares the first n characters of string s1 and string s2 
*
* @param s1
* @param s2
* @param n
*
* @return equal to zero if s1 equals s2 
*	less than zero if s1 is less than s2 
*	greater than zero if s1 greater than s2
*/
int strncmp(const char *s1, const char *s2, u32 n)
{
	return(sw_strncmp((char*)s1,(char*)s2,(s32int)n));
}

/**
* @brief converts the string argument pointed to ptr to integer
*
* @param ptr
*
* @return 
*/
u32 atoi(const char *ptr)
{
	return(sw_strtoi((char*)ptr));
}

/**
* @brief computes and returns the length of the string pointed to ptr
*
* @param ptr
*
* @return 
*/
u32 strlen(const char *ptr)
{
	return(sw_strlen((char*)ptr));
}

/**
* @brief copies the string pointed to by ptr2 to ptr1
*
* @param ptr1
* @param ptr2
*
* @return 
*/
char* strcpy(char* ptr1, const char* ptr2)
{
	return(sw_strcpy(ptr1,(char*)ptr2));
}

/**
* @brief compares the string pointed to by ptr1 to the 
*		string pointed to by ptr2
* @param ptr1
* @param ptr2
*
* @return 
*/
int strcmp(const char* ptr1, const char* ptr2)
{
	return(sw_strcmp(ptr1,ptr2));
}

/**
* @brief appends the string pointed to by ptr2 to the end of string pointed to by dest
*
* @param ptr1
* @param ptr2
*
* @return 
*/
char* strcat(char *ptr1, const char *ptr2)
{
	return(sw_strcat(ptr1,ptr2));
}

/**
* @brief initializes the mutex parameters  
*
* @param mutex
* @param attribute
*
* @return 
*/
s32int pthread_mutex_init(pthread_mutex_t *mutex, 
				const pthread_mutexattr_t *attribute)
{
	return(sw_mutex_init((sw_mutex_t*)mutex,(sw_mutexattr_t*)attribute));
}

/**
* @brief This function is called to destroy the mutex once it is used 
*		If it is under lock it returns an error code
*		init param is reinitialized to zero
* @param mutex
*
* @return 
*/
s32int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
	return(sw_mutex_destroy((sw_mutex_t*)mutex));
}

/**
* @brief This function is called to lock the specified mutex 
*
* @param mutex
*
* @return 
*/
s32int pthread_mutex_lock(pthread_mutex_t *mutex)
{
	return(sw_mutex_lock((sw_mutex_t*)mutex));
}

/**
* @brief This function is called to unlock a mutex 
*
* @param mutex
*
* @return 
*/
s32int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	return(sw_mutex_unlock((sw_mutex_t*)mutex));
}

/**
* @brief Same as sw_mutex_lock but it does not block the calling function
*		It returns an error code if the mutex is already locked
* @param mutex
*
* @return 
*/
s32int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
	return(sw_mutex_trylock((sw_mutex_t*)mutex));
}

/**
* @brief Semaphore initialization
*	Following successful call, other semaphore operations 
*	can be performed until semaphore is destroyed
*
* @param sem
* @param shared
* @param value
*
* @return 
*/
s32int sem_init(sem_t *sem, s32int shared, u32 value)
{
	return(sw_sem_init((sw_sem_t*)sem,shared,value));
}

/**
* @brief Locks the semaphore referenced by sem 
*	It will return to the calling thread only after locking the semaphore
*	or if the call is interrupted by a signal
* @param sem
*
* @return 
*/
s32int sem_wait(sem_t *sem)
{
	return(sw_sem_wait((sw_sem_t*)sem));
}

/**
* @brief unlocks the semaphore referenced by sem 
*	by performing a unlock operation on that semaphore
*
* @param sem
*
* @return 
*/
s32int sem_post(sem_t *sem)
{
	return(sw_sem_post((sw_sem_t*)sem));
}

/**
* @brief Destroys the semaphore indicated by sem 
*
* @param sem
*
* @return 
*/
s32int sem_destroy(sem_t *sem)
{
	return(sw_sem_destroy((sw_sem_t*)sem));
}

/**
* @brief This function is called to get the semaphore value 
*
* @param sem
* @param value
*
* @return 
*/
s32int sem_getvalue(sem_t *sem, s32int *value)
{
	return(sw_sem_getvalue((sw_sem_t*)sem,value));
}

/**
* @brief suspends action for a specified interval of time 
*
* @param seconds
*
* @return 
*/
void sleep(u32 seconds)
{
	sw_sleep(seconds);
}

/**
* @brief suspends action for micro second intervals 
*
* @param seconds
*
* @return 
*/
s32int usleep(u32 seconds)
{
#ifndef TIMER_NOT_DEFINED

#ifdef CONFIG_KSPACE_PROTECTION
	return(__sw_usleep(seconds));
#else
	return(sw_usleep(seconds));
#endif

#endif
}
EXPORT_SYMBOL(usleep);
/**
* @brief
* truncate - function cause the regular file named by path 
*    		 to be truncated to a size of precisely length bytes
*
* @param tr_path - truncates the file whose name is specified in filename
* @param tr_lth -  The desired truncate size of the file in bytes
*
* @return - If the file contents is successfully truncated, a zero value is 
*			returned.On failure, non-Zero is returned
*/
int truncate(const char* tr_path, long tr_lth)
{
#ifdef CONFIG_FILESYSTEM_SUPPORT
    int tr_v;
    tr_v=file_truncate(tr_path,(int)tr_lth);
    if(tr_v==-1) {
        sw_printf("error in truncate\n");
        return -1;
    }
    sw_printf("truncate function is working successfully\n");
    return 0;
#else
	return -1;
#endif
}

/**
 * @brief 
 * rename - Changes the name of the file or directory specified by oldname to 
 *			newname
 *
 * @param old_name -string containing the name of an existing file to be renamed
 * @param new_name -string containing the new name for the file
 *
 * @return - If the file is successfully renamed, a zero value is returned.
 *	On failure, a nonzero value is returned
 */
int rename(const char *old_name,const char *new_name)
{
#ifdef CONFIG_FILESYSTEM_SUPPORT
    int re_ret;
    re_ret=file_rename(old_name,new_name);
    if(re_ret==-1)
    {
		sw_printf("error in rename\n");
        return -1;
    }
	sw_printf("rename function is working successfully\n");
    return 0;
#else
	return -1;
#endif
}
