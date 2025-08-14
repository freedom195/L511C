/****************************************************************************
 *
 ****************************************************************************/
#include "string.h"
#include "osasys.h"
#include "cmsis_os2.h"
#include "ping.h"
#include "ol_nw_api.h"
#include "ol_log.h"

void ping_callback(uint32_t recv_byte,uint32_t time_ms,uint32_t ttl)
{
    OL_LOG_PRINTF("PING SUCCESS,recv_byte = %d,time = %d ms,ttl = %d",recv_byte,time_ms,ttl);
}

void ping_demo(void)
{
    int count = 5;
    int chkstatus = 0;

    ol_ping_register_callback(ping_callback);
    ping_url_init((const CHAR*)"www.baidu.com", count, 0, 0, FALSE, 0x0001, 1);

    chkstatus = (int)PingChkStatus();
    OL_LOG_PRINTF("ping test status = %d!", chkstatus);
}
