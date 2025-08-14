#ifndef __OL_NW_API__
#define __OL_NW_API__

#include "ps_lib_api.h"

typedef enum
{
   OL_CID_INDEX_INVALID = 0,
   OL_CID_INDEX_1,
   OL_CID_INDEX_2,
   OL_CID_INDEX_3,
   OL_CID_INDEX_4,
   OL_CID_INDEX_5,
   OL_CID_INDEX_6,
   OL_CID_INDEX_7,
   OL_CID_INDEX_8,
   OL_CID_INDEX_9,
   OL_CID_INDEX_10,
   OL_CID_INDEX_11,
   OL_CID_INDEX_12,
   OL_CID_INDEX_13,
   OL_CID_INDEX_14,
   OL_CID_INDEX_15,
   OL_CID_INDEX_MAX
}ol_cid_index_enum;

typedef enum
{
    OL_IPTYPE_INVALID = 0,
    OL_IPTYPE_V4,
    OL_IPTYPE_V6,
    OL_IPTYPE_V4V6,
    OL_IPTYPE_MAX
}ol_iptype_enum;

typedef enum
{
    OL_AUTH_NONE,
    OL_AUTH_PAP,
    OL_AUTH_CHAP,
    OL_AUTH_PAP_CHAP
}ol_auth_enum;

typedef enum
{
    OL_CID_DEACTIVE_REQ,
    OL_CID_ACTIVE_REQ
}ol_cid_active_req_enum;

typedef enum
{
    OL_TA_UPDATING                              = 0, //TA updating
    OL_COMBINED_TA_LA_UPDATING                  = 1, //combined TA/LA updating
    OL_COMBINED_TA_LA_UPDATING_WITH_IMSI_ATTACH = 2, //combined TA/LA updating with IMSI attach
    OL_PERIODIC_TA_UPDATING                     = 3  //periodic updating
}ol_tau_type_e;

typedef struct
{
    ol_iptype_enum iptype;
    char v4_addr[32];
    char v6_addr[64];
}ol_cid_info_struct;

typedef void (*ping_cb_t)(uint32_t recv_byte,uint32_t time,uint32_t ttl);

/*****************************************************************************
 * DESCRIPTION
 *    This API is to set the iptype and apn of cid
 *
 * PARAMETERS
 *    cid       : [IN]  cell id:ol_cid_index_enum
 *    iptype    : [IN]  ip address type:ol_iptype_enum
 *    apn       : [IN]  APN
 *
 * RETURN
 *    =0 : success
 *    <0 : fails
 *****************************************************************************/
extern int ol_cid_set(ol_cid_index_enum cid,ol_iptype_enum iptype,char* apn);


/*****************************************************************************
 * DESCRIPTION
 *    This API is to get the iptype and apn of cid
 *
 * PARAMETERS
 *    cid       : [IN]  cell id:ol_cid_index_enum
 *    iptype    : [OUT] the pointer to the ip address type
 *    apn       : [OUT] APN
 *
 * RETURN
 *    =0 : success
 *    <0 : fails
 *****************************************************************************/
extern int ol_cid_get(ol_cid_index_enum cid,ol_iptype_enum* iptype,char* apn);


/*****************************************************************************
 * DESCRIPTION
 *    This API is to set the authentication type��the username and password of the cid
 *
 * PARAMETERS
 *    cid       : [IN]  cell id:ol_cid_index_enum
 *    type      : [IN]  the authentication type:ol_auth_enum
 *    usr       : [IN]  username
 *    pwd       : [IN]  password
 *
 * RETURN
 *    =0 : success
 *    <0 : fails
 *****************************************************************************/
extern int ol_auth_set(ol_cid_index_enum cid,ol_auth_enum type,char* usr,char* pwd);


/*****************************************************************************
 * DESCRIPTION
 *    This API is to get the authentication type��the username and password of the cid
 *
 * PARAMETERS
 *    cid       : [IN]  cell id:ol_cid_index_enum
 *    type      : [OUT] the pointer to the authentication type
 *    usr       : [OUT] username
 *    pwd       : [OUT] password
 *
 * RETURN
 *    =0 : success
 *    <0 : fails
 *****************************************************************************/
extern int ol_auth_get(ol_cid_index_enum cid,ol_auth_enum* type,char* usr,char* pwd);


/*****************************************************************************
 * DESCRIPTION
 *    This API is to set cid active state
 *
 * PARAMETERS
 *    cid       : [IN]  cell id:ol_cid_index_enum
 *    act_sta   : [IN]  active status:ol_cid_active_req_enum
 *
 * RETURN
 *    =0 : success
 *    <0 : fails
 *****************************************************************************/
extern int ol_set_cid_status(ol_cid_index_enum cid,ol_cid_active_req_enum act_sta);


/*****************************************************************************
 * DESCRIPTION
 *    This API is to get cid active state
 *
 * PARAMETERS
 *    cid       : [IN]  cell id:ol_cid_index_enum
 *    act_sta   : [IN]  the pointer to the active status
 *
 * RETURN
 *    =0 : success
 *    <0 : fails
 *****************************************************************************/
extern int ol_get_cid_status(ol_cid_index_enum cid,ol_cid_active_req_enum* act_sta);


/*****************************************************************************
 * DESCRIPTION
 *    This API is to get cid infomation
 *
 * PARAMETERS
 *    cid       : [IN]  cell id:ol_cid_index_enum
 *    act_sta   : [IN]  the pointer to the struct ol_cid_info_struct
 *
 * RETURN
 *    =0 : success
 *    <0 : fails
 *****************************************************************************/
extern int ol_get_cid_info(ol_cid_index_enum cid,ol_cid_info_struct* info);


/*****************************************************************************
 * DESCRIPTION
 *    This API is to set dns information
 *
 * PARAMETERS
 *    iptype        : [IN]  the ip address type
 *    pri_dns       : [IN]  dns information
 *    sec_nds       : [IN]  dns information
 *
 * RETURN
 *    =0 : success
 *    <0 : fails
 *****************************************************************************/
extern int ol_set_dns(ol_iptype_enum iptype, char *pri_dns, char *sec_dns);


/*****************************************************************************
 * DESCRIPTION
 *    This API is to get dns information
 *
 * PARAMETERS
 *    iptype        : [IN]  the ip address type
 *    pri_dns       : [OUT] dns information
 *    sec_nds       : [OUT] dns information
 *
 * RETURN
 *    =0 : success
 *    <0 : fails
 *****************************************************************************/
extern int ol_get_dns(ol_iptype_enum iptype, char *pri_dns, char *sec_dns);


/*****************************************************************************
 * DESCRIPTION
 *    This API is to request PS to measure/search neighber cell
 *    Note: if function  return fails, will get the last successful cell info
 *
 * PARAMETERS
 *    pBcListInfo    : [OUT] Pointer to store the result basic cell info
 *
 * RETURN
 *    =0 : success
 *    <0 : fails
 *****************************************************************************/
extern int ol_get_cellinfo(BasicCellListInfo *pBcListInfo);


/*****************************************************************************
 * DESCRIPTION
 *    This API is to get Location Based Service(LBS) data
 *    Note: api function cannot be accessed repeatedly within 10 minutes 
 *          after the last successful attempt
 *
 * PARAMETERS
 *    hostname       : [IN]  the hostname of the server
 *    port           : [IN]  the server port
 *    info           : [OUT] ponit to output lbs string info "lbs: longitude: xxx, latitude: xxx"
 *    timeout_sec    : [IN]  Timeout waiting for server return lbs info
 *
 * RETURN
 *    =0 : success
 *    <0 : fails
 *****************************************************************************/
extern int ol_get_lbs_info(char * hostname, uint32_t port, char *info, uint32_t timeout_sec);


/*****************************************************************************
 * DESCRIPTION
 *    This API is to get Location Based Service(LBS) data through the amap key
 *    Note: api function cannot be accessed repeatedly within 10 minutes 
 *          after the last successful attempt
 *
 * PARAMETERS
 *    key               : [IN] The key applied from Amap
 *    pIndBuf           : [OUT] ponit to output lbs string info "lbs: longitude: xxx, latitude: xxx"
 *
 * RETURN
 *    =0 : success
 *    <0 : fails
 *****************************************************************************/
extern int ol_lbs_gdhttp_info(char *key, char *pIndBuf);


/*****************************************************************************
 * DESCRIPTION
 *    This API is to register the callback when ping url/ip
 *
 * PARAMETERS
 *    callback:the register callback
 *
 * RETURN
 *    =0 : success
 *    <0 : fails
 *****************************************************************************/
extern int ol_ping_register_callback(ping_cb_t callback);

/*****************************************************************************
 * DESCRIPTION
 *    This API is to trigger RRC local Release
 *
 * PARAMETERS
 *    NULL
 *
 * RETURN
 *    =0 : success
 *    <0 : fails
 *****************************************************************************/
extern int ol_rrc_release(void);

/*****************************************************************************
 * DESCRIPTION
 *    This API is to trigger TAU
 *
 * PARAMETERS
 *    tau_type:tau type(ol_tau_type_e)
 *
 * RETURN
 *    =0 : success
 *    <0 : fails
 *****************************************************************************/
extern int ol_tau_update(ol_tau_type_e tau_type);

#endif /*__OL_NW_API__*/
