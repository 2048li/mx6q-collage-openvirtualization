/*
 * openVirtualization: 
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
 * Header for Non secure Kernel Loader information
 */

#ifndef NSKINFO_H
#define NSKINFO_H

#include <sw_types.h>

/* Boot information on non secure kernel */
struct nsk_boot_info {
	u32 nskbi_srcaddr; 
	u32 nskbi_loadaddr;
	u32 nskbi_size;
	int nskbi_initrd_flag;  /* Setting this flag loads initrd */
	u32 nskbi_initrd_sa;
	u32 nskbi_initrd_la;
	u32 nskbi_initrd_size;
};

/**
* @brief 
*
* @param ni
*
* @return 
*/
extern int nsk_boot(struct nsk_boot_info *ni);

/**
* @brief 
*
* @param ni
*
* @return 
*/
int nsk_load(struct nsk_boot_info *ni);

#endif
