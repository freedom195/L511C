/**
 * @file tkl_cellular_base.h
 * @author www.tuya.com
 * @brief 蜂窝模组基础功能API实现接口。
 *
 * @copyright Copyright (c) tuya.inc 2021
 */
#ifndef __TKL_CELLULAR_H__
#define __TKL_CELLULAR_H__

#include "tuya_cloud_types.h"
#include "tkl_cellular_mds.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 蜂窝设备基本的IOCTL命令 */
typedef enum
{
	CELL_IOCTL_SET_PLMN,						//设置PLMN
	CELL_IOCTL_GET_PLMN,						
	CELL_IOCTL_SET_BAND,						//设置频段
	CELL_IOCTL_GET_BAND,						//获取频段
// 设置蜂窝的模块没有数据收发的检测时间，如果在超过的设置时间，蜂窝就主动释放RRC。 目的是进一步优化功耗。时间越小，有可能造成数据的丢包，慎重使用。
	CELL_IOCTL_SET_RRC_RELEASE_TIME,          	//设置rrc 释放时间, 
	CELL_IOCTL_GET_RRC_RELEASE_TIME,          	//获取rrc 释放时间
	CELL_IOCTL_GET_DATA_STATICS,              	//获取数据统计

	CELL_IOCTL_SET_PWRKEY_SHUTDOWN_TIME = 100,	//设置按键关机时长，为0时，关闭该功能,	
	CELL_IOCTL_CLOSE_WAKEUP_MODULE ,			//关闭低功耗下通过wakeup唤醒模组功能，释放唤醒脚
	CELL_IOCTL_GET_USB_INSERT_STATUS,			//获取usb插入状态
	CELL_INIT_VIRTUAL_AT,               		//初始化虚拟AT服务
	CELL_IOCTL_SEND_VIRTUAL_AT_CMD,				//发送虚拟AT命令	
	CELL_IOCTL_GET_SYS_VER,						//获取系统版本
	CELL_IOCTL_GET_MODULE,						//获取模块型号
	CELL_IOCTL_GET_RF_CALIBRATED,				//获取RF校准状态
	CELL_IOCTL_SET_NET_TYPE,
}CELL_IOCTRL_CMD_E;

/**
 * @brief sim卡状态定义
 */
typedef enum
{
    TKL_NO_SIM,     /*< 无SIM卡 */
    TKL_SIM_READY,  /*< SIM已识别 */
    TKL_SIM_INIT,   /*< SIM卡初始化中>*/
    TKL_SIM_WAIT_PIN,
    TKL_SIM_WAIT_PUK,
} TKL_SIM_STATE_E;

typedef enum{
    TUYA_TSIM_TYPE, /* 物理卡 */
    TUYA_VSIM_TYPE, /* 虚拟卡 */
}TUYA_SIM_TYPE_E;

typedef enum {
  	DOUBLE_SIM_DOUBLE_CHANNEL = 1,
  	DOUBLE_SIM_SINGLE_CHANNEL = 2,
  	SINGLE_SIM_SINGLE_CHANNEL = 4,
}TKL_CELLULAR_ABILITY_E;

#define NEIGHBOUR_NUM   4
typedef struct {
	char mcc[3];
	char mnc[3];
	uint16_t lac;           //LAC
	int16_t  rx_pwr;
	uint32_t   cellid;        //20bits(eNodeID)+8bits(Cellid)
}TKL_CELL_INFO_T;

typedef struct
{
	char mcc[3];
	char mnc[3];
	TKL_CELL_INFO_T main;
	TKL_CELL_INFO_T neighbour[NEIGHBOUR_NUM];
}TKL_LBS_INFO_T;

typedef struct {
   int fd;
   char name[32+1];
   TUYA_UART_BASE_CFG_T cfg;
   uint8_t              sim_id;
   TUYA_SIM_TYPE_E      sim_type;
}TKL_CELL_INIT_PARAM_T;

/**
 * @brief sim卡状态变化通知函数原型
 * @param state sim卡状态，查看 @TAL_SIM_STATE_E 定义
 */
typedef void (*TKL_SIM_NOTIFY)(TKL_SIM_STATE_E status);

/** 
 * @brief 蜂窝网络基础功能初始化
 */
OPERATE_RET tkl_cellular_base_init(TKL_CELL_INIT_PARAM_T *param);

/**
 * @brief 蜂窝网络注册状态变化通知函数原型
 * @param simId sim卡ID
 * @param state 蜂窝网络状态，查看 @TUYA_CELLULAR_MDS_STATUS_E 定义，只需上报
 *              TUYA_CELLULAR_MDS_STATUS_IDLE，TUYA_CELLULAR_MDS_STATUS_REG,TUYA_CELLULAR_MDS_STATUS_CAMPED 3个事件。
 */
typedef void (*TKL_REGISTION_NOTIFY)(TUYA_CELLULAR_MDS_STATUS_E st);

/**
 * @brief 获取当前设备的通讯能力
 * @param ability @TKL_CELLULAR_ABILITY_E 类型
 * @return 0 成功  其它 失败
 */
OPERATE_RET tkl_cellular_base_get_ability(TKL_CELLULAR_ABILITY_E *ability);

/**
 * @brief 切换当前使能的SIM卡。
 * @param simid SIM卡ID.(0~1)
 * @return 0 成功  其它 失败
 */
OPERATE_RET tkl_cellular_base_switch_sim(uint8_t sim_id);

/**
 * @brief 注册SIM状态变化通知函数
 * @param fun 状态变化通知函数
 * @return 0 成功  其它 失败
 */
OPERATE_RET tkl_cellular_base_register_sim_state_notify(uint8_t simd_id,TKL_SIM_NOTIFY fun);

/**
 * @brief 使能或禁止sim卡热拔插
 * @param simId sim卡ID
 * @param enable TRUE 使能 FALSE 禁止
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_base_enable_sim_hotplug(uint8_t sim_id, bool enable);

/**
 * @brief 获取SIM卡的状态
 * @param simId sim卡ID
 * @param state 1：正常，0：异常，2：初始化中
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_base_sim_get_status(uint8_t sim_id, uint8_t *state);

/**
 * @brief 获取蜂窝设备当前的通信功能设置
 *
 * @param cfun 获取的通信功能
 *
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_base_get_cfun_mode(uint8_t simd_id, int *cfun);

/**
 * @brief 设置蜂窝设备的通信功能模式
 *
 * @param cfun 通信功能，取值含义如下：
 *            1：全功能模式
 *            4：飞行模式
 *
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_base_set_cfun_mode(uint8_t simd_id,int cfun);

/**
 * @brief 获取SIM卡中的国际移动用户识别码
 *
 * @param simid,SIM卡id号(0,1,2...)
 * @param imsi识别码，为16字节的字符串
 *
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_base_get_imsi(uint8_t sim_id,char imsi[15 + 1]);

/**
 * @brief 获取SIM卡的ICCID
 * @param simid
 * @param ICCID识别码，为20字节的字符串
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_base_get_iccid(uint8_t sim_id,char iccid[20 + 1]);

/**
 * @brief 获取SIM卡所在通道设备的IMEI号
 * @param simid
 * @param IMEI识别码，为15字节的字符串
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_base_get_imei(uint8_t sim_id,char imei[15 + 1]);

/**
 * @brief 设置设备的IMEI号
 * @param simid
 * @param IMEI识别码，为15字节的字符串
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_base_set_imei(uint8_t sim_id,char imei[15 + 1]);

/**
 * @brief 获取SIM卡所在通道蜂窝设备的信号接收功率——单位dbm
 * @param simid
 * @param rsrp 返回实际的信号强度(dbm)
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_base_get_rsrp(uint8_t sim_id,int *rsrp);

/**
 * @brief 获取蜂窝设备SIM卡所在通道的信号噪声比及误码率
 * @param simid
 * @param sinr (0~31)
 * @param bit_error (0~7,99) 99无网络
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_base_get_sinr(uint8_t sim_id,int *sinr,int *bit_error);

/**
 * @brief SIM卡所在通道LBS的基站信息)
 * @param simid
 * @param lbs 返回基站信息
 * @param neighbour 是否搜索临近基站信息
 * @param timeout 搜索临近基站信息超时时间(一般需要4秒左右)
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_base_get_lbs(uint8_t sim_id,TKL_LBS_INFO_T *lbs,bool neighbour,int timeout);

/**
 * @brief 获取当前设备的射频校准状态
 * @return TRUE正常，FALSE异常
 */
bool tkl_cellular_base_rf_calibrated(void);

/**
 * @brief 使能或禁止sim卡gpio检测
 * @param simId sim卡ID
 * @param enable TRUE 使能 FALSE 禁止
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_base_enable_sim_detect(uint8_t simid, bool enable);

/**
 * @brief 获取默认的SIM ID
 * @return 小于0失败，其他SIM ID
 */
int8_t tkl_cellular_base_get_default_simid(void);

/**
 * @brief 蜂窝基础的通用控制功能，一般作为平台提供一些特殊的能力接口
 * @param cmd 参考CELL_IOCTRL_CMD
 * @param argv 平台自定义
 * @return 0 成功 其它 失败
 */
OPERATE_RET tkl_cellular_base_ioctl(int cmd, void* argv);

/**
 * @brief 设置模组底层网络注册事件的回调
 * @note 主要底层上报TUYA_CELLULAR_MDS_STATUS_IDLE，TUYA_CELLULAR_MDS_STATUS_REG,TUYA_CELLULAR_MDS_STATUS_CAMPED 3个事件。
 * 分别表示搜网中，注网成功，停止搜网
 * @param fun 回调函数
 * @return OPERATE_RET 操作结果，成功返回OPRT_OK，失败返回错误码
 */
OPERATE_RET tkl_cellular_register_dev_reg_notify(uint8_t sim_id, TKL_REGISTION_NOTIFY fun);

#ifdef __cplusplus
}
#endif

#endif
