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
 * semaphore implementation functions
 *
 */

#include <sw_semaphores.h>
#include <elf_loader.h>
#include <unused.h>
/**
* @brief This function must be called before entering the critical region 
*  		code obtains the requisite lock calling this function 
*
* @param lock
*
* @return 
*/
static __inline void sw_get_spinlock(volatile u32 *lock)
{
    sw_spinlock_s((void*)lock);
    /*OTZ_READ_MEMORY_BARRIER
    __memory_changed();*/
    data_memory_barrier();
}


/**
* @brief This function is called to release a lock already obtained
*
* @param lock
*
* @return 
*/
static __inline void sw_release_spinlock(volatile u32 *lock)
{
    /*__memory_changed();
    OTZ_WRITE_MEMORY_BARRIER*/
    data_sync_barrier();
    sw_spinunlock_s((void*)lock);
}


/**
* @brief Gets a trylock for a crictical region code 
*		If mutex is unlocked, it will lock it and return True
*		Or if it is locked it will immediately return False
* @param lock
*
* @return status of the lock
*/
static __inline int sw_get_trylock(volatile u32 *lock)
{
    u32 lock_state;

    lock_state = sw_trylock_s((void*)lock);
    /*OTZ_READ_MEMORY_BARRIER
    __memory_changed();*/
    data_memory_barrier();
    return(lock_state);
}


/**
* @brief Initializes mutex parameters 
*
* @param mutex
* @param attribute
*
* @return 
*/
s32int sw_mutex_init(sw_mutex_t *mutex, const sw_mutexattr_t *attribute)
{
    if(mutex == NULL) {
        return(OTZ_INVALID);
    }
    mutex->lock_param = OTZ_FREE;
    mutex->init_param = TRUE;
    UNUSED(attribute);
    return(OTZ_OK);
}

/**
* @brief This function is called to destroy the mutex once it is used 
*		If it is under lock it returns an error code
*		init param is reinitialized to zero 
* @param mutex
*
* @return 
*/
s32int sw_mutex_destroy(sw_mutex_t *mutex)
{
    if(mutex == NULL) {
        return(OTZ_INVALID);
    }
    if(mutex->lock_param != OTZ_FREE) {
        return(OTZ_BUSY);
    }
    mutex->init_param = FALSE;
    return(OTZ_OK);
}

/**
* @brief This function is called to lock the specified mutex 
*
* @param mutex
*
* @return 
*/
s32int sw_mutex_lock(sw_mutex_t *mutex)
{
    if((mutex == NULL) || (mutex->init_param == FALSE)) {
        return(OTZ_INVALID);
    }
    sw_get_spinlock((void*)&(mutex->lock_param));
    return(OTZ_OK);
}

EXPORT_SYMBOL(sw_mutex_lock);
/**
* @brief This function is called to unlock a mutex 
*
* @param mutex
*
* @return 
*/
s32int sw_mutex_unlock(sw_mutex_t *mutex)
{
    if((mutex == NULL) || (mutex->init_param == FALSE)) {
        return(OTZ_INVALID);
    }
    sw_release_spinlock((void*)&(mutex->lock_param));
    return(OTZ_OK);
}
EXPORT_SYMBOL(sw_mutex_unlock);
/**
* @brief Same as sw_mutex_lock but it does not block the calling function
*		It returns an error code if the mutex is already locked
* @param mutex
*
* @return 
*/
s32int sw_mutex_trylock(sw_mutex_t *mutex)
{
    if((mutex == NULL) || (mutex->init_param == FALSE)) {
        return(OTZ_INVALID);
    }
    if(sw_get_trylock((void*)&(mutex->lock_param)) != OTZ_FREE) {
        return(OTZ_BUSY);
    }
    return(OTZ_OK);
}
EXPORT_SYMBOL(sw_mutex_trylock);
/**
* @brief Semaphore initialization
*	Following successful call, other semaphore operations 
*	can be performed until semaphore is destroyed
* @param sem
* @param shared
* @param value
*
* @return 
*/
s32int sw_sem_init(sw_sem_t *sem, s32int shared, u32 value)
{
    if(sem == NULL) {
        return(OTZ_INVALID);
    }
    sem->init_param = TRUE;
    sem->counter_param = value;
    UNUSED(shared);
    return(OTZ_OK);
}

/**
* @brief Locks the semaphore referenced by sem 
*	It will return to the calling thread only after locking the semaphore
*	or if the call is interrupted by a signal
*	
* @param sem
*
* @return 
*/
s32int sw_sem_wait(sw_sem_t *sem)
{
    if((sem == NULL) || (sem->init_param == FALSE)) {
        return(OTZ_INVALID);
    }
    sw_sem_wait_s((s32int*)&(sem->counter_param));
    /*OTZ_READ_MEMORY_BARRIER
    __memory_changed();*/
    data_memory_barrier();
    return(OTZ_OK);
}

/**
* @brief unlocks the semaphore referenced by sem 
*	by performing a unlock operation on that semaphore
*
* @param sem
*
* @return 
*/
s32int sw_sem_post(sw_sem_t *sem)
{
    if((sem == NULL) || (sem->init_param == FALSE)) {
        return(OTZ_INVALID);
    }
    /*__memory_changed();
    OTZ_WRITE_MEMORY_BARRIER */
    data_sync_barrier();
    sw_sem_post_s((s32int*)&(sem->counter_param));
    return(OTZ_OK);
}

/**
* @brief Destroys the semaphore indicated by sem 
*
* @param sem
*
* @return 
*/
s32int sw_sem_destroy(sw_sem_t *sem)
{
    if(sem == NULL) {
        return(OTZ_INVALID);
    }
    if((int)sem->counter_param < 0) {
        return(OTZ_BUSY);
    }
    sem->counter_param = 0;
    sem->init_param = FALSE;
    return(OTZ_OK);
}

/**
* @brief This function is called to get the semaphore value 
*
* @param sem
* @param value
*
* @return 
*/
s32int sw_sem_getvalue(sw_sem_t *sem, s32int *value)
{
    if((sem == NULL) || (sem->init_param == NULL) || (value == NULL)) {
        return(OTZ_INVALID);
    }
    *value = sem->counter_param;
    return(OTZ_OK);
}
