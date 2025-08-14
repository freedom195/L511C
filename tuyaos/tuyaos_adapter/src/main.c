#include "tkl_cellular.h"
extern void tuya_app_main(void);

int main(void)
{
    tkl_cellular_init();
    tkl_cellular_mds_init(0);
    tuya_app_main();
    return 0;
}

