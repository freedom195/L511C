/****************************************************************************
 *
 ****************************************************************************/
#include <stdio.h>
#include "osasys.h"
#include "cmsis_os2.h"
#include "ol_log.h"
#include "ol_adc_api.h"

void adc_demo(void)
{
    int index = 0;

    while(index++ < 5)
    {
        OL_LOG_INFO("--------------ADC test-----------------");
        OL_LOG_INFO("read ADC0: %lu (uV)", ol_adc_get_vol(OL_ADC_0));
        OL_LOG_INFO("read ADC1: %lu (uV)", ol_adc_get_vol(OL_ADC_1));
        OL_LOG_INFO("read Vbat: %lu (uV)", ol_adc_get_vol(OL_ADC_VBAT));
        OL_LOG_INFO("read Temp: %ld (Cel)", ol_adc_get_temperature());
        osDelay(3000);
    }
}

