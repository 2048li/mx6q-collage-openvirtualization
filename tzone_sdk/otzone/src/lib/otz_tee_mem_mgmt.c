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
 * memory management implementation
 *
 */

#include <sw_buddy.h>
#include <sw_types.h>
#include <sw_debug.h>
#include <libc_sos.h>
#include <sw_mem_functions.h>
#include <otz_tee_mem_mgmt.h>
#include <otz_tee_common.h>
#include <sw_types.h>

/*static void *Data;

TEE_Result TEE_CheckMemoryAccessRights(	u32 accessFlags, void* buffer, size_t size) {
    return TEE_SUCCESS;
}

void TEE_SetInstanceData( void* instanceData ) {
    Data = instanceData;
}

void* TEE_GetInstanceData( void ) {

	return Data;
}
*/
/**
* @brief allocates space for an object 
* whose size in bytes is specified in the parameter size
*
* @param size
* @param hint
*
* @return 
*/
void* TEE_Malloc( size_t size, u32 hint ) {
	void* buffer;
	if((buffer = malloc(size))==NULL) {
		sw_printf("Memory Allocation Failed\n");
		return NULL;
	}
		
	if(hint == 0x0) {
		sw_memset(buffer, 0, size);
	}
	/*else {
	  hint in the range [0x00000001, 0xFFFFFFFF] can be used in future versions.
	}*/
	return buffer;
}

/**
 * @brief changes the size of the memory object pointed to by buffer 
 * 		to the size specified by newSize
 *
 * @param buffer
 * @param newSize
 *
 * @return 
 */
void* TEE_Realloc( void* buffer, u32 newSize ) {
    return(realloc(buffer,newSize));
}

/**
 * @brief causes the space pointed to by buffer to be deallocated
 * 		that is, made available for further allocation
 *
 * @param buffer
 */
void TEE_Free( void *buffer ) {
    sw_free(buffer);
}

/**
 * @brief copies count bytes from the object pointed to by src 
 *		into the object pointed to by dest
 *
 * @param dest
 * @param src
 * @param count
 */
void  TEE_MemMove( void* dest, void* src, u32 count ) {
    sw_memmove(dest, src, count);
}

/**
 * @brief compares the first length bytes of buffer1 to the
 *		first length bytes of buffer2
 *
 * @param src
 * @param dest
 * @param length
 *
 * @return 
 */
int TEE_MemCompare( void* src, void* dest, u32 length) {
    return(sw_memcmp(src, dest, length));
}

/**
 * @brief writes the byte x into the first size bytes of the buffer 
 *
 * @param buffer
 * @param x
 * @param size
 */
void  TEE_MemFill( void* buffer, u32 x, u32 size) {
    sw_memset(buffer, x, size);
}

/**
 * @brief copies the first count bytes of src to the
 *		first count bytes of dest
 *
 * @param dest
 * @param src
 * @param count
 */
void  TEE_MemCpy( void* dest, void* src, u32 count ) {
    sw_memcpy(dest, src, count);
}
