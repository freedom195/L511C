#ifndef __PKG_716E_MAPDEF_H__
#define __PKG_716E_MAPDEF_H__

#define AP_FLASH_BASE_LNA 0x800000



//For 716E, CP image in AP Flash
#define CP_PKGIMG_LNA (0x0081a000)

#define BL_PKGIMG_LNA (0x00803000)

#define BOOTLOADER_PKGIMG_LIMIT_SIZE (0x12000)

#define AP_PKGIMG_LNA (0x0087e000)

//#ifdef FEATURE_EXCEPTION_FLASH_DUMP_ENABLE
//#define AP_PKGIMG_LIMIT_SIZE (0x30b000 - FLASH_EXCEP_DUMP_SIZE)
//#else
//#define AP_PKGIMG_LIMIT_SIZE (0x30b000)
//#endif
/*mbtk*/
#define AP_PKGIMG_LIMIT_SIZE (0x2d3000)

#define CP_PKGIMG_LIMIT_SIZE (0x64000)


#endif

