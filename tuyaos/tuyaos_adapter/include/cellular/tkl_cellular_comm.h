/**
 * @file tkl_cellular_comm.h
 * @brief
 * @version 0.1
 * @date 2023-8-22
 *
 * @copyright Copyright (c) 2021-2023 Tuya Inc. All Rights Reserved.
 *
 * Permission is hereby granted, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), Under the premise of complying
 * with the license of the third-party open source software contained in the software,
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software.
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 */
#ifndef __TKL_CELLULAR_COMM_H
#define __TKL_CELLULAR_COMM_H

#include <time.h>
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
    int cid;
    uint32_t rx_count;
    uint32_t tx_count;
}TKL_CELL_DATA_STATISTICS;

typedef enum
{
    TUYA_CELL_NET_TYPE_UNKNOWN = -1,
    TUYA_CELL_NET_TYPE_GSM = 1,
    TUYA_CELL_NET_TYPE_LTE,
    TUYA_CELL_NET_TYPE_3G,
    TUYA_CELL_NET_TYPE_CATM, //CATM1
    TUYA_CELL_NET_TYPE_NB,
}TUYA_CELLULAR_RAT_E;

typedef struct {
    int8_t rat;             // -1无效
    int8_t act1;            // -1无效
    int8_t act2;            // -1无效
    int8_t gsm_band_count;
    uint16_t gsm_band[4];   //850/900/1800/1900MHz
    int8_t lte_band_count;
    uint8_t lte_band[11];   //1~85,支持设置11个
}TKL_GTACT_T;

typedef struct
{
    int cur_plmn;         //当前设备注网的PLMN
    int8_t plmn_list_count; //当前设备支持制式所在国家的PLMN数量
    int *plmn_list;       //当前设备支持制式所在国家的PLMN 如46000
}PLMN_LIST_T;

/**
 * @brief 虚拟AT接收应答回调函数
 * @param resp 返回的AT应答
 * @param len  返回的AT应答数据长度
 */
typedef void (*TKL_VIRTAT_RESP)(char *resp,int len);

/**
 * @brief 控制蜂窝AP LOG输出
 * @param enable FLASE:输出关闭，TRUEL:输出打开
 * @return 0 成功  其它 失败
 */
OPERATE_RET tkl_cellular_comm_ctrl_ap_trace(bool enable);

/**
 * @brief 电源键关机功能使能控制
 * @param enable TRUE:允许，FALSE:禁止
 * @return 0 成功  其它 失败
 */
OPERATE_RET tkl_cellular_comm_ctrl_powerkey(bool enable);

/**
 * @brief 使能AT口功能
 * @param enable TRUE:允许串口AT，FALSE:禁止串口AT
 * @return 0 成功  其它 失败
 */
OPERATE_RET tkl_cellular_comm_ctrl_at_port(bool enable);

/**
 * @brief 获取RSSI dBm值
 * @param sim_id 对应的SIM卡ID(0~1)
 * @param rssi_dBm:信号强度指针，单位dBm
 * @return 0 成功  其它 失败
 */
OPERATE_RET tkl_cellular_comm_get_rssi_dBm(uint8_t sim_id,int *rssi_dBm);

/**
 * @brief 获取当前蜂窝设备网络注册成功后，当前的网络类型
 * @param sim_id sim id
 * @param net_type 获取的网络类型指针
 * @return 0 成功  其它 失败
 */
OPERATE_RET tkl_cellular_comm_get_nettype(uint8_t sim_id, TUYA_CELLULAR_RAT_E *net_type);

/**
 * @brief 获取当前蜂窝设备的参考信号质量，参考AT+CSQ
            <rssi>:
                0 113 dBm or less
                1 111 dBm
                2. . . 30 109. . . 53 dBm
                31 51 dBm or greater
                99 not known or not detectable
 * @param rssi 获取的信号强度指针
 * @return 0 成功  其它 失败
 */
OPERATE_RET tkl_cellular_comm_get_rssi(uint8_t sim_id,int *rssi);

/**
 * @brief 获取当前的 Unix 时间戳（以秒为单位）
 * @param epoch_sec 用于存储 Unix 时间戳的指针，函数执行成功后，该指针指向的变量将被设置为当前的 Unix 时间戳（以秒为单位）
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_comm_get_epoch_time(uint64_t *epoch_sec);

/**
 * @brief 获取本次系统上电后的累计时间，单位ms
 * @param up_ms 用于存储上电累计时间时的指针
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_comm_get_poweron_time(uint64_t *up_ms);

/**
 * @brief 根据基站同步的时间，获取蜂窝的设备的本地时间
 * @param local_tm 用于存储本地时间的指针，函数执行成功后，该指针指向的结构体将被设置为当前的本地时间
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_comm_ctrl_get_localtime(struct tm *local_tm);

/**
 * @brief 根据基站同步的时间，获取时区信息
 * @param timezone 获取的时区的指针
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_comm_get_timezone(int *timezone);

/**
 * @brief 获取蜂窝SN号
 * @param sn 设置的SN字符串
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_comm_get_sn(char sn[25]);

/**
 * @brief 设置蜂窝SN号
 * @param sim_id (0~1)
 * @param sn 获取的SN字符串
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_comm_set_sn(char sn[25]);

/**
 * @brief 设置RRC connect切换到idle的延迟时间
 * @param time 单位秒。
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_comm_set_rrc_release_time(uint32_t time);

/**
 * @brief 获取当前设备RRC connect切换到idle的延迟时间
 * @param time 单位秒。
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_comm_get_rrc_release_time(uint32_t *time);

/**
 * @brief 获取基础固件的版本号。该功能一般给双固件使用，应用固件调用该API可以获取到系统固件的版本号
 * @param ver 获取的版本号字符串
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_comm_get_sysfw_ver(char ver[32]);

/**
 * @brief 获取模组型号
 * @param module 获取模组型号名称
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_comm_get_module(char module[32]);

/**
 * @brief 设置PLMN
 * @note  指定模组注册到指定运营商。当模组注册指定运营商失败后，模组底层自动搜索可注册的运营商
 *        功能和AT+COPS=4,2,"46000"一样
 * @param plmn 为数字格式的运营商编码字符串，如"46000"
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_comm_set_plmn(char *plmn);

/**
 * @brief 设置虚拟AT命令应答的回调函数
 * @param resp_callback 设置的回调函数
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_comm_set_virtual_at_recv_cb(TKL_VIRTAT_RESP resp_callback);

/**
 * @brief 通过虚拟AT发送AT命令
 * @param at_cmd AT命令字符串
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_comm_send_virtual_at(char *at_cmd);



#ifdef __cplusplus
}
#endif
#endif
