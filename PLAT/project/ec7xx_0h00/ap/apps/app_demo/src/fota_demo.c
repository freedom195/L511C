#include "string.h"
#include "osasys.h"
#include "cmsis_os2.h"
#include "ec_tcpip_api.h"
#include "ol_fota_api.h"
#include "ol_log.h"
#include "ol_uart_api.h"
#include "uart.h"
#include "queue.h"
#include "bsp_custom.h"

#define FOTA_TCP_IP "118.114.239.159"
#define FOTA_TCP_PORT 30165
unsigned int size = 0;
unsigned int offset = 0;
volatile bool local_fota_event_timeout = FALSE;
volatile bool local_fota_event_complete = FALSE;
static osEventFlagsId_t local_fota_evtflag_handle = NULL;

char fota_ca[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIIDKjCCAhICCQCOewfZiRCiNjANBgkqhkiG9w0BAQUFADBXMQswCQYDVQQGEwJD\r\n"
"TjEQMA4GA1UECBMHU2lDaHVhbjEVMBMGA1UEChMMTU9CSUxFVEVLLkNBMQswCQYD\r\n"
"VQQLEwJJVDESMBAGA1UEAxMJTU9CSUxFVEVLMB4XDTE4MDkxODA4MDUzMloXDTMz\r\n"
"MDkxOTA4MDUzMlowVzELMAkGA1UEBhMCQ04xEDAOBgNVBAgTB1NpQ2h1YW4xFTAT\r\n"
"BgNVBAoTDE1PQklMRVRFSy5DQTELMAkGA1UECxMCSVQxEjAQBgNVBAMTCU1PQklM\r\n"
"RVRFSzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOkdYJF1h1xjKbY0\r\n"
"ipbl88G653PiCh8ZMjmIUYeiDKC8+0wtXZtHvQIl6AncOzBy9XHVOctbKn34exC8\r\n"
"SEotMuo2T49vs9VtE8GYu2pOrf3m42NpLRnYAxfm9qw53CMHx+Jn7Oa9fnxa8haA\r\n"
"pRc2BTVadWGoS8EEwoZFk0eNb7Z2Gc7U0c+GhISI4oVTTocGvGgMzkvduu5JJbbc\r\n"
"BOcNFrii9sRO9vtOYQtqOEg01Uum2Dwp/o2bDLXNJEqAIh4WACiM4iPmmlRHWT2y\r\n"
"NjQ3vcbEdrFwbHRtO46+Vw54HnSyCoFb3uCHMNMvXObZ/8AU9E3Cgat4j0sgEeB0\r\n"
"hqA4MiMCAwEAATANBgkqhkiG9w0BAQUFAAOCAQEAtEAjf0CjsLgG9ROdmp1qXYft\r\n"
"+ndIT5l82KRK57ZQsfdFbnJOvALeF/ICKU0M2TXgJNiGOA5RxDi00YYdMbOIPwVZ\r\n"
"JH4b87J/LYdLAGf+Q+kVI6gWH3hPm4Jzfzq/40KVrf3mpa54yWz6ZYtwfxBjrMgr\r\n"
"IVe0O5SIJ99lsddgzgUkqYN2vWJW2zZ50xuXOAyo+pOnjzX0wuOcaBT3JCHWJRAb\r\n"
"VhJCf9JbswDgnddJerqFtB8pnpAOdGokLCOoM06q3s3P9mhGX+72HXdX7G8CSAuG\r\n"
"PVCGf6RaF0/G4B9R1c3du3lZRlQWfx2pxyU0LS86iFQFWqzqcWEXIcULVdcErQ==\r\n"
"-----END CERTIFICATE-----\r\n";

char fota_client_crt[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIIDKTCCAhECCQDpY/MI6IXYcTANBgkqhkiG9w0BAQUFADBXMQswCQYDVQQGEwJD\r\n"
"TjEQMA4GA1UECBMHU2lDaHVhbjEVMBMGA1UEChMMTU9CSUxFVEVLLkNBMQswCQYD\r\n"
"VQQLEwJJVDESMBAGA1UEAxMJTU9CSUxFVEVLMB4XDTE4MDkxODA4MDcxNloXDTMz\r\n"
"MDkxOTA4MDcxNlowVjELMAkGA1UEBhMCQ04xEDAOBgNVBAgTB1NpQ2h1YW4xFDAS\r\n"
"BgNVBAoTC01PQklMRVRFSy5DMQswCQYDVQQLEwJJVDESMBAGA1UEAxMJTU9CSUxF\r\n"
"VEVLMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAk1EdX++SIEgEnJVm\r\n"
"T//7U8xBJBT4JL61A0MAIFSE+xLcFjKQ492ev19Y6XmrcrlrXfvaO+d9cCMvfjxc\r\n"
"xQ+aBOiNs0Sy2jHLdZJfLAKRo8Pt9eM1cX8ALaymigpmgL+7zeXNhW0ejzzCrsay\r\n"
"XLVWKZyOHNyv5kdDfRyfM1kGnPxU9qV6YNgwELz0+Nhf+b4DSNEUHDDT+eu2ZiVs\r\n"
"VoiC7Duwen2794yka1xyPrDQazW4uZMqUOZzZhrBptjjNXQ6PCHqcV5FDzFzXM7h\r\n"
"lklnl4ZOVOCSVoTCNLS3M35N+P70e4uSXM3SMJuMhFURUWrJ+BRFBBa/Oqv97lJZ\r\n"
"DLnnpQIDAQABMA0GCSqGSIb3DQEBBQUAA4IBAQAyUETpOQS6p6RAxDuX8/O7r5M0\r\n"
"ZYtzd+eUyx5tAt7H2YPCXRPBEpEppExayCihoWoEYsItfs8fQ7a6dA5goltK7bXv\r\n"
"XT3uVJnF5aBmVY3fdXiOmH5bTD7SUv7QWmXoA8qIfNcOgPL4wCwYaGdGwfP+x5+W\r\n"
"lHZK79BKli7MiABaMCmn/Ivzi1aq5iLFvCRs054ibuiz/LgVtRG912LHJbmRin0R\r\n"
"jtIzguJ7ViGW2eCFhlnpA8oACh2G7JQp6ifw8Mzi0N7rCVV8BjIc2Dfh0zWjbQ6D\r\n"
"uhiuzXYukHoEM/DRIbn66ffxb/R/tU6u2UewW3+Bv/h1vWzfKJdbqIIgwNbY\r\n"
"-----END CERTIFICATE-----\r\n";

char fota_client_key[] =
"-----BEGIN RSA PRIVATE KEY-----\r\n"
"MIIEpAIBAAKCAQEAk1EdX++SIEgEnJVmT//7U8xBJBT4JL61A0MAIFSE+xLcFjKQ\r\n"
"492ev19Y6XmrcrlrXfvaO+d9cCMvfjxcxQ+aBOiNs0Sy2jHLdZJfLAKRo8Pt9eM1\r\n"
"cX8ALaymigpmgL+7zeXNhW0ejzzCrsayXLVWKZyOHNyv5kdDfRyfM1kGnPxU9qV6\r\n"
"YNgwELz0+Nhf+b4DSNEUHDDT+eu2ZiVsVoiC7Duwen2794yka1xyPrDQazW4uZMq\r\n"
"UOZzZhrBptjjNXQ6PCHqcV5FDzFzXM7hlklnl4ZOVOCSVoTCNLS3M35N+P70e4uS\r\n"
"XM3SMJuMhFURUWrJ+BRFBBa/Oqv97lJZDLnnpQIDAQABAoIBADv70kcm6EwGpwIO\r\n"
"/xrh8zb6uydy9lsX3+KVHG8NmxwUYNMVAdQWgDYSH8TpSrsq7qHVE9ZCetv3J7Zx\r\n"
"46BFqwQGoPP2rBCWJ+8Wi3QDE8Qn2jO9sRwmhy8R1rnsZDJWX6UFxjpm5QOTzP2X\r\n"
"YDbJi2zkFyV/YowURlGiHtysijnPEq0PcYlKrOMp7ee4znCKmcyEWU9EjvkVvBBS\r\n"
"WSgGYsMCePh4AzlfyhoxcO8NLWM7XTl32ov5IjUEqwV0oIVqB5yFFRxPS1gN5nSn\r\n"
"N6uggVrzwLjK4bgTGwbMAHkZhfXPeNvpSVbWLXqsgTYeyqLyikigzZrMF+PmMbKT\r\n"
"hgsOmr0CgYEAxFhLo8zMkoPmKc3Rfv3lpE6AtSIqdQfaJhVxbE+tiRzt4xpbfFtb\r\n"
"LbWGAUwcewesxMWyMFzl2nBixBPkIJkDYMkAo8dR26oL4QorWtb96xLQceDCF3L5\r\n"
"uI6Tm/3tCd1iNCEzg4yqCTtljelkcIyLwOClcscBlqIYe+ac6tj56PcCgYEAwBNr\r\n"
"CI/+s3QUiT7b9Na6WZhCItRmnA8Z3sUCdcDirm48UEwaWLFzuvolpxj9h9YrP/GQ\r\n"
"6ODnqZpaMgeTr0yW8c/LrXkW5o6RmRcnOLTWHHvphlaEgrU33xpmr0XbabOCl+TS\r\n"
"yKyrArYpyIL5wPAvCtOKHzDuQqRk+9lwBp3YyUMCgYEAramjDXghIR5Ev4jp2Tbc\r\n"
"nN1KfeuAPg755mFk9vXqebH6vrobXPy4ws8hfZhCQJdjOo/ZsWkZbIGm+eLWDfcI\r\n"
"w9xFEpdDUdUvuJX0Dt7Fq9vyPicbxP3O3mxNJtYLiIQlThJnq90IsC22/zQFwH6v\r\n"
"RaPs8n4Oa5tnqH8rH5VWRbUCgYAY/+4EdUl/bfJmUqoqWfBzTN/+zIp2cXi6iAXj\r\n"
"8bEPZwWupdkgBii8A2b3Msd88KE8d2KXDP7aEl7++AF+5YcX/iXSdFpIs/G7bUoL\r\n"
"lW3w0yf39jbVFGUrQrJuybbfMSAkSQIuYFr5xV/22yVKuXhF/naRzLqWLfN+3DQ6\r\n"
"iDz5JQKBgQCC+SK63r5sXt8xKdHy7KXNo9WcjBgDumiF3VuaBunqSasQHpYlUBol\r\n"
"M/ZvR2RLOuZWCM6XZcU84bowdSXMfWEwhMPbXvuVgMiYVfVGe9unomtRVOZdUqct\r\n"
"ULwV8cGeNE6fwrLN8hkXTmEjmL3ESNXKjinJp46U0IJ/4iVtdzfiNg==\r\n"
"-----END RSA PRIVATE KEY-----\r\n";


int test_download_status = OL_FOTA_BEGIN;

void fota_callback(ol_fota_status status, int percent)
{
    OL_LOG_PRINTF("fota_cb status = %d,percent = %d", status, percent);
    test_download_status = status;
}

void ftp_fota_demo(void)
{
    int ret = -1;
    mbtk_fota_server_info server_info;

    memcpy(server_info.host,"240e:699:0:7::6",sizeof("240e:699:0:7::6"));
    server_info.is_ipv6 = 1;
    server_info.port = 5021;
    server_info.ssl_type = MBTK_FTP_SSL_TYPE_AUTH;
    memcpy(server_info.username, "ftp123", sizeof("ftp123"));
    memcpy(server_info.password, "ftp123", sizeof("ftp123"));

    memcpy(server_info.path, "/test/default_delta.par",sizeof("/test/default_delta.par"));
    ol_ftp_fota_set_ssl(fota_ca,fota_client_crt,fota_client_key);
    ret = ol_fota_ftp_api(server_info,fota_callback);
    OL_LOG_PRINTF("ret = %d",ret);

    if(0 != ret)
    {
        OL_LOG_PRINTF("ol_fota_ftp_api fail %d", ret);
        return;
    }

    while(1)
    {
        if(OL_FOTA_BEGIN == test_download_status
                || OL_FOTA_DL_PROC == test_download_status)
        {
            osDelay(1000);
            continue;
        }
        if(OL_FOTA_DOWNLOADEND == test_download_status)
        {
            OL_LOG_PRINTF("reboot and start upgrade");
            ol_fota_reboot_upgrade();
            break;
        }
        else
        {
            break;
        }
    }
}

void http_fota_demo(void)
{
    int ret = -1;
    int isHTTPs = 0;
    //download fota package success and reboot upgrade
    OL_LOG_PRINTF("enter http fota_demo");

    if(isHTTPs == 1)
    {
        ol_http_fota_set_ssl(1, fota_ca, fota_client_crt, fota_client_key, 0);
        ret = ol_fota_http_api("https://220.167.54.26:4430/default_delta.par", fota_callback);
    }
    else
    {
        ret = ol_fota_http_api("http://118.114.239.159:30175/default_delta.par", fota_callback);
    }
    //ret = ol_fota_http_api("https://365wifi.oss-cn-zhangjiakou.aliyuncs.com/locator/ota/jg09_to_10_delta.par", fota_callback);
    if(0 != ret)
    {
        OL_LOG_PRINTF("ol_fota_http_api fail %d", ret);
        return;
    }

    while(1)
    {
        if(OL_FOTA_BEGIN == test_download_status
                || OL_FOTA_DL_PROC == test_download_status)
        {
            osDelay(1000);
            continue;
        }
        if(OL_FOTA_DOWNLOADEND == test_download_status)
        {
            OL_LOG_PRINTF("reboot and start upgrade");
            ol_fota_reboot_upgrade();
            break;
        }
        else
        {
            break;
        }
    }
}
void fota_demo(void)
{
    int fota_type = 0;

    OL_LOG_PRINTF("enter fota demo fota_type %d",fota_type);
    if(fota_type == 0)
    {
        http_fota_demo();       //download fota package through HTTP
    }
    else if(fota_type == 1)
    {
        ftp_fota_demo();       //download fota package through FTP
    }
    else if(fota_type == 2)
    { 
#ifdef MBTK_OPENCPU_SUPPORT_SSL
        int ret = ol_fota_mbtk_api("sd","ec_version_1","ec_version_2",fota_callback);
        if(0 != ret)
        {
            OL_LOG_PRINTF("ol_fota_http_api fail %d", ret);
            return;
        }

        while(1)
        {
            if(OL_FOTA_BEGIN == test_download_status
                    || OL_FOTA_DL_PROC == test_download_status)
            {
                osDelay(1000);
                continue;
            }
            if(OL_FOTA_DOWNLOADEND == test_download_status)
            {
                OL_LOG_PRINTF("reboot and start upgrade");
                ol_fota_reboot_upgrade();
                break;
            }
            else
            {
                break;
            }
        }
#endif
    }

}


void fota_tcp_cb(UINT8 connectionEventType, void* bodyEvent)
{
    int ret = 0;
    if(connectionEventType == TCPIP_CONNECTION_STATUS_EVENT)
    {
        TcpipConnectionStatusInd* status = (TcpipConnectionStatusInd*)bodyEvent;
        OL_LOG_PRINTF("id = %d", status->connectionId);
        OL_LOG_PRINTF("status = %d", status->status);
        if(status->status == 0)//disconnect
        {
            //ret = tcpipConnectionClose(status->connectionId);
            if(offset > 0)
            {
                OL_LOG_PRINTF("write over offset=%d", offset);
                ret = ol_fota_nvm_check();
                if(ret == 0)
                {
                    OL_LOG_PRINTF("reboot and start upgrade");
                    ol_fota_reboot_upgrade();
                }
                offset = 0;
            }
        }
    }
    else if(connectionEventType == TCPIP_CONNECTION_RECEIVE_EVENT)
    {
        unsigned char* data = NULL;

        TcpipConnectionRecvDataInd* recv = (TcpipConnectionRecvDataInd*)bodyEvent;
        OL_LOG_PRINTF("id = %d", recv->connectionId);
        OL_LOG_PRINTF("length = %d", recv->length);
        OL_LOG_PRINTF("data = %s", recv->data);
        data = malloc(recv->length + 1);
        memcpy(data, recv->data, recv->length);

        ret = ol_fota_nvm_write(offset, data, recv->length);
        if(ret == 0)
        {
            offset += recv->length;
        }
        if(data)
        {
            free(data);
        }
    }
    else if(connectionEventType == TCPIP_CONNECTION_UL_STATUS_EVENT)
    {
        TcpipConnectionUlDataStatusInd* ul = (TcpipConnectionUlDataStatusInd*)bodyEvent;
        OL_LOG_PRINTF("id = %d", ul->connectionId);
        OL_LOG_PRINTF("status = %d", ul->status);
    }
}

void fota_tcp_demo(void)
{
    int ret = -1;

    ret = ol_fota_nvm_init();
    OL_LOG_PRINTF("ol_fota_nvm_init = %d", ret);

    ret =  tcpipConnectionCreate(TCPIP_CONNECTION_PROTOCOL_TCP, NULL, 0, FOTA_TCP_IP, FOTA_TCP_PORT, fota_tcp_cb);
    OL_LOG_PRINTF("tcpipConnectionCreate = %d", ret);
}

void local_fota_uart_callback(uint32_t event)
{
    OL_LOG_PRINTF("test uart cb event %d", event);

    if(event & ARM_USART_EVENT_RX_TIMEOUT)
    {
        local_fota_event_timeout = TRUE;
        osEventFlagsSet(local_fota_evtflag_handle, 0x00000001);
    }
    if(event & ARM_USART_EVENT_RECEIVE_COMPLETE)
    {
        local_fota_event_complete = TRUE;
        osEventFlagsSet(local_fota_evtflag_handle, 0x00000001);
    }
}

void local_fota_demo(void)
{ 
    ol_uart_config_t config = {0x0};
    uint8_t test_buffer[2048] = {"start send fota package\r\n"};
    int offset = 0;

    config.baudRate = 115200;
    config.cb_event = local_fota_uart_callback;
    config.control = (ARM_USART_MODE_ASYNCHRONOUS |
                      ARM_USART_DATA_BITS_8 |
                      ARM_USART_PARITY_NONE |
                      ARM_USART_STOP_BITS_1 |
                      ARM_USART_FLOW_CONTROL_NONE);

    local_fota_evtflag_handle = osEventFlagsNew(NULL);
    if(NULL == local_fota_evtflag_handle)
    {
      OL_LOG_PRINTF("create event flag handle fail, return");
      return;
    }

    int ret = ol_uart_init(OL_UART_1, &config);
    if(OL_UART_OK != ret)
    {
        OL_LOG_PRINTF("init open UART1 fail = %d, return", ret);
        return;
    }

    OL_LOG_PRINTF("init open OL_UART_1 OK");
    ol_uart_send(OL_UART_1, test_buffer, strlen((char *)test_buffer));

    ret = ol_fota_nvm_init();
    if(ret != 0)
    {
        OL_LOG_PRINTF("init fota nvm error");
        return;
    }

    while(1)
    {
        int length = 0;
        memset(test_buffer, 0, sizeof(test_buffer));
        ret = ol_uart_recv_async(OL_UART_1, test_buffer, sizeof(test_buffer));
        OL_LOG_PRINTF("uart async receive ret = %d", ret);

        osEventFlagsWait(local_fota_evtflag_handle, 0x00000001, osFlagsWaitAny, osWaitForever);
        length = ol_uart_rx_count(OL_UART_1);
        OL_LOG_PRINTF("uart async receive data length = %d", length);

        if(0 == strncmp("quit", (const char *)test_buffer, 4))
        {
            break;
        }
        else
        {
            ret = ol_fota_nvm_write(offset, test_buffer, length);
            if(ret == 0)
            {
                offset += length;
            }
        }
        local_fota_event_timeout = FALSE;
        local_fota_event_complete = FALSE;
    }

    if(offset > 0)
    {
        OL_LOG_PRINTF("write over offset=%d", offset);
        ret = ol_fota_nvm_check();
        if(ret == 0)
        {
            OL_LOG_PRINTF("reboot and start upgrade");
            ol_fota_reboot_upgrade();
        }
        else
        {
            OL_LOG_PRINTF("OL_FOTA_PACKAGE_MISMATCH");
        }
        offset = 0;
    }

    ol_uart_uninit(OL_UART_1);
    osEventFlagsDelete(local_fota_evtflag_handle);
}

