/**
 * @file tkl_cellular_sms.h
 * @author www.tuya.com
 * @brief 蜂窝模组短信API实现接口。
 *
 * @copyright Copyright (c) tuya.inc 2021
 */

#ifndef __TKL_CELLULAR_SMS_H__
#define __TKL_CELLULAR_SMS_H__

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 收发短信时，手机号码最大长度
 */
#define TUYA_SMS_PHONENUM_LEN_MAX    24

/**
 * @brief 短信内容最大长度
 */
#define TUYA_SMS_MSG_LEN_MAX    160

/**
 * @brief 短信内容编码格式
 */
typedef enum{
    TUYA_SMS_ISO8859_1,
    TUYA_SMS_UTF8,
    TUYA_SMS_UTF16BE,
    TUYA_SMS_UTF16LE,
    TUYA_SMS_GSM,
    TUYA_SMS_CP936   //GBK编码
} TUYA_CELLULAR_SMS_ENCODE_E;

/**
 * @brief 发送短信结构体定义
 */
typedef struct
{
    char phone[TUYA_SMS_PHONENUM_LEN_MAX];  /*< 接收短信的手机号 */
    uint16_t msg_len;                        /*< 短信长度 */
    uint16_t msg[TUYA_SMS_MSG_LEN_MAX];     /*< 短信内容 */
    TUYA_CELLULAR_SMS_ENCODE_E   sms_encode;
} TUYA_CELLULAR_SMS_SEND_T;

/**
 * @brief 短信时间戳信息结构体定义
 */
typedef struct
{
    uint16_t year;  /*< 年 */
    uint8_t month;  /*< 月 */
    uint8_t day;    /*< 日 */
    uint8_t hour;   /*< 时 */
    uint8_t minute; /*< 分 */
    uint8_t second; /*< 秒 */
    uint8_t zone;   /*< 时区 */
} TUYA_CELLULAR_SMS_TIMESTAMP_T;

/**
 * @brief 接收短信结构体定义
 */
typedef struct
{
    char phone[TUYA_SMS_PHONENUM_LEN_MAX];  /*< 发送短信的手机号码 */
    TUYA_CELLULAR_SMS_TIMESTAMP_T  date;       /*< 短信接收时间 */
    int msg_len;                             /*< 短信内容长度 */
    char msg[TUYA_SMS_MSG_LEN_MAX];          /*< 短信内容 */
} TUYA_CELLULAR_SMS_RECV_T;

/**
 * @brief 短信接收回调函数接口原型
 * @note 回调函数中不要调用阻塞性函数，msg结构体内存由系统
 *       负责分配和释放。
 * @param simId sim卡ID
 * @param msg 短信结构体
 * @return 无
 */
typedef void (*TUYA_CELLULAR_SMS_CB)(uint8_t sim_id, TUYA_CELLULAR_SMS_RECV_T* msg);

/**
 * @brief 发送短信
 *
 * @param simId sim卡ID
 * @param smsMsg 发送的短信
 *
 * @return  0 发送成功 其它 发送失败
 */
OPERATE_RET tkl_cellular_sms_send(uint8_t sim_id, TUYA_CELLULAR_SMS_SEND_T* sms_msg);

/**
 * @brief 注册短信接收回调函数
 * @note 该函数需在SIM卡激活前注册，否则可能引起异常或短信丢失。
 *
 * @param callback 短信接收回调函数
 *
 * @return  0 注册成功 其它 注册失败
 */
OPERATE_RET tkl_cellular_sms_recv_cb_register(TUYA_CELLULAR_SMS_CB callback);

/**
 * @brief 设置短信接收时静音
 * @note 默认情况下，收到短信时，会通过扬声器播放提示音。通过该函数，可设置接收
 *       短信时，不播放提示音。
 *
 * @param mute TRUE 静音 FALSE 短信接收时打开提示音
 *
 * @return 0 设置成功 其它 设置失败
 */
OPERATE_RET tkl_cellular_sms_mute(bool mute);

/**
 * 短信内容编码转换
 *
 * When \a from_charset or \a to_charset is unknown or unsupported,
 * return NULL.
 * The returned pointer is allocated inside, caller should free it.
 *
 * At most \a from_size byte count will be processed. When \a from_size
 * is -1, process till null chracter.
 *
 * null character will always be inserted into the output string.
 * Invalid characters are replaced silently.
 *
 * \a to_size can be NULL in case that the output byte count is not needed.
 *
 * @param from          input string
 * @param from_size     input string byte count
 * @param from_chset    input string charset
 * @param to_chset      output string charset
 * @param to_size       output string byte count
 * @return
 *      - NULL: invalid parameters
 *      - output string
 */
void *tkl_cellular_sms_convert_str(const void *from, int from_size,
    TUYA_CELLULAR_SMS_ENCODE_E from_chset, TUYA_CELLULAR_SMS_ENCODE_E to_chset, int *to_size);


/**
 * @brief 设置短信默认接收短信编码
 * @param chset TUYA_CELLULAR_SMS_ENCODE_E类型
 *
 * @return 0 设置成功 其它 设置失败
 */
OPERATE_RET tkl_cellular_sms_set_charactor(TUYA_CELLULAR_SMS_ENCODE_E chset);

#ifdef __cplusplus
}
#endif

#endif
