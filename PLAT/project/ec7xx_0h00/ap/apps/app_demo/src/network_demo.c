/****************************************************************************
 *
 ****************************************************************************/
#include "stdio.h"
#include "string.h"
#include "osasys.h"
#include "cmsis_os2.h"
#include "ps_lib_api.h"
#include "ol_open_api.h"
#include "ol_nw_api.h"
#include "ol_log.h"


#define TEST_CID_INDEX_1 OL_CID_INDEX_1
#define TEST_CID_INDEX_2 OL_CID_INDEX_2


void get_iccid(void)
{
    int ret = 0;
    char iccid[32] = {0};

    memset(iccid, 0x0, sizeof(iccid));
    ret = appGetIccidNumSync(iccid);
    OL_LOG_INFO("appGetIccidNumSync ret = %d, iccid = %s", ret, iccid);
}

void get_imei(void)
{
    int ret = 0;
    char imei[32] = {0};

    memset(imei, 0x0, sizeof(imei));
    ret = appGetImeiNumSync(imei);
    OL_LOG_INFO("appGetImeiNumSync ret = %d, imei = %s", ret, imei);
}

void get_imsi(void)
{
    int ret = 0;
    char imsi[32] = {0};

    memset(imsi, 0x0, sizeof(imsi));
    ret = appGetImsiNumSync(imsi);
    OL_LOG_INFO("appGetImsiNumSync ret = %d, imsi = %s", ret, imsi);
}

void get_cereg(void)
{
    int ret = 0;
    CeregGetStateParams param_info;

    ret = appGetCeregStateSync(&param_info);
    OL_LOG_INFO("appGetCeregStateSync ret = %d, rereg_state = %d", ret, param_info.state);
}

void get_cnum(void)
{
    int i = 0;
    int ret = 0;
    ol_cnum_info cnum;

    memset(&cnum, 0x0, sizeof(cnum));
    ret = ol_get_cnum(&cnum);
    OL_LOG_INFO("ol_get_cnum ret = %d, total = %d", ret, cnum.totalnum);

    for(i = 0; i < cnum.totalnum; i++)
    {
        OL_LOG_INFO("cnum[%d]: type = %d, alphaid = %s, dialnum = %s",
                                            i, cnum.numlist[i].type,
                                            cnum.numlist[i].alphaid,
                                            cnum.numlist[i].dialnum);
    }
}

void get_cellinfo(void)
{
    int i = 0;
    int ret = 0;
    BasicCellListInfo cellinfo;

    memset(&cellinfo, 0x0, sizeof(cellinfo));
    ret = ol_get_cellinfo(&cellinfo);
    OL_LOG_INFO("ol_get_cellinfo ret = %d", ret);

    OL_LOG_INFO("---server cell---");
    OL_LOG_INFO("mcc=%d, mnc=%d, cellid=%d, tac=%d, phyid=%d", \
                            cellinfo.sCellInfo.plmn.mcc, \
                            cellinfo.sCellInfo.plmn.mncWithAddInfo, \
                            cellinfo.sCellInfo.cellId, \
                            cellinfo.sCellInfo.tac, \
                            cellinfo.sCellInfo.phyCellId);

    OL_LOG_INFO("---neighbour cell [%d]---",cellinfo.nCellNum);
    for(i = 0; i < cellinfo.nCellNum; i++)
    {
        OL_LOG_INFO("mcc=%d, mnc=%d, cellid=%d, tac=%d, phyid=%d", \
                                cellinfo.nCellList[i].plmn.mcc, \
                                cellinfo.nCellList[i].plmn.mncWithAddInfo, \
                                cellinfo.nCellList[i].cellId, \
                                cellinfo.nCellList[i].tac, \
                                cellinfo.nCellList[i].phyCellId);
    }
}

void set_apn(void)
{
    int ret = 0;
    ret = ol_cid_set(TEST_CID_INDEX_1,OL_IPTYPE_V4V6,"cmnet_1");
    OL_LOG_INFO("ol_cid_set ret = %d",ret);
    ret = ol_cid_set(TEST_CID_INDEX_2,OL_IPTYPE_V4V6,"cmnet_2");
    OL_LOG_INFO("ol_cid_set ret = %d",ret);

}

void get_apn(void)
{
    int ret = 0;
    ol_iptype_enum iptype;
    char apn[16];
    memset(apn,0x0,sizeof(apn));

    ret = ol_cid_get(TEST_CID_INDEX_1,&iptype,apn);
    OL_LOG_INFO("ol_cid_get ret = %d, iptype = %d, apn = %s", ret, iptype, apn);
}

void set_auth(void)
{
    int ret = 0;
    ret = ol_auth_set(TEST_CID_INDEX_1,OL_AUTH_PAP_CHAP,"usr1","123456");
    OL_LOG_INFO("ol_auth_set ret = %d",ret);
}

void get_auth(void)
{
    int ret = 0;
    ol_auth_enum auth_type;
    char usr[64];
    char pwd[64];

    memset(usr,0x0,sizeof(usr));
    memset(pwd,0x0,sizeof(pwd));

    ret = ol_auth_get(TEST_CID_INDEX_1,&auth_type,usr,pwd);
    OL_LOG_INFO("ol_auth_get ret = %d, auth_type=%d, usr=%s, pwd=%s",ret, auth_type, usr, pwd);
}

void active_cid(void)
{
    int ret = 0;

    ret = ol_set_cid_status(TEST_CID_INDEX_1,OL_CID_ACTIVE_REQ);
    OL_LOG_INFO("active ol_set_cid_status ret = %d",ret);
    osDelay(5000);
    ret = ol_set_cid_status(TEST_CID_INDEX_2,OL_CID_ACTIVE_REQ);
    OL_LOG_INFO("active ol_set_cid_status ret = %d",ret);
}

void get_cid_act(void)
{
    int ret = 0;
    ol_cid_active_req_enum act;

    ret = ol_get_cid_status(TEST_CID_INDEX_1,&act);
    OL_LOG_INFO("ol_get_cid_status ret = %d, act_status = %d", ret, act);
}

void get_cid_info(void)
{
    int ret = 0;
    ol_cid_info_struct info;

    memset(&info, 0x0, sizeof(info));
    ret = ol_get_cid_info(TEST_CID_INDEX_1, &info);
    OL_LOG_INFO("ol_get_cid_info ret = %d, iptype=%d, ipv4=%s, ipv6=%s", ret, info.iptype, info.v4_addr, info.v6_addr);
}

void deactive_cid(void)
{
    int ret = 0;
    ret = ol_set_cid_status(TEST_CID_INDEX_2,OL_CID_DEACTIVE_REQ);
    OL_LOG_INFO("deactive ol_set_cid_status = %d",ret);
}

void dns_set_test(void)
{
    ol_set_dns(OL_IPTYPE_V4,"114.114.114.114","8.8.8.8");
    ol_set_dns(OL_IPTYPE_V6,"240E:56:4000:8000:0:0:0:69","240E:56:4000:0:0:0:0:218");
}

void dns_get_test(void)
{
    char pri[128] = {0};
    char sec[128] = {0};

    ol_get_dns(OL_IPTYPE_V4,pri,sec);
    OL_LOG_INFO("ol_get_dns IPV4 pri=%s, sec=%s",pri, sec);

    memset(pri,0,sizeof(pri));
    memset(sec,0,sizeof(pri));
    ol_get_dns(OL_IPTYPE_V6,pri,sec);
    OL_LOG_INFO("ol_get_dns IPV6 pri=%s, sec=%s",pri, sec);
}

void wifiscan_demo(void)
{
    SetWifiScanParams wifiscanreq = {0};
    GetWifiScanInfo *pWifiScanInfo = PNULL;
    int i = 0;
    char pRspStr[128] = {0};

    wifiscanreq.maxTimeOut      = 12000;
    wifiscanreq.round           = 1;
    wifiscanreq.maxBssidNum     = 5;
    wifiscanreq.scanTimeOut     = 5;
    wifiscanreq.wifiPriority    = 0;
    wifiscanreq.channelRecLen   = 280;
    wifiscanreq.channelCount    = 1;

    pWifiScanInfo = (GetWifiScanInfo *)OsaAllocZeroMemory(sizeof(GetWifiScanInfo));
    if(pWifiScanInfo == PNULL)
    {
        OL_LOG_INFO("alloc pWifiScanInfo == NULL,ERROR");
        return;
    }

    appGetWifiScanInfo(&wifiscanreq, pWifiScanInfo);
    OL_LOG_INFO("Get Wifi Scan bssidNum: %d",pWifiScanInfo->bssidNum);

    for (i = 0; i < pWifiScanInfo->bssidNum; i++)
    {
        memset(pRspStr, 0, 128);
        snprintf(pRspStr, 128, "WIFISCAN:(-,-,%d,\"%02X:%02X:%02X:%02X:%02X:%02X\",%d)",
             pWifiScanInfo->rssi[i],
             pWifiScanInfo->bssid[i][0],
             pWifiScanInfo->bssid[i][1],
             pWifiScanInfo->bssid[i][2],
             pWifiScanInfo->bssid[i][3],
             pWifiScanInfo->bssid[i][4],
             pWifiScanInfo->bssid[i][5],
             pWifiScanInfo->channel[i]);
        OL_LOG_INFO("%s",pRspStr);
    }

    OsaFreeMemory(&pWifiScanInfo);
    osDelay(2000/portTICK_PERIOD_MS);
}

void rrc_release_demo()
{
    int ret = -1;
    ret = ol_rrc_release();
    OL_LOG_INFO("rrc release ret = %d",ret);

    ret = ol_tau_update(OL_PERIODIC_TA_UPDATING);
    OL_LOG_INFO("tau update ret = %d",ret);
}

void network_demo(void)
{
    OL_LOG_INFO("--- test network_demo, begin ---");

    //Set SIM Notify
    //SIM ready or removed will report by AppPSUrcCallback (in app.c)
    appSetSIMHotSwapNotify(TRUE);
    osDelay(3000);

    get_imei();
    get_iccid();
    get_imsi();
    get_cereg();
    get_cnum();
    get_cellinfo();

    set_apn();
    get_apn();
    set_auth();
    get_auth();
    active_cid();
    get_cid_act();
    get_cid_info();
    deactive_cid();

    dns_get_test();
    dns_set_test();
    dns_get_test();

    OL_LOG_INFO("--- test network_demo, end ---");
}

