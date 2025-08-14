/**
 * @file tkl_cellular_mds.h
 * @author www.tuya.com
 * @brief 蜂窝模组数据API实现接口。
 *
 * @copyright Copyright (c) tuya.inc 2021
 */

#ifndef __TKL_CELLULAR_MDS_H__
#define __TKL_CELLULAR_MDS_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 蜂窝移动数据鉴权状态
 */
typedef enum
{
    TUYA_CELLULAR_MDS_STATUS_UNKNOWN = 0,
    TUYA_CELLULAR_MDS_STATUS_IDLE = 1,  /*< 空闲状态 */
    TUYA_CELLULAR_MDS_STATUS_REG,       /*< PS域注册 */
    TUYA_CELLULAR_MDS_STATUS_ACTIVE,    /*< PDP激活 */
    TUYA_CELLULAR_MDS_STATUS_CAMPED,    /*< 拒绝注册 */
}TUYA_CELLULAR_MDS_STATUS_E;

/**
 * @brief 蜂窝网络状态
 */
typedef enum
{
    TUYA_CELLULAR_MDS_NET_DISCONNECT = 0,
    TUYA_CELLULAR_MDS_NET_CONNECT = 1,
}TUYA_CELLULAR_MDS_NET_STATUS_E;

/**
 * @brief 蜂窝网络IP类型
 */
typedef enum{
    TUYA_MDS_PDP_IPV4 = 0,
    TUYA_MDS_PDP_IPV6,
    TUYA_MDS_PDP_IPV4V6
}TUYA_MDS_PDP_TYPE_E;

/**
 * @brief 蜂窝网络状态变化通知函数原型，该接口是为了svc_netmgr适配
 * @param simId sim卡ID
 * @param state 蜂窝网络状态，查看 @TUYA_CELLULAR_MDS_NET_STATUS_E 定义
 */
typedef void (*TKL_MDS_NOTIFY)(TUYA_CELLULAR_MDS_NET_STATUS_E st);

/**
 * @brief 初始化蜂窝移动数据服务
 * @param simId sim卡ID
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_mds_init(uint8_t sim_id);

/**
 * @brief 获取蜂窝移动数据服务的鉴权状态
 * @param simId sim卡ID
 * @return 蜂窝移动数据鉴权状态，查看 @TUYA_CELLULAR_MDS_STATUS_E 定义
 */
TUYA_CELLULAR_MDS_STATUS_E tkl_cellular_mds_get_status(uint8_t sim_id);

/**
 * @brief 获取蜂窝移动数据服务的鉴权状态
 * @param simId sim卡ID
 * @return 蜂窝移动数据鉴权状态，查看 @TUYA_CELLULAR_MDS_STATUS_E 定义
 */
TUYA_CELLULAR_MDS_STATUS_E tkl_cellular_mds_adv_get_status(uint8_t sim_id,uint8_t cid);

/**
 * @brief 蜂窝移动数据PDP激活，默认使用CID为1
 * @param simId sim卡ID
 * @param apn 运营商APN设置
 * @param username 用户名
 * @param password 密码
 *
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_mds_pdp_active(uint8_t sim_id,char * apn, char * username, char * password);

/**
 * @brief 蜂窝移动数据指定CID PDP激活
 * @param simId sim卡ID
 * @param cid Specify the PDP Context Identifier
 * @param apn 运营商APN设置
 * @param username 用户名
 * @param password 密码
 *
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_mds_adv_pdp_active(uint8_t sim_id,uint8_t cid,TUYA_MDS_PDP_TYPE_E pdp_type,char * apn, char * username, char * password);

/**
 * @brief 蜂窝移动数据PDP去激活，默认使用CID为1
 * @param simId sim卡ID
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_mds_pdp_deactive(uint8_t sim_id);

/**
 * @brief 蜂窝移动数据指定CID PDP去激活
 * @param simId sim卡ID
 * @param cid Specify the PDP Context Identifier
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_mds_adv_pdp_deactive(uint8_t sim_id,uint8_t cid);

/**
 * @brief 蜂窝移动数据PDP自动重激活设置
 * @param simId sim卡ID
 * @param enable TRUE 开启自动重新激活 FALSE 关闭自动重新激活
 *
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_mds_pdp_auto_reactive(uint8_t sim_id,bool enable);

/**
 * @brief 注册蜂窝数据服务状态变化通知函数
 * @param fun 状态变化通知函数
 * @return 0 成功  其它 失败
 */
OPERATE_RET tkl_cellular_mds_register_state_notify(uint8_t sim_id, TKL_MDS_NOTIFY fun);

/**
 * @brief   Get device ip address.
 * @param   ip: The type of NW_IP_S
 * @return  OPRT_OK: success  Other: fail
 */
OPERATE_RET tkl_cellular_mds_get_ip(uint8_t sim_id,NW_IP_S *ip);

/**
 * @brief   Get device ip address.
 * @param   ip: The type of NW_IP_S
 * @return  OPRT_OK: success  Other: fail
 */
OPERATE_RET tkl_cellular_mds_adv_get_ip(uint8_t sim_id,uint8_t cid,NW_IP_S *ip);

#ifdef __cplusplus
}
#endif

#endif
