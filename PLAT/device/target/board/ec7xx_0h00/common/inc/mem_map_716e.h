
#ifndef MEM_MAP_716E_H
#define MEM_MAP_716E_H

/* 4MB flash + no psram*/

/*
flash layout, toatl 4MB
flash raw address: 0x00000000---0x00400000
flash xip address(from both ap/cp view): 0x00800000---0x00c00000

0x00000000          |---------------------------------|
                    |      header1 4KB                |
0x00001000          |---------------------------------|
                    |      header2 4KB                |
0x00002000          |---------------------------------|
                    |      fuse mirror 4KB            |
0x00003000          |---------------------------------|
                    |      bl 72KB                    |
0x00015000          |---------------------------------|
                    |      rel data(factory)20KB      |
0x0001a000          |---------------------------------|
                    |      cp img 400KB               |
0x0007e000          |---------------------------------|
                    |      app img 3116KB             |//mbtk -224 = 2892
0x00389000          |---------------------------------|//mbtk 0x00351000
                    |      hib backup 96KB            |
0x003a1000          |---------------------------------|//mbtk 0x00369000
#if defined (FEATURE_FOTA_FS_ENABLE)
                    |      lfs  272KB                 |
0x003e5000          |---------------------------------|
                    |      fota rsvd 48KB             |
#else
                    |      lfs  48KB                  |//mbtk +224 = 272
0x003ad000          |---------------------------------|
                    |      fota 272KB                 |
#endif
0x003f1000          |---------------------------------|
                    |      rel data 52KB              |
0x003fe000          |---------------------------------|
                    |      plat config 8KB            |
0x00400000          |---------------------------------|


*/

#include "pkg_716e_mapdef.h"


/* -------------------------------flash  address define-------------------------*/


#define AP_FLASH_XIP_ADDR               (0x00800000)


//for fuse mirror, due to limitation of fuse read times
#define FUSE_FLASH_MIRROR_XIP_ADDR      (AP_FLASH_XIP_ADDR+0x2000)
#define FUSE_FLASH_MIRROR_ADDR          (0x2000)
#define FUSE_FLASH_MIRROR_SIZE          (0x1000)


//bl addr and size
#define BOOTLOADER_FLASH_LOAD_ADDR              (0x00803000)
#define BOOTLOADER_FLASH_LOAD_SIZE              (0x12000)//72kB, real region size, tool will check when zip TODO:ZIP
#define BOOTLOADER_FLASH_LOAD_UNZIP_SIZE        (0x18000)//96KB ,for ld


#define AP_FLASH_LOAD_ADDR              (0x0087e000)

//#ifdef FEATURE_EXCEPTION_FLASH_DUMP_ENABLE
//#define AP_FLASH_LOAD_SIZE              (0x30b000 - FLASH_EXCEP_DUMP_SIZE)//3116KB - 8KB
//#else
//#define AP_FLASH_LOAD_SIZE              (0x30b000)//3116KB
//#endif
//#define AP_FLASH_LOAD_UNZIP_SIZE        (0x319000)//3172KB ,for ld
/*mbtk*/
#define AP_FLASH_LOAD_SIZE              (0x2d3000)//2892KB
#define AP_FLASH_LOAD_UNZIP_SIZE        (0x2e1000)//2948KB ,for ld

//hib bakcup addr and size
//#define FLASH_HIB_BACKUP_EXIST          (1)
//#define FLASH_MEM_BACKUP_ADDR           (AP_FLASH_XIP_ADDR+FLASH_MEM_BACKUP_NONXIP_ADDR)
//#define FLASH_MEM_BACKUP_NONXIP_ADDR    (0x389000)
//#define FLASH_MEM_BACKUP_SIZE           (0x18000)//96KB
//#define FLASH_MEM_BLOCK_SIZE            (0x6000)
//#define FLASH_MEM_BLOCK_CNT             (0x4)
/*mbtk*/
#define FLASH_HIB_BACKUP_EXIST          (1)
#define FLASH_MEM_BACKUP_ADDR           (AP_FLASH_XIP_ADDR+FLASH_MEM_BACKUP_NONXIP_ADDR)
#define FLASH_MEM_BACKUP_NONXIP_ADDR    (0x351000)
#define FLASH_MEM_BACKUP_SIZE           (0x18000)//96KB
#define FLASH_MEM_BLOCK_SIZE            (0x6000)
#define FLASH_MEM_BLOCK_CNT             (0x4)

//fs addr and size
//#define FLASH_FS_REGION_START           (0x3a1000)
//#define FLASH_FS_REGION_SIZE            (FLASH_FS_REGION_END-FLASH_FS_REGION_START) // 48KB
/*mbtk*/
#define FLASH_FS_REGION_START           (0x369000)
#define FLASH_FS_REGION_SIZE            (FLASH_FS_REGION_END-FLASH_FS_REGION_START) // 272KB
#if defined (FEATURE_FOTA_FS_ENABLE)
#define FLASH_FS_REGION_END             (0x3e5000)  // 272KB

//fota addr and size
#define FLASH_FOTA_REGION_START         (0x3e5000)
#define FLASH_FOTA_REGION_LEN           (0xc000)//48KB
#else
#define FLASH_FS_REGION_END             (0x3ad000)//mbtk 272KB

//fota addr and size
#define FLASH_FOTA_REGION_START         (0x3ad000)
#define FLASH_FOTA_REGION_LEN           (0x44000)//272KB
#endif
#define FLASH_FOTA_REGION_END           (0x3f1000)

//ap reliable addr and size
#define NVRAM_FACTORY_PHYSICAL_BASE     (0x15000)
#define NVRAM_FACTORY_PHYSICAL_SIZE     (0x5000)//20KB
#define NVRAM_PHYSICAL_BASE             (0x3f1000)
#define NVRAM_PHYSICAL_SIZE             (0xD000)//52KB

//plat config addr and size
#define FLASH_MEM_PLAT_INFO_ADDR        (AP_FLASH_XIP_ADDR+FLASH_MEM_PLAT_INFO_NONXIP_ADDR)
#define FLASH_MEM_PLAT_INFO_NONXIP_ADDR (0x3fe000)
#define FLASH_MEM_PLAT_INFO_SIZE        (0x1000)//4KB

#define CP_FLASH_XIP_ADDR               (0x00800000)

//cp img
#define CP_FLASH_LOAD_ADDR              (0x0081a000)
//cp img
#define CP_FLASH_LOAD_SIZE              (0x64000)//400KB,real region size, tool will check when zip
#define CP_FLASH_LOAD_UNZIP_SIZE        (0x80000)//512KB, for ld


//cp reliable addr and size, cp nvm write by ap
#define CP_NVRAM_FACTORY_PHYSICAL_BASE  (NVRAM_FACTORY_PHYSICAL_BASE + 0x1000)
#define CP_NVRAM_FACTORY_PHYSICAL_SIZE  (0x4000)//16KB
#define CP_NVRAM_PHYSICAL_BASE          (NVRAM_PHYSICAL_BASE + 0x1000)
#define CP_NVRAM_PHYSICAL_SIZE          (0xc000)//48KB


//add for img merge tool,should fix as AP_xx/CP_xx/BL_xx, tool will extract img type from it
//#define AP_IMG_MERGE_ADDR               (0x0007c000)
//#define CP_IMG_MERGE_ADDR               (0x00018000)
//#define BL_IMG_MERGE_ADDR               (0x00003000)

// Flash Dump Macros
#define FLASH_EXCEP_DUMP_ADDR            (FLASH_FOTA_REGION_END - FLASH_EXCEP_DUMP_SIZE)
#define FLASH_EXCEP_DUMP_SIZE            0x4000
#define FLASH_EXCEP_DUMP_SECTOR_NUM      0x4
#define FLASH_EXCEP_KEY_INFO_ADDR        0x0
#define FLASH_EXCEP_KEY_INFO_LEN         0x0

#if ((FLASH_EXCEP_DUMP_ADDR)>=(FLASH_FOTA_REGION_START)) && ((FLASH_EXCEP_DUMP_ADDR)<(FLASH_FOTA_REGION_END-0xB000))
#error "Vaild excep dump area in FOTA region is [FLASH_FOTA_REGION_END-0xB000,FLASH_FOTA_REGION_END]."
#endif

#ifdef FEATURE_EXCEPTION_FLASH_DUMP_ENABLE
/*
 *	BaseAddress is FLASH_EXCEP_DUMP_ADDR and the dump space is FLASH_EXCEP_DUMP_SIZE.
 *	offset 0                   offset 0x1000	          offset 0x3000
 *	|--------------------------|--------------------------|--------------------------|
 *	| 1K PLAT + 1K PHY + 2K PS |         8K UNILOG        |         4K CUST          |
 *	|--------------------------|--------------------------|--------------------------|
 */
#define FLASH_EXCEP_DUMP_ADDR_OFFSET_PLAT    0x0000
#define FLASH_EXCEP_DUMP_ADDR_OFFSET_PHY     0x0400
#define FLASH_EXCEP_DUMP_ADDR_OFFSET_PS      0x0800
#define FLASH_EXCEP_DUMP_ADDR_OFFSET_UNILOG  0x1000
#define FLASH_EXCEP_DUMP_ADDR_OFFSET_CUST    0x3000
#define FLASH_EXCEP_DUMP_SPACE_LIMIT_PLAT    (FLASH_EXCEP_DUMP_ADDR_OFFSET_PHY - FLASH_EXCEP_DUMP_ADDR_OFFSET_PLAT)
#define FLASH_EXCEP_DUMP_SPACE_LIMIT_PHY     (FLASH_EXCEP_DUMP_ADDR_OFFSET_PS - FLASH_EXCEP_DUMP_ADDR_OFFSET_PHY)
#define FLASH_EXCEP_DUMP_SPACE_LIMIT_PS      (FLASH_EXCEP_DUMP_ADDR_OFFSET_UNILOG - FLASH_EXCEP_DUMP_ADDR_OFFSET_PS)
#define FLASH_EXCEP_DUMP_SPACE_LIMIT_UNILOG  (FLASH_EXCEP_DUMP_ADDR_OFFSET_CUST - FLASH_EXCEP_DUMP_ADDR_OFFSET_UNILOG)
#define FLASH_EXCEP_DUMP_CUST_SPACE_LIMIT    (0x1000) /* Customer can change this macro depend on its real needs */
#endif


//for secure boot
#define BLS_SEC_HAED_ADDR               (0x0)
#define BLS_FLASH_LOAD_SIZE             (0x1000)
#define SYS_SEC_HAED_ADDR               (0x1000)
#define SYS_FLASH_LOAD_SIZE             (0x1000)

/* -----------ram address define, TODO: need modify according to ram lauout-------------*/

//csmb start
#define CSMB_START_ADDR                 (0x0)
#define CSMB_END_ADDR                   (0x10000)
#define CSMB_TOTAL_LENGTH               (CSMB_END_ADDR-CSMB_START_ADDR)
#define CSMB_RAMCODE_START_ADDR         (0x20)
#define CSMB_PHY_AONMEM_ADDR            (0xF800)


//csmb end


//msmb start

/*
0x00400000          |---------------------------------|
                    |      LOAD_AP_FIRAM_MSMB         |
                    |---------------------------------|
                    |      LOAD_APOS                  |
                    |---------------------------------|
                    |      LOAD_DRAM_BSP              |
                    |---------------------------------|
                    |      UNLOAD_SLPMEM              |
                    |---------------------------------|
                    |      LOAD_DRAM_SHARED           |
0x004CA000          |---------------------------------|   <---MSMB_APMEM_END_ADDR
                    |      LOAD_CP_FIRAM_MSMB         |
                    |---------------------------------|
                    |      LOAD_CPOS_IRAM             |
                    |---------------------------------|
                    |      UNLOAD_SLPMEM              |
                    |---------------------------------|
                    |      LOAD_CPDRAM_BSP            |
                    |---------------------------------|
                    |      LOAD_CPDRAM_SHARED         |
                    |---------------------------------|
                    |      RESERVED                   |
0x004FE000          |---------------------------------|   <---XP_SHAREINFO_BASE_ADDR
                    |      LOAD_XP_SHAREDINFO         |
0x004FF000          |---------------------------------|   <---IPC_SHAREDMEM_START_ADDR
                    |      LOAD_XP_IPCMEM             |
0x00500000          |                                 |   <---MSMB_END_ADDR

*/

#define MSMB_START_ADDR                 (0x00400000)
#define MSMB_END_ADDR                   (0x00500000)
#define MSMB_TOTAL_LENGTH               (MSMB_END_ADDR-MSMB_START_ADDR)
#ifdef FEATURE_MORERAM_ENABLE
#define MSMB_APMEM_END_ADDR             (0x004DD000)
#define MSMB_CPMEM_START_ADDR           (0x004DD000)
#else
#define MSMB_APMEM_END_ADDR             (0x004CA000)
#define MSMB_CPMEM_START_ADDR           (0x004CA000)
#endif
#define MSMB_CPMEM_LENGTH               (MSMB_END_ADDR-MSMB_CPMEM_START_ADDR)
#define MSMB_CPDATA_START_ADDR          (0x004ED000)
#define XP_SHAREINFO_BASE_ADDR          (0x004FE000)
#define XP_DBGRESERVED_BASE_ADDR        (0x004FEF00)
#define IPC_SHAREDMEM_START_ADDR        (0x004FF000)
#define EXCEPTION_INFO_MEM_END_ADDR     (MSMB_END_ADDR)

//msmb end


//asmb start
/*
0x00000000          |---------------------------------|
                    |      bootcode                   |
0x00001000          |---------------------------------|
                    |      LOAD_AP_PIRAM_ASMB         |
                    |---------------------------------|
                    |      LOAD_AP_FIRAM_ASMB         |
0x0000A800          |---------------------------------|   <---AMSB_CPAON_START_ADDR
                    |      UNLOAD_CPAON               |
0x0000B000          |---------------------------------|
                    |      LOAD_RRCMEM                |
0x0000C000          |---------------------------------|
                    |      LOAD_FLASHMEM              |
0x00010000          |---------------------------------|
*/
#define ASMB_START_ADDR                 (0x00000000)
#define ASMB_END_ADDR                   (0x00010000)
#define ASMB_TOTAL_LENGTH               (ASMB_END_ADDR-ASMB_START_ADDR)
#define ASMB_IRAM_START_ADDR            (0x00001000)
#define ASMB_RRCMEM_START_ADDR          (0x0000B000)
#define ASMB_FLASHMEM_START_ADDR        (0x0000C000)
#define ASMB_START_ADDR_FOR_CP          (ASMB_START_ADDR+0x200000)
#define CP_AONMEMBACKUP_START_ADDR      (0x0000A800)
#define ASMB_UNREMAP_START_ADDR         (0x00100000)
#define AMSB_CPAON_START_ADDR_FOR_CP    (CP_AONMEMBACKUP_START_ADDR+0x200000)

//asmb end


// ps ram
#define PSRAM_EXIST                     (0)
#define PSRAM_START_ADDR                (0x10000000)
#define PSRAM_END_ADDR                  (0x10200000)
#define PSRAM_TOTAL_LENGTH              (PSRAM_END_ADDR-PSRAM_START_ADDR)


#ifdef OPEN_CPU_MODE
#if FEATURE_IMS_ENABLE
#define min_heap_size_threshold 0x3E800//isms heap(+ 150KB) will also use heap
#else
#define min_heap_size_threshold 0x19000
#endif
#ifdef FEATURE_MORERAM_ENABLE
#define up_buf_start 0x4A0000   // should be 4 byte align
#define UP_BUF_MAX_SIZE 0x3CA00//only upbuf size, need another 512B for other buf also in this region
#else
#define up_buf_start 0x48D000   // should be 4 byte align
#define UP_BUF_MAX_SIZE 0x3CA00//only upbuf size, need another 512B for other buf also in this region
#endif
#else       //716e always enable OPEN_CPU_MODE
#define min_heap_size_threshold 0x20000
#define up_buf_start 0x462C00   // should be 4 byte align
#define UP_BUF_MAX_SIZE 0x66D00//only upbuf size, need another 512B for other buf also in this region
#endif

// no space for excption dump

// AP Section define
#define SECTIONAP_LOAD_BOOTCODE           1
#define SECTIONAP_LOAD_AP_PIRAM_ASMB      2
#define SECTIONAP_LOAD_AP_FIRAM_ASMB      3
#define SECTIONAP_LOAD_AP_RWDATA_ASMB     4
#define SECTIONAP_LOAD_PS_RWDATA_ASMB     5
#define SECTIONAP_LOAD_AP_PIRAM_MSMB      6
#define SECTIONAP_LOAD_AP_FIRAM_MSMB      7
#define SECTIONAP_LOAD_APOS               8
#define SECTIONAP_LOAD_DRAM_BSP           9
#define SECTIONAP_LOAD_DRAM_SHARED        10
#define SECTIONAP_LOAD_AP_PS_FIRAM_MSMB   11

#define SECTIONAP_HASH_CHECK_MAX_INDEX    SECTIONAP_LOAD_AP_PS_FIRAM_MSMB


// CP Section define
#define SECTIONCP_LOAD_CP_PIRAM_CSMB            1
#define SECTIONCP_LOAD_CP_FIRAM_CSMB            2
#define SECTIONCP_LOAD_CP_SLP2PIRAM_MSMB        3
#define SECTIONCP_LOAD_CP_PIRAM_MSMB            4
#define SECTIONCP_LOAD_CP_FIRAM_MSMB_PART2      5
#define SECTIONCP_LOAD_CP_FIRAM_MSMB_PART1      6
#define SECTIONCP_LOAD_CPOS                     7
#define SECTIONCP_LOAD_CPDRAM_BSP_DATA          8
#define SECTIONCP_LOAD_CPDRAM_SHARED_DATA       9

#define SECTIONCP_HASH_CHECK_MAX_INDEX    SECTIONCP_LOAD_CPOS


// BL Section define
#define SECTIONBL_LOAD_AIRAM_SHARED           1
#define SECTIONBL_LOAD_AIRAM_OTHER_RAMCODE    2
#define SECTIONBL_LOAD_CIRAM_RAMCODE          3


//add for ram version to seperate grp0 size in json

#define AP_GRP0_SIZE_IN_JSON               (24576)
#define AP_GRP1_SIZE_IN_JSON               (98304)
#define AP_UNZIP_GRP_MAX_SIZE              (176128)


#ifdef FEATURE_MORERAM_ENABLE
#define CP_GRP0_SIZE_IN_JSON               (57344)//less ramcode
#else
#define CP_GRP0_SIZE_IN_JSON               (90112)
#endif
#define CP_GRP1_SIZE_IN_JSON               (98304)
#define CP_UNZIP_GRP_MAX_SIZE              (176128)


#include "pkg_716e_mapchk.h"

//mbtk add for ex_flash
/* external storage device */
//e.g. external flash data addr and size
#define EF_FLASH_XIP_ADDR               (0x80000000)
#define EF_DATA_LOAD_ADDR               (0x00000000)
#define EF_DATA_LOAD_SIZE               (0x100000)//1MB

#endif

