#ifndef __FAT32_H_
#define __FAT32_H_

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
 * Header file for fat32 filesystem
 */
#include<sw_types.h>

#define END_OF_CLUSTER	         0x0FFFFFFF
#define END_OF_ROOT_CLUSTER      0X0FFFFFF8
#define MAX_DIR_ENTRY_CNT	 16
#define FILE_SYS_TYPE_OFF    	 82
#define BYTES_PER_SEC_OFF        11
#define SEC_PER_CLUS_OFF         13
#define RES_SEC_CNT_OFF          14
#define FAT_CNT_OFF              16
#define TOT_SEC_CNT_OFF          32
#define SEC_PER_FAT              36
#define ROOT_DIR_STRT_CLUS_OFF   44
#define FS_INFOSECTOR_OFF        48
#define BACKUP_BOOT_SEC_OFF      50
#define NXT_FREE_CLUS_OFF        492
#define FILE_SYS_TYPE_LENGTH     8 
#define SHRT_FILE_NAME_LEN       11
#define STRT_CLUS_LOW_OFF        26
#define STRT_CLUS_HIGH_OFF       20
#define FILE_SIZE_OFF	         28
#define ATTR_OFF                 11
#define FILE_STAT_LEN            21
#define CHECK_SUM_OFF	         13
#define FILE_NAME_SHRT_LEN       8
#define FILE_NAME_EXTN_LEN       3
#define LONG_FILE_NAME_LEN       255
#define LOW_CLUSWORD_MASK        0x0000FFFF
#define HIGH_CLUSWORD_MASK       0xFFFF0000
#define LONG_FNAME_MASK		 0x0F
#define LAST_ORD_FIELD_SEQ	 0x40
#define LFN_END_MARK		 0xFFFF
#define LFN_TERM_MARK		 0x0000
#define LFN_FIRST_OFF		 0x01
#define LFN_SIXTH_OFF		 0x0E
#define LFN_TWELVETH_OFF	 0x1C
#define LFN_FIRST_SET_CNT	 5
#define LFN_SEC_SET_CNT		 6
#define LFN_THIRD_SET_CNT        2        
#define LFN_FIRST_SET_LEN	 10
#define LFN_SEC_SET_LEN          12
#define LFN_THIRD_SET_LEN        4
#define LFN_EMPTY_LEN	         2 
#define LFN_LEN_PER_ENTRY        13
#define FNAME_EXTN_SEP_OFF       6
#define FNAME_SEQ_NUM_OFF        7            
#define BYTES_PER_CLUSTER_ENTRY  4
#define DIR_ENTRY_LEN            32
#define VOL_ID_LEN               4
#define VOL_LABEL_LEN            11
#define RESERV_LEN               12
#define FS_VER_LEN               2 
#define OEM_NAME_LEN             8
#define JUMP_INS_LEN             3
#define MAX_FAT_CNT              2 
#define SPACE_VAL                32 
#define FILE_READ                0x01
#define FILE_WRITE               0X02
#define FILE_CREATE_NEW          0x04
#define FILE_CREATE_ALWAYS       0x08
#define FILE_APPEND              0x10              
#define ATTR_READ                0x01
#define ATTR_HIDDEN              0x02
#define ATTR_SYSTEM              0x04
#define ATTR_VOL_LABEL           0x08
#define ATTR_DIR                 0x10
#define ATTR_ARCHIVE             0x20
#define ATTR_LONG_FNAME		 0x0F
#define FREE_DIR_ENTRY           0x00
#define DEL_DIR_ENTRY            0xE5
#define DOT_DIR_ENTRY		 0x2E
#define ASCII_DIFF               32
#define FILE_SEEK_SET            0
#define FILE_SEEK_CUR            1
#define FILE_SEEK_END            2
#define DELIMITER	         '/'
#define EXTN_DELIMITER           '.'
#define TILDE	                 '~'
#define FULL_SHRT_NAME_LEN       13
 
/**
* @brief 	
*	Structure of the boot sector
*/
typedef struct __attribute__((__packed__))
{
	u8   jump[JUMP_INS_LEN];
	u8   OEM_name[OEM_NAME_LEN];
	u16  bytes_per_sec;
	u8   sec_per_clus;
	u16  reserved_sec_cnt;
	u8   fat_cnt;
	u16  root_dir_max_cnt;
	u16  tot_sectors;
	u8   media_desc;
	u16  sec_per_fat_fat16;
	u16  sec_per_track;
	u16  number_of_heads;
	u32  hidden_sec_cnt;  
	u32  tol_sector_cnt;
	u32  sectors_per_fat;
	u16  ext_flags;
	u8   fs_version[FS_VER_LEN];   
	u32  root_dir_strt_cluster;
	u16  fs_info_sector;
	u16  backup_boot_sector;
	u8   reserved[RESERV_LEN];
	u8   drive_number;
	u8   reserved1;
	u8   boot_sig;
	u8   volume_id[VOL_ID_LEN];
	u8   volume_label[VOL_LABEL_LEN];
	u8   file_system_type[FILE_SYS_TYPE_LENGTH];  
} boot_sector;


/**
* @brief 
*	Structure of the 32 byte directory entry
*/
typedef struct __attribute__((__packed__))
{
	u8  name[FILE_NAME_SHRT_LEN];
	u8  extn[FILE_NAME_EXTN_LEN];
	u8  attr;
	u8  reserved;
	u8  crt_time_tenth;
	u16 crt_time;
	u16 crt_date;
	u16 lst_access_date;
	u16 strt_clus_hword;
	u16 lst_mod_time;
	u16 lst_mod_date;
	u16 strt_clus_lword;
	u32 size;
} dir_entry;

/**
* @brief 
*	Structure of the long file name slot
*/
typedef struct
{
	u8 ord_field;
	u8 fname0_4[LFN_FIRST_SET_LEN];
	u8 flag;
	u8 reserved;
	u8 chksum;
	u8 fname6_11[LFN_SEC_SET_LEN];
	u8 empty[LFN_EMPTY_LEN];
	u8 fname12_13[LFN_THIRD_SET_LEN];	
} lfn_entry;

struct f_info;
typedef struct f_info file_info;
/**
* @brief 
*	Structure of a file containing its basic information
*	required for many operations
*/
struct f_info
{
	int fd;
	dir_entry *entry;
	const char *file_path;
	u32 strt_cluster;
	u32 cur_cluster;
	u16 cur_offset;
	u32 file_size;
	u8  mode;
	u32 bytes_traversed;
	u8 *dir_entry_offset;
	file_info *next;
};


/**
* @brief 
*	Structure containing the basic details of a file/directory
*/
typedef struct 
{
	char name[FULL_SHRT_NAME_LEN];
	const char *lname;
	u32 size;
	u16 time;
	u16 date;
	u8 attr;	
} file_stat;

/**
* @brief 
*	Mount the FAT file system and retrieve its basic information
* @param dev
*	Pointer to the start of the file system image
* @return 0:File system is FAT32 and the mount process is successful
*         -1:Mount is unsuccessful 
*/
int mount_file_system(u8 *dev);

/**
* @brief 
*	 Verify the type of FAT file system 
* @param file_sys_type
*	 Pointer to the location where the type of file system is stored
* @return true:indicates FAT32 file system
*	  false:indicates not FAT32 file system
*/
bool chk_fat32_file_system(u8 *file_sys_type);

/**
* @brief 
*	Function to calculate all the region offsets
* @param bs
*	Pointer to the boot sector structure which holds the information
*	of the boot sector of the file system 
* @param dev
*	Pointer to the start of the file system
*/
void cal_region_offsets(boot_sector *bs, u8 *dev);

/**
* @brief 
*	Function to open a specific file mentioned in the path 
*	based on the incoming modes
* @param file_path
*	Pointer to the location of the file path string
* @param flags
*	Flags indicating the modes in which the file is to be opened
*
* @return Non-zero:File descriptor of the opened file
	  Zero    :File open is unsuccessful
*/
int file_open(const char *file_path,int flags);


/**
* @brief 
*	Function to check whether a given file name exists in the file system
*	and if so retrieve the corresponding short file name directory entry
* @param fname
*	Pointer to the start of the file name which needs to be searched
* @param entry
*	Pointer to a pointer of the directory entry structure in which the 
*	short file name directory entry content is to be stored
* @param strt
*	Pointer to the start of the region where the file/directory needs 
*	to be searched
* @param strt_clus
*	Starting cluster of the directory in which the file is assumed 
*	to be present 
* @param is_lfn
*	Boolean value which indicates whether the file is short or long file
* @return 
*  	true: if the file/dir is present	
*	false:if it is not present in the respected path
*/
bool get_dir_entry(char *fname,dir_entry **entry,u8 *strt,
				   u32 strt_clus,bool is_lfn);


/**
* @brief 
*	Function to get the long file name correspoding to a directory slot
* @param entry
*	Pointer to the slot where a long file component resides
* @param lfname
*	Pointer to the start of the long file name
* @param index
*	Pointer to the offset of the long file name where the retrieved name 
*	from the given slot is going to be stored
* @return 
*	0:if the file name doesnt end in the given slot
*	1:if the file name ends in the given slot
*/
int lfn_entry_to_name(lfn_entry *entry,char *lfname,int *index);

/**
* @brief 
*	Function to get the corresponding physical address from the given cluster
* @param cluster
*	Cluster value for which the address is to be obtained
* @return 
*	Memory address of the incoming cluster
*/
u8 * cluster_to_memory_addr(u32 cluster);

/**
* @brief 
*	Function to assign a file descriptor and necessary information to a file
*	structure inorder to pursue subsequent operations
*
* @param finfo : Pointer to a file structure where the file information is stored
* @param entry : Pointer to the directory entry of the file/directory 
* @param mode : Mode in which the file is requested to be opened
* @param dir_offset : directory offset 
* @param path : path
*
* @return returns : File descriptor of the file
*/
int retrieve_file_info(file_info *finfo,dir_entry *entry,u8 mode,
					   u8 *dir_offset,const char *path);
/**
* @brief 
*	Function to retrieve the next cluster value in a cluster chain from 
*	the FAT table
* @param cluster
*	Cluster whose next cluster value is to be obtained 
* @return 
*	Corresponding FAT table entry for the given cluster
*/
u32 get_fat_table_entry(u32 cluster);

/**
* @brief 
*	Function to convert the incoming short file name to its corresponding upper 
*	case letters 
* @param file_name
*	File name whose values are to be converted	
*/
void convert_to_uppercase(char * file_name);

/**
* @brief 
*	Function to calculate the number of iterations needed to reach the file from
* 	the given starting path
* @param file_path
*	Pointer to the starting of the file path
* @return 
*	Count of directories which resides before the file/directory
*	from the given path
*/
int find_depth(const char *file_path);

/**
* @brief 
*	Function to create a file if the file doesnt exists and the create 
*	flag is set
* @param lfname
*	Pointer to the start of the long file. NULL in case of short file
*	name creation 
* @param shrt_name
*	Pointer to the start of the short file name. In case of long file names
*	the corresponding short file name is found out.
* @param clus
*	Starting cluster of the directory in which the file is going to be
*	created
* @param entry
*	Pointer to a pointer of the directory entry structure in which the 
*       short file name directory entry content is to be stored
* @return 
*	On successful creation returns zero
*/
int create_file(char *lfname,char *shrt_name,u32 clus,dir_entry **entry);


/**
* @brief 
*	Function to read the contents of a file
* @param fd
*	File descriptor of the file in which read operation needs to be performed
* @param buffer
*	Pointer to the start of the buffer where the contents read are stored
* @param bytes_to_read
*	Number of bytes to be read from the file
* @return 
*	Number of bytes successfully read
*/
int file_read(int fd,char * buffer,int bytes_to_read);

/**
* @brief 
*	Function to write into the given file with the contents given
* @param fd
*	File descriptor of the file 
* @param buffer
*	Pointer to the start of the contents which needs to be written into
*	the file
* @param bytes_to_write
*	Indicates the number of bytes which needs to be written into the file
* @return 
*	Integer representing number of bytes successfully written into the file
*/
int file_write(int fd, const char *buffer,int bytes_to_write);

/**
* @brief 
*	Function to find an unused cluster from the FAT table
* @return
*	Newly allocated cluster for the file
*/
u32 allocate_cluster();

/**
* @brief 
*	Function to write into the FAT table after assigning an empty cluster 
*	to a file
* @param cluster
*	Cluster number for which the entry has to be made
* @param value
*	Next cluster value of the cluster number provided
*/
void write_fat_table(u32 cluster,u32 value);

/**
* @brief Function to extract the short file name given its long file name
*
* @param lfname : Pointer to the start of the long file name string
* @param sfname : Pointer to the start of the short file name in which this function 
*	extracts and stored the short file name value
* @param seq_num : sequence number
*/
void get_short_file_name(char * lfname,char *sfname,char seq_num);

/**
* @brief 
*	Function to put the long file name entries into the respective allocated
*	region
* @param fname
*	Pointer to the start of the long file name 
* @param entry
*	Starting memory location to the short file name directory entry
* @param chk_sum
*	Calculated check sum of the short file name corresponding to the
*	long file name
* @param count
*	Counter indicating the number of 32 byte entries needed for the whole 
*	file name to get accommodated
*/
void fill_lng_fname_entries(char *fname,u8 *entry,u8 chk_sum,int count);
/**
* @brief 
*	Function to calculate the checksum for the long file name 
*	based on its short name entry
* @param dir_entry
*	Pointer to the start of the short file name corresponding
*	to a long file name
* @return 
*	checksum value of the short file name
*/
u8 calc_check_sum(u8 * dir_entry);


/**
* @brief 
*	Close the opened file based on the file descriptor and 
*	free the file structure pointer
* @param fd
*	File descriptor of the file which needs to be closed
* @return 
*	Zero on successful closing of the file
*/
int file_close(int fd);

/**
* @brief 
*	Function to seek the opened file pointer to the desired location
*	so that subsequent read and write operations are performed from 
*	the new value
* @param fd
*	File descriptor of the file in which the seek operation needs to be 
*	performed
* @param offset
*	Offset bytes indicating the number of bytes which needs to be moved
* @param whence
*	Flag denoting the position from which the seek operation needs to be
*	performed
* @return 
*	Number of bytes traversed from the beginning of the file to the current 
*	position
*/
int file_seek(int fd,int offset,int whence);

/**
* @brief 
*	Function to move the current cluster and its offset value to 
*	the value specified in the offset from the starting cluster
	provided
* @param file_ptr
*	File structure pointer pointing to the opened file
* @param strt_cluster
*	Starting cluster value from which the seek operation has to 
*	be performed
* @param offset
*	Offset bytes for the seek operation
* @return 
*	Number of bytes traversed from the beginning of the file
*/
int get_seek_off(file_info * file_ptr,u32 strt_cluster,int offset);

/**
* @brief 
*	Function to retrieve the long file name given its
*	end of the long file name entry
* @param fname
*	Pointer to a location where the retrieved long file name starts
* @param dir_strt
*	Pointer to the memory location where the last component of the long 
*	file name 
*	resides
* @param count
*	Count representing the number of directory entries needed for the 
*	long file name
* @return 
* 	Length of the long file name
*/
int get_long_file_name(char *fname,u8 **dir_strt,int *count);

/**
* @brief 
*	Function to rename a file/directory
* @param old
*	Constant pointer to the start of the path where the file which 
*	needs to be renamed is located
* @param new
*	Constant pointer to the start of the path where the new name of the 
*	file/directory is situated
* @return 
*	zero on successful renaming
*	-1 if the new name already exists 
*/
int file_rename(const char *old,const char *new);

/**
* @brief 
*	Function to create a directory
* @param path
*	Pointer to the start of the path which represents the directory hierarchy
* @return 
*	On successful creation:File descriptor of the directory 
*	-1:If the creation failed or if it already exists
*/
int create_dir(const char *path);
/**
 * @brief 
 *
 * @param path
 *
 * @return 
 */
int chk_file_lock(const char *path);
/**
* @brief 
*
* @param fd
*
* @return 
*/
file_stat * read_dir(int fd);

/**
* @brief 
*	Function to open a directory 
* @param path
*	Pointer to the start of the path representing the directory hierarchy
* @return 
*	file descriptor assigned to the directory
*/
int open_dir(const char *path);

/**
* @brief 
*
* @param file_name_len
*
* @return 
*/
int cal_req_dir_cnt(int file_name_len);

/**
* @brief
* file_truncate - function cause the regular file named by path 
*                 to be truncated to a size of precisely length bytes
*
* @param tr_pf -  truncates the file whose name is specified in filename
* @param tr_lgh - The desired truncate size of the file in bytes
*
* @return - If the file contents is successfully truncated, a zero value is 
*           returned.On failure, non-Zero is returned
*/
int file_truncate(const char* tr_pf,int tr_lgh);

/**
 * @brief 
 * file_remove - Deletes the file whose name is specified in filename
 *
 * @param fl_name - string containing the name of the file to be deleted
 *
 * @return - if the file is successfully deleted, a zero value is returned.
 *           On failure, a non-zero value is returned
 */
int file_remove(const char *fl_name);

/**
* @brief  file_remove - Deletes the file whose name is specified in filename
*
* @param ul_name - string containing the name of the file to be deleted
*
* @return if the file is successfully deleted, a zero value is returned.
*           On failure, a non-zero value is returned
*/
int file_unlink(const char *ul_name);

/**
* @brief 
*	Function to list the directory contents 
* @param path
*	Pointer to the start of the path representing the directory hierarchy
* @return 
*	Number of directories/files present in the given directory
*/
int list_dir(const char *path);

#endif
