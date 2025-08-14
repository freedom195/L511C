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

typedef enum
{
    TUYA_CELL_NET_TYPE_UNKNOWN = -1,
    TUYA_CELL_NET_TYPE_GSM = 1,
    TUYA_CELL_NET_TYPE_LTE,
    TUYA_CELL_NET_TYPE_3G,
    TUYA_CELL_NET_TYPE_CATM, //CATM1
    TUYA_CELL_NET_TYPE_NB,
}TUYA_CELLULAR_RAT_E;

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
 * @brief 获取RSSI dBm值
 * @param sim_id 对应的SIM卡ID(0~1)
 * @param rssi_dBm:信号强度指针，单位dBm
 * @return 0 成功  其它 失败
 */
OPERATE_RET tkl_cellular_get_rssidbm(uint8_t sim_id,int *rssi_dBm);

/**
 * @brief 获取当前蜂窝设备网络注册成功后，当前的网络类型
 * @param sim_id sim id
 * @param net_type 获取的网络类型指针
 * @return 0 成功  其它 失败
 */
OPERATE_RET tkl_cellular_get_nettype(uint8_t sim_id, TUYA_CELLULAR_RAT_E *net_type);

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
OPERATE_RET tkl_cellular_get_rssi(uint8_t sim_id,int *rssi);

/**
 * @brief 获取当前的 Unix 时间戳（以秒为单位）
 * @param epoch_sec 用于存储 Unix 时间戳的指针，函数执行成功后，该指针指向的变量将被设置为当前的 Unix 时间戳（以秒为单位）
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_get_epoch_time(uint64_t *epoch_sec);

/**
 * @brief 获取本次系统上电后的累计时间，单位ms
 * @param up_ms 用于存储上电累计时间时的指针
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_get_poweron_time(uint64_t *up_ms);

/**
 * @brief 根据基站同步的时间，获取蜂窝的设备的本地时间
 * @param local_tm 用于存储本地时间的指针，函数执行成功后，该指针指向的结构体将被设置为当前的本地时间
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_get_localtime(struct tm *local_tm);

/**
 * @brief 根据基站同步的时间，获取时区信息
 * @param timezone 获取的时区的指针
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_get_timezone(int *timezone);

/**
 * @brief 获取蜂窝SN号
 * @param sn 设置的SN字符串
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_get_sn(char sn[25]);

/**
 * @brief 设置蜂窝SN号
 * @param sim_id (0~1)
 * @param sn 获取的SN字符串
 * @return OPERATE_RET 类型，0 表示成功，其他值表示失败
 */
OPERATE_RET tkl_cellular_set_sn(char sn[25]);

/**
 * @brief 蜂窝基础的通用控制功能，一般作为平台提供一些特殊的能力接口
 * @param cmd 参考CELL_IOCTRL_CMD
 * @param argv 平台自定义
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_ioctl(int cmd,void* argv);

#ifdef __cplusplus
}
#endif
#endif
