/****************************************************************************
 *
 ****************************************************************************/
#include "string.h"
#include "osasys.h"
#include "cmsis_os2.h"
#include "ol_log.h"
#include "ol_lbs_api.h"

void lbs_demo(void)
{
    int ret = 0;
    char lbs_info[128];
    char key[30] = {"0"};//input the Gaode key requested by the customer

    //test 1
    memset(lbs_info, 0, sizeof(lbs_info));
    ret = ol_get_lbs_info("121.40.65.12", 18571, lbs_info, 15);
    OL_LOG_INFO("lbs test: ret = %d, info = %s",ret, lbs_info);

    //test 2
    memset(lbs_info, 0, sizeof(lbs_info));
    ret = ol_lbs_gdhttp_info(key, lbs_info);
    OL_LOG_INFO("gd lbs test: ret = %d, info = %s",ret, lbs_info);
}

