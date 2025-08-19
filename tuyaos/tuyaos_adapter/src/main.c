#include "tkl_cellular_base.h"
extern void tuya_app_main(void);

int main(void)
{
    tkl_cellular_base_init(NULL);
    tkl_cellular_mds_init(0);
    tuya_app_main();
    return 0;
}

