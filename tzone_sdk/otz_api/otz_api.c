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
 */

/* 
 * Trustzone client API implementation.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <otz_api.h>
#include <otz_client.h>
#include <otz_id.h>


/**
 * @brief 
 * This function opens a connection with the device in the underlying operating
 * environment that represents the secure environment. When the client no longer
 * requires the device it must call otz_device_close to close the connection and
 * free any associated resources. This function accepts a pointer to a
 * otz_device_t structure assumed to be in the OTZ_STATE_UNDEFINED state. On
 * success this function must set the device structure *ps_device to the state
 * OTZ_STATE_OPEN with a session count of zero. On failure, the device is set to
 * the state OTZ_STATE_INVALID. It is possible to create multiple concurrent
 * device connections from a single client. The number of devices that can be
 * supported globally within the entire system, or locally within a single
 * client, is implementation-defined.
 *
 * @param pk_device_name
 * @param pk_init
 * @param ps_device
 *
 * @return 
 */
otz_return_t otz_device_open(void const* pk_device_name, void const* pk_init,
                            otz_device_t* ps_device) 
{
    int ret=0;
    if(ps_device == NULL)
    {
        printf("tz_device_open : Device is null\n");
        return OTZ_ERROR_ILLEGAL_ARGUMENT;
    }

    if(pk_device_name == NULL)
    {
#ifdef OTZ_DEBUG
        printf("/dev/otz_client is assigned as default device\n");
#endif
        pk_device_name = "/dev/otz_client";
    }

    if(pk_init == NULL)
    {
#ifdef OTZ_DEBUG
        printf("Device is opened as Read-only by default\n");
#endif
        pk_init = O_RDONLY;
    }

    ret = open(pk_device_name, (uint32_t)pk_init); 
    if( ret == -1){
        ps_device->ui_state = OTZ_STATE_INVALID;
        ps_device->s_errno = errno;
        return OTZ_ERROR_GENERIC;
    }
    else{
        ps_device->fd = ret ;
        ps_device->ui_state = OTZ_STATE_OPEN;
        ps_device->session_count = 0;
    }

    return OTZ_SUCCESS;
}

/**
 * @brief 
 * Calling this function generates a device-local absolute time limit in the
 * structure pointed to by ps_time_limit  from a timeout value ui_timeout. 
 * The absolute time limit is equal to the  current time plus 
 * the specified timeout.
 *
 * @param ps_device
 * @param ui_timeout
 * @param ps_timelimit
 *
 * @return 
 */
otz_return_t otz_device_get_timelimit(otz_device_t* ps_device,
                                    uint32_t ui_timeout,
                                    otz_timelimit_t* ps_timelimit)
{
    return OTZ_ERROR_NOT_IMPLEMENTED;
}

/**
 * @brief 
 *
 * This function is responsible for locally preparing an operation that can be
 * used to connect with the service  defined by the UUID 
 * pointed to by pks_service, using the timeout pointed to by
 * pks_time_limit and the login credentials specified in pks_login.
 *
 * @param ps_device
 * @param pks_service
 * @param pks_login
 * @param pks_timelimit
 * @param ps_session
 * @param ps_operation
 *
 * @return 
 */
otz_return_t otz_operation_prepare_open(otz_device_t* ps_device,
/*                                      tz_uuid_t const* pks_service, */
                                      int pks_service,
                                      otz_login_t const* pks_login,
                                      otz_timelimit_t const* pks_timelimit,
                                      otz_session_t* ps_session,
                                      otz_operation_t* ps_operation )
{

    if (ps_device == NULL || pks_service == OTZ_SVC_INVALID || 
        ps_session == NULL || ps_operation == NULL) {
    
        printf("operation prepare open : Illegal argument\n");
        return OTZ_ERROR_ILLEGAL_ARGUMENT;
    }

    ps_session->ui_state = OTZ_STATE_INVALID;
    ps_session->device = ps_device;
    ps_session->operation_count = 0;
    ps_session->operation_count++;
    ps_session->service_id = pks_service;
    ps_session->shared_mem_cnt = 0;
    ps_operation->temp_mem_ref_count = 0;

    INIT_LIST_HEAD(&ps_session->shared_mem_list);

    ps_operation->type = OTZ_OPERATION_OPEN;
    ps_operation->ui_state = OTZ_STATE_ENCODE;
    ps_operation->session = ps_session;
    return OTZ_SUCCESS;
}

/**
 * @brief 
 *
 * This function is responsible for locally preparing an operation that can be
 * used to issue a command to a service  with which the client has 
 * already created a session.
 *
 * @param ps_session
 * @param ui_command
 * @param pks_timelimit
 * @param ps_operation
 *
 * @return 
 */
otz_return_t otz_operation_prepare_invoke(otz_session_t* ps_session,
                                        uint32_t ui_command,
                                        otz_timelimit_t const* pks_timelimit,
                                        otz_operation_t* ps_operation)
{

    if(ps_session == NULL || ps_operation == NULL) {
        printf("prepare invoke : Illegal argument\n");
        return OTZ_ERROR_ILLEGAL_ARGUMENT;
    }

    if(ps_session->ui_state != OTZ_STATE_OPEN) {
        printf("prepare invoke : Illegal state\n");
        return OTZ_ERROR_ILLEGAL_STATE;
    }

     ps_operation->session = ps_session;
     ps_session->operation_count++;
     ps_operation->type = OTZ_OPERATION_INVOKE;
     ps_operation->ui_state = OTZ_STATE_ENCODE;
     ps_operation->enc_dec.encode_id = -1;
     ps_operation->enc_dec.cmd_id = ui_command;
     ps_operation->enc_dec.enc_error_state = OTZ_SUCCESS;
     ps_operation->enc_dec.dec_error_state = OTZ_SUCCESS;
     ps_operation->shared_mem_ref_count = 0;
     ps_operation->temp_mem_ref_count = 0;
     return OTZ_SUCCESS;
}

/**
 * @brief Prepare the operation for close session 
 *
 * This function is responsible for locally preparing an operation that can be
 * used to close a session between the client and a service.
 *
 * @param ps_session
 * @param ps_operation
 *
 * @return 
 */
otz_return_t otz_operation_prepare_close(otz_session_t* ps_session,
                                       otz_operation_t* ps_operation)
{
    if(ps_session == NULL || ps_operation == NULL) {
        printf("operation prepare close : Illegal argument\n");
        return OTZ_ERROR_ILLEGAL_ARGUMENT;
    }

    if(ps_session->ui_state != OTZ_STATE_OPEN) {
        printf("operation prepare close : Illegal state\n");
        return OTZ_ERROR_ILLEGAL_STATE;
    }

    if(ps_operation->ui_state != OTZ_STATE_UNDEFINED) {
        printf("operation prepare close : Illegal state\n");
        return OTZ_ERROR_ILLEGAL_STATE;
    }

    ps_session->ui_state = OTZ_STATE_CLOSING;
    ps_session->operation_count++;

    ps_operation->session = ps_session;
    ps_operation->type = OTZ_OPERATION_CLOSE;
    ps_operation->ui_state = OTZ_STATE_PERFORMABLE;
    ps_operation->shared_mem_ref_count = 0;
    ps_operation->temp_mem_ref_count = 0;
    return OTZ_SUCCESS;

}

/**
 * @brief Performs the previously prepared operation
 *
 * This function performs a previously prepared operation – issuing it to the
 * secure environment.
 * There are three kinds of operations that can be issued: opening a client
 * session, invoking a service command,
 * and closing a client session. Each type of operation is prepared with its
 * respective function, which returns the
 * operation structure to be used:
 *    otz_operation_prepare_open prepares an open session operation.
 *    otz_operation_prepare_invoke prepares an invoke service command operation.
 *    otz_operation_prepare_close prepares a close session operation.
 *
 * @param ps_operation
 * @param pui_service_return
 *
 * @return 
 */
otz_return_t otz_operation_perform(otz_operation_t* ps_operation,
                                 otz_return_t* pui_service_return )
{
    int ret = 0;
    struct ser_ses_id ses_close;
    struct ser_ses_id ses_open;
    struct otz_client_encode_cmd enc;

    if(ps_operation == NULL || pui_service_return == NULL) {
        printf("operation_perform : Illegal argument\n");    
        return OTZ_ERROR_ILLEGAL_ARGUMENT;
    }

    if(!(ps_operation->ui_state == OTZ_STATE_ENCODE || 
        ps_operation->ui_state == OTZ_STATE_PERFORMABLE)) {
        printf("operation_perform : Illegal state\n");    
        return OTZ_ERROR_ILLEGAL_STATE;
    }

#ifdef OTZ_DEBUG
    printf("perform \n");
#endif
    ps_operation->ui_state = OTZ_STATE_RUNNING;

     /* For close operation the service cannot return a message*/
     /* The client cannot cancel or time-out the operation */
     /* When this is complete irrespective of success or failure
      * the session is considered close */  
    if(ps_operation->type == OTZ_OPERATION_CLOSE)
    {
        if(ps_operation->session->operation_count == 1 && 
                        ps_operation->session->shared_mem_cnt == 0){
                /* session can be closed */
    
            ses_close.service_id = ps_operation->session->service_id ;
            ses_close.session_id = ps_operation->session->session_id ;
    
            ret = ioctl(ps_operation->session->device->fd,
                        OTZ_CLIENT_IOCTL_SES_CLOSE_REQ, &ses_close);

            if(ret < 0){
     
                /* Error is detected before reaching the service */
                *pui_service_return = OTZ_ERROR_GENERIC;
                ps_operation->ui_state = OTZ_STATE_INVALID;
                ps_operation->s_errno = errno;
                /*return  Error actually occurred */
                /*Decoder functions cannot be used on the operation */
                 if(ret == -EFAULT)
                        return OTZ_ERROR_ACCESS_DENIED;

                return OTZ_ERROR_UNDEFINED;
            }
            else if(ret > 0){
                /* Operation reaches the service but it returns error */
                /* Error code from Service assigned */
                *pui_service_return = ret;

                ps_operation->ui_state = OTZ_STATE_INVALID;
                return OTZ_ERROR_SERVICE; 
            }

            *pui_service_return = OTZ_SUCCESS;

            ps_operation->session->device->session_count--;  
            ps_operation->session->device = NULL;
            ps_operation->session->ui_state = OTZ_STATE_UNDEFINED;  
            ps_operation->session->session_id = -1;  
            ps_operation->session->service_id = OTZ_SVC_INVALID;  

            ps_operation->ui_state = OTZ_STATE_INVALID;
            return OTZ_SUCCESS;
        }
        else{
            /* Undefined Behaviour */
            printf("Operation_cnt or shared_mem_cnt != 0 undef behaviour\n");
            printf("Operation_cnt = %d   shared_mem_cnt = %d\n",
                        ps_operation->session->operation_count,
                        ps_operation->session->shared_mem_cnt);
            return OTZ_ERROR_GENERIC;
        }

    }
    
    if(ps_operation->type == OTZ_OPERATION_OPEN)
    {
        ses_open.service_id = ps_operation->session->service_id;

        ret = ioctl(ps_operation->session->device->fd,
                    OTZ_CLIENT_IOCTL_SES_OPEN_REQ, &ses_open);
            if(ret < 0){
                 /* Error is detected before reaching the service */
                *pui_service_return = OTZ_ERROR_GENERIC;
                ps_operation->ui_state = OTZ_STATE_INVALID;
                ps_operation->s_errno = errno;
                /*return  Error actually occurred */
                /*Decoder functions cannot be used on the operation */

            /*  The encoder ran out of space */
                 if(ret == -ENOMEM)
                        return OTZ_ERROR_MEMORY;
                 if(ret == -EFAULT)
                     return  OTZ_ERROR_ACCESS_DENIED;
                 if(ret == -EINVAL)
                     return  OTZ_ERROR_ILLEGAL_ARGUMENT;
                return OTZ_ERROR_UNDEFINED;
            }
            else if(ret > 0){
                /* Operation reaches the service but it returns error */
                /* Error code from Service assigned */
                *pui_service_return = ret;

                /* The service may have a message for the client
                 * which can be decoded if needed */
                ps_operation->ui_state = OTZ_STATE_DECODE;
                return OTZ_ERROR_SERVICE; 
            }

#ifdef OTZ_DEBUG
        printf("ses open success\n");
#endif

        ps_operation->session->device->session_count++ ;
        ps_operation->session->session_id = ses_open.session_id;

        *pui_service_return = OTZ_SUCCESS;
        ps_operation->ui_state = OTZ_STATE_DECODE;
        ps_operation->session->ui_state = OTZ_STATE_OPEN; 
        return OTZ_SUCCESS;
    }


    if(ps_operation->type == OTZ_OPERATION_INVOKE) {    
        enc.encode_id = ps_operation->enc_dec.encode_id; 
        enc.cmd_id = ps_operation->enc_dec.cmd_id; 
        enc.service_id = ps_operation->session->service_id ;
        enc.session_id = ps_operation->session->session_id ;
    
        ret = ioctl(ps_operation->session->device->fd,
                        OTZ_CLIENT_IOCTL_SEND_CMD_REQ, &enc);

            if(ret < 0){
                 /* Error is detected before reaching the service */
                *pui_service_return = OTZ_ERROR_GENERIC;
                ps_operation->ui_state = OTZ_STATE_INVALID;
                ps_operation->s_errno = errno;
                /*return  Error actually occurred */
                /*Decoder functions cannot be used on the operation */

                 if(ret == -EFAULT)
                     return  OTZ_ERROR_ACCESS_DENIED;
                 if(ret == -EINVAL)
                     return  OTZ_ERROR_ILLEGAL_ARGUMENT;
                return OTZ_ERROR_UNDEFINED;
            }
            else if(ret > 0){
                /* Operation reaches the service but it returns error */
                /* Error code from Service assigned */
                *pui_service_return = ret;

                /* The service may have a message for the client
                 * which can be decoded if needed */
                ps_operation->ui_state = OTZ_STATE_DECODE;
                return OTZ_ERROR_SERVICE; 
            }

        *pui_service_return = OTZ_SUCCESS;
        ps_operation->ui_state = OTZ_STATE_DECODE;
        ps_operation->session->ui_state = OTZ_STATE_OPEN; 
        return OTZ_SUCCESS;
    }
    
    return OTZ_ERROR_UNDEFINED;

}

/**
 * @brief Frees the Temporary shared Memory 
 *
 * @param ps_operation
 */
void otz_free_temp_shared_mem(otz_operation_t* ps_operation)
{
    int ret = 0;
    struct otz_session_shared_mem_info mem_info;
    int i;
    mem_info.service_id = ps_operation->session->service_id;
    mem_info.session_id = ps_operation->session->session_id;

    for(i = 0; i < ps_operation->temp_mem_ref_count; i++) {
        mem_info.user_mem_addr = (uint32_t)ps_operation->temp_mem[i].shared_mem;
        munmap((void*)mem_info.user_mem_addr, ps_operation->temp_mem[i].length);
        ret = ioctl(ps_operation->session->device->fd, 
            OTZ_CLIENT_IOCTL_SHR_MEM_FREE_REQ, &mem_info);
        if(ret ){
            ps_operation->s_errno = ret;
            perror("Warning : Freeing temp shared mem failed \n");
        }

    }
    return;
}


/**
 * @brief Release operation
 *
 * This function releases an operation, freeing any associated resources.
 *
 * @param ps_operation
 */
void otz_operation_release(otz_operation_t* ps_operation)
{
    struct otz_client_encode_cmd enc;
    int ret = 0;

    if (ps_operation == NULL) {
        printf("operation release : Null operation\n"); 
        return;
    }

    if (!(ps_operation->ui_state ==  OTZ_STATE_ENCODE ||
        ps_operation->ui_state == OTZ_STATE_PERFORMABLE ||
        ps_operation->ui_state ==  OTZ_STATE_DECODE ||
        ps_operation->ui_state ==  OTZ_STATE_INVALID)) {

        printf("operation release : Illegal state\n"); 
        printf("Warning : Undefined Behaviour\n"); 
        return;
    }
#ifdef OTZ_DBG
    printf("o_release \n");
#endif
    ps_operation->session->operation_count--;

    otz_free_temp_shared_mem(ps_operation);
    /* Open/Invoke operation has been prepared but not being
     * given to service for Implementation */

    if (ps_operation->ui_state == OTZ_STATE_ENCODE)
    {
        if(ps_operation->type == OTZ_OPERATION_OPEN) {
            ps_operation->session->device->session_count--;
            ps_operation->session->device = NULL;
            ps_operation->session->ui_state = OTZ_STATE_UNDEFINED;
            ps_operation->session->session_id = -1;
            ps_operation->session->service_id = OTZ_SVC_INVALID;
        }
        else if(ps_operation->type == OTZ_OPERATION_INVOKE) {
            /* Perform Necessary state reversal etc.,.*/
        }

    }
    else if (ps_operation->ui_state == OTZ_STATE_PERFORMABLE){
    /* Close operation has been prepared but not being given to
     * service for Implementation */

        if ( ps_operation->type == OTZ_OPERATION_CLOSE) {
            ps_operation->session->ui_state = OTZ_STATE_OPEN;
        }
    }
    else if(ps_operation->ui_state == OTZ_STATE_DECODE)
    {

        enc.encode_id = ps_operation->enc_dec.encode_id; 
        enc.cmd_id = ps_operation->enc_dec.cmd_id; 
        enc.service_id = ps_operation->session->service_id ;
        enc.session_id = ps_operation->session->session_id ;

        ret = ioctl(ps_operation->session->device->fd, 
                        OTZ_CLIENT_IOCTL_OPERATION_RELEASE, &enc);
        if (ret){
            perror("Operation release failed\n");
            ps_operation->s_errno = errno;
        }
    }
    else {
            /* Do Nothing;Just release the operation */
    }

    ps_operation->session = NULL;
    ps_operation->ui_state = OTZ_STATE_UNDEFINED;
    ps_operation->type = OTZ_OPERATION_NONE;
    
    return ; 
}

/**
 * @brief This function Cancels the operation 
 *
 * @param ps_operation
 */
void otz_operation_cancel(otz_operation_t* ps_operation)
{
    return;
}


/**
 * @brief This function allocates a block of memory, defined by the structure
 * pointed to by ps_shared_mem, which is shared 
 * between the client and the service it is connected to.
 *
 * @param ps_session
 * @param ps_shared_mem
 *
 * @return 
 */
otz_return_t otz_shared_memory_allocate(otz_session_t* ps_session ,
                                        otz_shared_mem_t* ps_shared_mem)
{
    int ret = 0, mmap_flags = PROT_READ | PROT_WRITE;
    struct otz_session_shared_mem_info mem_info;

    if(ps_session == NULL || ps_shared_mem == NULL ) {
        printf("shr_mem_allocate : Error Illegal argument\n");
        return OTZ_ERROR_ILLEGAL_ARGUMENT;
    }

    if(ps_session->ui_state != OTZ_STATE_OPEN ||
        ps_shared_mem->ui_length == 0 ||
        (ps_shared_mem->ui_flags != OTZ_MEM_SERVICE_RO && 
         ps_shared_mem->ui_flags != OTZ_MEM_SERVICE_WO && 
         ps_shared_mem->ui_flags != OTZ_MEM_SERVICE_RW)) {
        printf("shr_mem_allocate : Error Illegal state\n");
        return OTZ_ERROR_ILLEGAL_STATE;
    }

    ps_shared_mem->p_block = NULL;

    if(ps_shared_mem->ui_flags ==  OTZ_MEM_SERVICE_RO)
        mmap_flags =  PROT_READ;
    else if(ps_shared_mem->ui_flags ==  OTZ_MEM_SERVICE_WO)
        mmap_flags =  PROT_WRITE;
    else if(ps_shared_mem->ui_flags ==  OTZ_MEM_SERVICE_RW)
        mmap_flags = PROT_READ | PROT_WRITE;

    /* call mem allocation function from the driver */
    /* should allocate eight byte aligned memory */
#ifdef OTZ_DEBUG
    printf("mmap call  \n");
    printf("shared mem len  %d\n",ps_shared_mem->ui_length);
    printf("shared mem fd  %d\n",ps_session->device->fd);
#endif

    ps_shared_mem->p_block = mmap(0, ps_shared_mem->ui_length,
                                      mmap_flags , MAP_SHARED,
                                      ps_session->device->fd, 0);
#ifdef OTZ_DEBUG
    printf("return from mmap\n");
    printf("mmap u_addr  %p\n", (uint32_t*)ps_shared_mem->p_block);
#endif

    if(ps_shared_mem->p_block != MAP_FAILED) {
        mem_info.service_id = ps_session->service_id;
        mem_info.session_id = ps_session->session_id;
        mem_info.user_mem_addr = (uint32_t)ps_shared_mem->p_block;
        ret = ioctl(ps_session->device->fd, 
                OTZ_CLIENT_IOCTL_SHR_MEM_ALLOCATE_REQ, &mem_info);
    }
    else {
        perror("otz_shared_memory_allocate - mmap failed\n"); 
        ps_shared_mem->s_errno = errno;
        ret = -1;
    }

    if(ret != -1){ 
        ps_shared_mem->ui_state = OTZ_STATE_OPEN ; 
        ps_shared_mem->session = ps_session;
        ps_shared_mem->operation_count = 0;

        INIT_LIST_HEAD(&ps_shared_mem->head_ref);
        list_add_tail(&ps_session->shared_mem_list, &ps_shared_mem->head_ref);
        ps_session->shared_mem_cnt++;
        return OTZ_SUCCESS;
    } 
    else {
#ifdef OTZ_DEBUG
        printf("shared_mem_allocation_failed\n");
#endif
        ps_shared_mem->s_errno = errno;
        ps_shared_mem->ui_state = OTZ_STATE_INVALID;
        ps_shared_mem->ui_length = 0 ; 
        ps_shared_mem->ui_flags = OTZ_MEM_SERVICE_UNDEFINED; 
        ps_shared_mem->p_block = NULL ; 

        ps_shared_mem->session = NULL;
        return OTZ_ERROR_MEMORY;
    }

    return OTZ_ERROR_UNDEFINED;
}


/**
 * @brief 
 *
 * @param ps_session
 * @param ps_shared_mem
 *
 * @return 
 */
otz_return_t otz_shared_memory_register(otz_session_t* ps_session ,
                                        otz_shared_mem_t* ps_shared_mem)
{
    return OTZ_ERROR_NOT_IMPLEMENTED;
}

/**
 * @brief This function allocates a block of memory, defined by the structure
 * pointed to by ps_shared_mem, which is shared
 * between the client and the service it is connected to.
 * This function marks a block of shared memory associated with a session as 
 * no longer shared.
 *
 * @param ps_shared_mem
 */
void otz_shared_memory_release(otz_shared_mem_t* ps_shared_mem)
{
    int ret=0;
    struct otz_session_shared_mem_info mem_info;
    struct list *l;
    otz_shared_mem_t* ps_temp;
    int found = 0;

    if(ps_shared_mem == NULL || ps_shared_mem->operation_count != 0) {
#ifdef OTZ_DEBUG
        printf("shared mem release : Illegal arg\n");
        printf("Warning: Undefined Behaviour\n");
#endif
        return;
    }

    if(ps_shared_mem->ui_state == OTZ_STATE_INVALID)
    {
        return;
    }

    mem_info.service_id = ps_shared_mem->session->service_id;
    mem_info.session_id = ps_shared_mem->session->session_id;
    mem_info.user_mem_addr = (uint32_t)ps_shared_mem->p_block;    

    munmap(ps_shared_mem->p_block, ps_shared_mem->ui_length);
    ret = ioctl(ps_shared_mem->session->device->fd,
                   OTZ_CLIENT_IOCTL_SHR_MEM_FREE_REQ, &mem_info);
#ifdef OTZ_DEBUG
    printf("coming out of unmap\n");
#endif
    if(ret)
    {
#ifdef OTZ_DEBUG
        printf("Ioctl for mem free failed\n");
#endif
        printf("Shared memory release failed\n");
        ps_shared_mem->s_errno = errno;
        return; 
    }

    ps_shared_mem->ui_state = OTZ_STATE_UNDEFINED;
    ps_shared_mem->p_block = NULL;
    ps_shared_mem->ui_length = 0;
    ps_shared_mem->ui_flags = OTZ_MEM_SERVICE_UNDEFINED;
    ps_shared_mem->session_id = -1;

    list_for_each(l, &ps_shared_mem->session->shared_mem_list) {
        ps_temp = list_entry(l, otz_shared_mem_t, head_ref);
        if (ps_temp == ps_shared_mem) {
            found = 1;
            break;
        }
    }

    if(found) {
        list_del(&ps_temp->head_ref);
        ps_shared_mem->session->shared_mem_cnt--;
    }
    ps_shared_mem->session = NULL;
    return ;
}

/**
 * @brief Close the device connection
 * 
 * This function closes a connection with a device, freeing any associated
 * resources.
 *
 * @param ps_device
 *
 * @return 
 */
otz_return_t otz_device_close(otz_device_t* ps_device)
{
    if(!ps_device)
    {
        return OTZ_ERROR_ILLEGAL_ARGUMENT;
    }
#ifdef OTZ_DEBUG
    printf("d_close \n");
#endif

    if (ps_device->ui_state == OTZ_STATE_INVALID)
    {
        ps_device->ui_state = OTZ_STATE_UNDEFINED;
        return OTZ_SUCCESS;
    }
    else if (ps_device->ui_state == OTZ_STATE_OPEN)
    {
        if(ps_device->session_count == 0)
        {
            printf("device closed \n");
            close(ps_device->fd);
            ps_device->fd = 0;
            ps_device->ui_state = OTZ_STATE_UNDEFINED;
            return OTZ_SUCCESS;
        }
        else if(ps_device->session_count != 0){
            printf("warning: pending open sessions %d\n", 
                        ps_device->session_count);
            ps_device->s_errno = errno;
            return OTZ_ERROR_ILLEGAL_STATE;
        }
    }

    return OTZ_ERROR_UNDEFINED;
}

/**
 * @brief Encode check 
 *
 * @param ps_operation
 *
 * @return 
 */
static int check_encode(otz_operation_t* ps_operation)
{
    if(!ps_operation) {
        return -1;
    }
    
    if (ps_operation->ui_state != OTZ_STATE_ENCODE) {
        ps_operation->enc_dec.enc_error_state = OTZ_ERROR_ILLEGAL_STATE;
        return -1;
    }

    if(ps_operation->enc_dec.enc_error_state != OTZ_SUCCESS)
        return -1;

    return 0; 
}

/**
 * @brief Encode unsigned 32-bit integer
 *
 * Calling this function appends the value of the passed uint32_t, pk_data, 
 * to the end of the encoded message.
 *
 * @param ps_operation:
 * @param data:
 * @param param_type:
 */
void otz_encode_uint32(otz_operation_t* ps_operation,  void const* data, 
            int param_type)
{
    struct otz_client_encode_cmd enc;
    int ret;

    if(check_encode(ps_operation))
        return;

    enc.encode_id = ps_operation->enc_dec.encode_id; 
    enc.cmd_id = ps_operation->enc_dec.cmd_id; 
    enc.service_id = ps_operation->session->service_id ;
    enc.session_id = ps_operation->session->session_id ;

    enc.data = (void*)data;
    enc.len  = sizeof(uint32_t);
    enc.param_type = param_type;

    ret = ioctl(ps_operation->session->device->fd, 
                    OTZ_CLIENT_IOCTL_ENC_UINT32, &enc);
    if (ret) {
        ps_operation->enc_dec.enc_error_state = OTZ_ERROR_ENCODE_MEMORY;
        ps_operation->s_errno = errno;
    }
    else {
        ps_operation->enc_dec.encode_id = enc.encode_id;
    }
         
    return;
}

/**
 * @brief Encode binary array to the encoded message
 *
 * Calling this function appends a binary array pointed to by array and of
 * length length bytes to the end ofthe encoded message. The implementation must
 * guarantee that when decoding the array in the service the base pointer 
 * is eight byte aligned to enable any basic C data structure to be 
 * exchanged using this method.
 *
 * @param ps_operation
 * @param pk_array
 * @param length
 * @param param_type
 */
void otz_encode_array( otz_operation_t* ps_operation, void const* pk_array, 
                            uint32_t length, int param_type)
{
    struct otz_client_encode_cmd enc;
    int ret;

    if(check_encode(ps_operation))
        return;

    enc.encode_id = ps_operation->enc_dec.encode_id; 
    enc.cmd_id = ps_operation->enc_dec.cmd_id; 
    enc.service_id = ps_operation->session->service_id ;
    enc.session_id = ps_operation->session->session_id ;

    enc.data = (void*)pk_array;
    enc.len  = length;
    enc.param_type = param_type;

    ret = ioctl(ps_operation->session->device->fd, 
                    OTZ_CLIENT_IOCTL_ENC_ARRAY, &enc);
    if (ret) {
        ps_operation->enc_dec.enc_error_state = OTZ_ERROR_ENCODE_MEMORY;
        ps_operation->s_errno = errno;
    }
    else {
        ps_operation->enc_dec.encode_id = enc.encode_id;
    }
     
    return;
}

/**
 * @brief Encode empty binary array to the encoded message
 *
 * Calling this function appends an empty binary array of length "length" bytes 
 * to the end of the encoded message and returns the pointer to this array 
 * to the client. This allows an implementation with fewer copies, as
 * the encoder buffer can be filled directly by the client without needing a 
 * copy from an intermediate buffer into the real encoder buffer.
 *
 * @param ps_operation
 * @param length
 * @param param_type
 *
 * @return 
 */
void* otz_encode_array_space( otz_operation_t* ps_operation,  
                                uint32_t length, int param_type)
{
    struct otz_client_encode_cmd enc;
    struct otz_session_shared_mem_info mem_info;
    
    int ret = 0, mmap_flags;

    enc.data = NULL;

    if(check_encode(ps_operation))
        goto return_func;

    if(ps_operation->temp_mem_ref_count >= MAX_MEMBLOCKS_PER_OPERATION) {
        ps_operation->enc_dec.enc_error_state =  OTZ_ERROR_ENCODE_MEMORY;
        goto return_func;
    }
    
    enc.encode_id = ps_operation->enc_dec.encode_id; 
    enc.cmd_id = ps_operation->enc_dec.cmd_id; 
    enc.service_id = ps_operation->session->service_id ;
    enc.session_id = ps_operation->session->session_id ;

    mmap_flags = PROT_READ | PROT_WRITE;

    enc.data = NULL;
    enc.data = mmap(0, length, mmap_flags , MAP_SHARED,
                    ps_operation->session->device->fd, 0);    
    if(enc.data != MAP_FAILED) {
        mem_info.service_id = ps_operation->session->service_id;
        mem_info.session_id = ps_operation->session->session_id;
        mem_info.user_mem_addr = (uint32_t)enc.data;
        ret = ioctl(ps_operation->session->device->fd, 
                OTZ_CLIENT_IOCTL_SHR_MEM_ALLOCATE_REQ, &mem_info);
        if(ret == -1) {
            ps_operation->s_errno = errno;
            goto free_map_data;
        }
    }
    else {
        perror("memory_allocate - mmap failed\n"); 
        ps_operation->s_errno = errno;
        enc.data = NULL;
        ps_operation->enc_dec.enc_error_state =  OTZ_ERROR_ENCODE_MEMORY;
        goto return_func;        
    }

    enc.len  = length;
    enc.param_type = param_type;
    enc.offset = 0;
    enc.flags = OTZ_MEM_SERVICE_RW;

    ret = ioctl(ps_operation->session->device->fd, 
                   OTZ_CLIENT_IOCTL_ENC_ARRAY_SPACE, &enc);
    if (ret) {
        ps_operation->s_errno = errno;
        goto free_map_data;
    }
    else {
        ps_operation->enc_dec.encode_id = enc.encode_id;
        ps_operation->temp_mem[ps_operation->temp_mem_ref_count].shared_mem 
                                                        = enc.data;

        ps_operation->temp_mem[ps_operation->temp_mem_ref_count].offset 
                                                                    = 0;
        ps_operation->temp_mem[ps_operation->temp_mem_ref_count].length 
                                                                    = length;
        ps_operation->temp_mem[ps_operation->temp_mem_ref_count].param_type 
                                                                   = param_type;
        ps_operation->temp_mem_ref_count++;        
    }
    return enc.data;
  
free_map_data:
    mem_info.service_id = ps_operation->session->service_id;
    mem_info.session_id = ps_operation->session->session_id;
    mem_info.user_mem_addr = (uint32_t)enc.data;
    munmap(enc.data, length);
    ioctl(ps_operation->session->device->fd,
            OTZ_CLIENT_IOCTL_SHR_MEM_FREE_REQ, &mem_info);
    enc.data = NULL;
    ps_operation->enc_dec.enc_error_state =  OTZ_ERROR_ENCODE_MEMORY;

return_func:
    return enc.data;
}

/**
 * @brief Appends a reference of previously allocated shared block to the
 * encoded buffer
 *
 * Calling this function appends a reference to a range of a previously created 
 * shared memory block.
 *
 * Memory references are used to provide a synchronization token protocol which 
 * informs the service when it can read from or write to a portion of the shared
 * memory block. A memory reference is associated with a specific operation and 
 * is valid only during the execution of that operation.
 *
 * @param ps_operation
 * @param ps_shared_mem
 * @param offset
 * @param length
 * @param flags
 * @param param_type
 */
void otz_encode_memory_reference( otz_operation_t* ps_operation,  
                            otz_shared_mem_t* ps_shared_mem,
                            uint32_t offset,
                            uint32_t length,
                            uint32_t flags,
                            int param_type)
{    
    struct otz_client_encode_cmd enc;
    int ret;
    
    if(check_encode(ps_operation))
        goto return_func;

    if(ps_shared_mem == NULL) {
        ps_operation->enc_dec.enc_error_state = OTZ_ERROR_ENCODE_FORMAT;
        goto return_func;
    }

    if((flags == OTZ_MEM_SERVICE_RO) && 
        (ps_shared_mem->ui_flags != OTZ_MEM_SERVICE_RO && 
         ps_shared_mem->ui_flags != OTZ_MEM_SERVICE_RW)) {
        ps_operation->enc_dec.enc_error_state = OTZ_ERROR_ENCODE_FORMAT;
        goto return_func;
    }

    if((flags == OTZ_MEM_SERVICE_WO) && 
        (ps_shared_mem->ui_flags != OTZ_MEM_SERVICE_WO && 
         ps_shared_mem->ui_flags != OTZ_MEM_SERVICE_RW)) {
        ps_operation->enc_dec.enc_error_state = OTZ_ERROR_ENCODE_FORMAT;
        goto return_func;
    }
    
    if(param_type == OTZ_PARAM_IN && flags == OTZ_MEM_SERVICE_WO) {
        ps_operation->enc_dec.enc_error_state = OTZ_ERROR_ENCODE_FORMAT;
        goto return_func;
    }

    if(param_type == OTZ_PARAM_OUT && flags == OTZ_MEM_SERVICE_RO) {
        ps_operation->enc_dec.enc_error_state = OTZ_ERROR_ENCODE_FORMAT;
        goto return_func;
    }

    if(((offset + length) >  ps_shared_mem->ui_length)) {
        ps_operation->enc_dec.enc_error_state =  OTZ_ERROR_ENCODE_MEMORY;
        goto return_func;
    }
    
    if(ps_operation->shared_mem_ref_count >= MAX_MEMBLOCKS_PER_OPERATION) {
        ps_operation->enc_dec.enc_error_state =  OTZ_ERROR_ENCODE_MEMORY;
        goto return_func;
    }
    
    enc.encode_id = ps_operation->enc_dec.encode_id; 
    enc.cmd_id = ps_operation->enc_dec.cmd_id; 
    enc.service_id = ps_operation->session->service_id ;
    enc.session_id = ps_operation->session->session_id ;

    enc.data = ps_shared_mem->p_block;
    enc.len  = length;
    enc.offset = offset;
    enc.flags = flags;
    enc.param_type = param_type;

    ret = ioctl(ps_operation->session->device->fd, 
                OTZ_CLIENT_IOCTL_ENC_MEM_REF, &enc);
    if (ret){
        ps_operation->enc_dec.enc_error_state = OTZ_ERROR_ENCODE_MEMORY;
        ps_operation->s_errno = errno;
    }
    else {
        ps_operation->enc_dec.encode_id = enc.encode_id;
        ps_operation->shared_mem[ps_operation->shared_mem_ref_count].shared_mem 
                                                        = ps_shared_mem;

        ps_operation->shared_mem[ps_operation->shared_mem_ref_count].offset 
                                                                    = offset;
        ps_operation->shared_mem[ps_operation->shared_mem_ref_count].length 
                                                                    = length;
        ps_operation->shared_mem[ps_operation->shared_mem_ref_count].param_type 
                                                                   = param_type;
        ps_operation->shared_mem_ref_count++;
    }
return_func:     
    return;
}

/**
 * @brief decode check 
 *
 * @param ps_operation
 *
 * @return 
 */
static int otz_check_decode(otz_operation_t* ps_operation)
{
    if(!ps_operation) {
        return -1;
    }
    
    if (ps_operation->ui_state != OTZ_STATE_DECODE) {
        ps_operation->enc_dec.dec_error_state = OTZ_ERROR_ILLEGAL_STATE;
        return -1;
    }

    if(ps_operation->enc_dec.dec_error_state != OTZ_SUCCESS)
        return -1;

    return 0; 
}


/**
 * @brief Decode a unsigned 32-bit integer value from message
 *
 * This function decodes a single item of type uint32_t from the current offset 
 * in the structured message returned by the service.
 *
 * @param ps_operation
 *
 * @return 
 */
uint32_t otz_decode_uint32( otz_operation_t* ps_operation)
{
    int ret = 0;
    struct otz_client_encode_cmd dec;

    if(otz_check_decode(ps_operation)) {
       *((uint32_t*)dec.data) = 0;
       goto return_func;
    }

    dec.encode_id = ps_operation->enc_dec.encode_id; 
    dec.cmd_id = ps_operation->enc_dec.cmd_id; 
    dec.service_id = ps_operation->session->service_id ;
    dec.session_id = ps_operation->session->session_id ;
    dec.len  = sizeof(uint32_t);

    ret = ioctl(ps_operation->session->device->fd, 
                    OTZ_CLIENT_IOCTL_DEC_UINT32, &dec);
    if (ret) {
        ps_operation->enc_dec.dec_error_state = OTZ_ERROR_DECODE_NO_DATA;
        ps_operation->s_errno = errno;
       *((uint32_t*)dec.data) = 0;
       goto return_func;
    }
return_func:
    return *((uint32_t*)dec.data);

}

/**
 * @brief  Decode a block of binary data from the message
 *
 * This function decodes a block of binary data from the current offset 
 * in the structured message returned by the service. 
 * The length of the block is returned in *pui_length and the base pointer is 
 * the function return value.
 *
 * @param ps_operation
 * @param plength
 *
 * @return 
 */
void* otz_decode_array_space(otz_operation_t* ps_operation, uint32_t *plength)
{
    int ret = 0;
    struct otz_client_encode_cmd dec;

    if(otz_check_decode(ps_operation)) {
        *plength = 0;
        dec.data = NULL;
        goto return_func;
    }

    dec.encode_id = ps_operation->enc_dec.encode_id; 
    dec.cmd_id = ps_operation->enc_dec.cmd_id; 
    dec.service_id = ps_operation->session->service_id ;
    dec.session_id = ps_operation->session->session_id ;

    ret = ioctl(ps_operation->session->device->fd, 
                    OTZ_CLIENT_IOCTL_DEC_ARRAY_SPACE, &dec);
    if (ret)
    {
        *plength = 0;
        ps_operation->enc_dec.dec_error_state = OTZ_ERROR_DECODE_NO_DATA;
        ps_operation->s_errno = errno;
        dec.data = NULL;
        goto return_func;
    }

    *plength = dec.len;
return_func:    
    return (void*)dec.data;

}

/**
 * @brief Returns the decoder stream data type
 *
 * This function returns the type of the data at the current offset 
 * in the decoder stream.
 *
 * @param ps_operation
 *
 * @return 
 */
uint32_t otz_decode_get_type(otz_operation_t* ps_operation)
{
    int type = OTZ_TYPE_NONE, ret;
    struct otz_client_encode_cmd dec;

    if(!ps_operation)
        goto return_func;
    
    if (ps_operation->ui_state != OTZ_STATE_DECODE) 
        goto return_func;

    if(ps_operation->enc_dec.dec_error_state != OTZ_SUCCESS)
        goto return_func;

    dec.encode_id = ps_operation->enc_dec.encode_id; 
    dec.cmd_id = ps_operation->enc_dec.cmd_id; 
    dec.service_id = ps_operation->session->service_id ;
    dec.session_id = ps_operation->session->session_id ;

    ret = ioctl(ps_operation->session->device->fd, 
                    OTZ_CLIENT_IOCTL_GET_DECODE_TYPE, &dec);
    if (ret)
    {
        ps_operation->s_errno = errno;
        goto return_func;
    }

    if((uint32_t)dec.data == OTZ_ENC_UINT32)
        type = OTZ_TYPE_UINT32;
    else if(((uint32_t)dec.data == OTZ_ENC_ARRAY) || 
            ((uint32_t)dec.data == OTZ_MEM_REF))
        type = OTZ_TYPE_ARRAY;        


return_func:    
    return type;
}

/**
 * @brief Get decode error
 *
 * This function returns the error state of the decoder associated 
 * with the given operation.
 *
 * This function does not affect the error state of the decoder.
 *
 * @param ps_operation
 *
 * @return 
 */
otz_return_t otz_decode_get_error(otz_operation_t* ps_operation)
{
    return  ps_operation->enc_dec.dec_error_state;
}

