/*
 * Automatically generated by make menuconfig: don't edit
 */

/*
 * Code maturity level options
 */
#undef  CONFIG_EXPERIMENTAL

/*
 * Loadable module support
 */
#define CONFIG_MODULES 1
#undef  CONFIG_MODVERSIONS
#undef  CONFIG_KERNELD

/*
 * General setup
 */
#undef  CONFIG_MATH_EMULATION
#define CONFIG_MEM_STD 1
#undef  CONFIG_MEM_ENT
#undef  CONFIG_MEM_SPECIAL
#define CONFIG_MAX_MEMSIZE 1024
#define CONFIG_NET 1
#undef  CONFIG_MAX_16M
#define CONFIG_PCI 1
#define CONFIG_SYSVIPC 1
#define CONFIG_BINFMT_AOUT 1
#define CONFIG_BINFMT_ELF 1
#define CONFIG_KERNEL_ELF 1
#undef  CONFIG_M386
#undef  CONFIG_M486
#define CONFIG_M586 1
#undef  CONFIG_M686
#undef  CONFIG_MTRR
#undef  CONFIG_APM

/*
 * Floppy, IDE, and other block devices
 */
#define CONFIG_BLK_DEV_FD 1
#define CONFIG_BLK_DEV_IDE 1
#undef  CONFIG_BLK_DEV_HD_IDE
#define CONFIG_BLK_DEV_IDECD 1
#undef  CONFIG_BLK_DEV_IDETAPE
#undef  CONFIG_BLK_DEV_IDEFLOPPY
#undef  CONFIG_BLK_DEV_IDESCSI
#undef  CONFIG_BLK_DEV_IDE_PCMCIA
#define CONFIG_BLK_DEV_CMD640 1
#undef  CONFIG_BLK_DEV_CMD640_ENHANCED
#define CONFIG_BLK_DEV_RZ1000 1
#define CONFIG_BLK_DEV_TRITON 1
#undef  CONFIG_BLK_DEV_OFFBOARD
#undef  CONFIG_IDE_CHIPSETS
#undef  CONFIG_BLK_DEV_LOOP
#define CONFIG_BLK_DEV_MD 1
#define CONFIG_MD_LINEAR 1
#define CONFIG_MD_STRIPED 1
#define CONFIG_MD_MIRRORING 1
#define CONFIG_MD_RAID5 1
#undef  CONFIG_BLK_DEV_RAM
#undef  CONFIG_BLK_DEV_XD
#undef  CONFIG_BLK_DEV_DAC960
#undef  CONFIG_BLK_CPQ_DA
#undef  CONFIG_PARIDE
#undef  CONFIG_BLK_DEV_HD

/*
 * Networking options
 */
#undef  CONFIG_FIREWALL
#undef  CONFIG_NET_ALIAS
#define CONFIG_INET 1
#undef  CONFIG_IP_FORWARD
#undef  CONFIG_IP_MULTICAST
#undef  CONFIG_SYN_COOKIES
#undef  CONFIG_IP_ACCT
#undef  CONFIG_IP_ROUTER
#undef  CONFIG_NET_IPIP
#undef  CONFIG_INET_PCTCP
#undef  CONFIG_INET_RARP
#undef  CONFIG_NO_PATH_MTU_DISCOVERY
#define CONFIG_IP_NOSR 1
#define CONFIG_SKB_LARGE 1
#undef  CONFIG_IPX
#undef  CONFIG_ATALK
#undef  CONFIG_AX25
#undef  CONFIG_NETLINK

/*
 * SCSI support
 */
#undef  CONFIG_SCSI

/*
 * Network device support
 */
#define CONFIG_NETDEVICES 1
#undef  CONFIG_DUMMY
#define CONFIG_DUMMY_MODULE 1
#undef  CONFIG_EQUALIZER
#undef  CONFIG_PLIP
#undef  CONFIG_PPP
#undef  CONFIG_SLIP
#undef  CONFIG_NET_RADIO
#define CONFIG_NET_ETHERNET 1
#define CONFIG_NET_VENDOR_3COM 1
#undef  CONFIG_EL1
#undef  CONFIG_EL2
#define CONFIG_EL3 1
#undef  CONFIG_3C515
#undef  CONFIG_VORTEX
#undef  CONFIG_NET_VENDOR_SMC
#undef  CONFIG_NET_PCI
#undef  CONFIG_NET_ISA
#undef  CONFIG_NET_EISA
#undef  CONFIG_NET_POCKET
#undef  CONFIG_TR
#undef  CONFIG_FDDI
#undef  CONFIG_ARCNET

/*
 * ISDN subsystem
 */
#undef  CONFIG_ISDN

/*
 * CD-ROM drivers (not for SCSI or IDE/ATAPI drives)
 */
#undef  CONFIG_CD_NO_IDESCSI

/*
 * Filesystems
 */
#undef  CONFIG_QUOTA
#define CONFIG_MINIX_FS 1
#undef  CONFIG_EXT_FS
#define CONFIG_EXT2_FS 1
#undef  CONFIG_XIA_FS
#define CONFIG_NLS 1
#define CONFIG_ISO9660_FS 1
#define CONFIG_FAT_FS 1
#define CONFIG_MSDOS_FS 1
#undef  CONFIG_UMSDOS_FS
#undef  CONFIG_VFAT_FS

/*
 * Select available code pages
 */
#undef  CONFIG_NLS_CODEPAGE_437
#undef  CONFIG_NLS_CODEPAGE_737
#undef  CONFIG_NLS_CODEPAGE_775
#undef  CONFIG_NLS_CODEPAGE_850
#undef  CONFIG_NLS_CODEPAGE_852
#undef  CONFIG_NLS_CODEPAGE_855
#undef  CONFIG_NLS_CODEPAGE_857
#undef  CONFIG_NLS_CODEPAGE_860
#undef  CONFIG_NLS_CODEPAGE_861
#undef  CONFIG_NLS_CODEPAGE_862
#undef  CONFIG_NLS_CODEPAGE_863
#undef  CONFIG_NLS_CODEPAGE_864
#undef  CONFIG_NLS_CODEPAGE_865
#undef  CONFIG_NLS_CODEPAGE_866
#undef  CONFIG_NLS_CODEPAGE_869
#undef  CONFIG_NLS_CODEPAGE_874
#undef  CONFIG_NLS_ISO8859_1
#undef  CONFIG_NLS_ISO8859_2
#undef  CONFIG_NLS_ISO8859_3
#undef  CONFIG_NLS_ISO8859_4
#undef  CONFIG_NLS_ISO8859_5
#undef  CONFIG_NLS_ISO8859_6
#undef  CONFIG_NLS_ISO8859_7
#undef  CONFIG_NLS_ISO8859_8
#undef  CONFIG_NLS_ISO8859_9
#undef  CONFIG_NLS_ISO8859_14
#undef  CONFIG_NLS_ISO8859_15
#undef  CONFIG_NLS_KOI8_R
#define CONFIG_PROC_FS 1
#undef  CONFIG_UNSAFE_MMAP
#define CONFIG_NFS_FS 1
#undef  CONFIG_ROOT_NFS
#undef  CONFIG_SMB_FS
#undef  CONFIG_HPFS_FS
#undef  CONFIG_SYSV_FS
#undef  CONFIG_UFS_FS

/*
 * Character devices
 */
#define CONFIG_SERIAL 1
#undef  CONFIG_DIGI
#undef  CONFIG_CYCLADES
#undef  CONFIG_STALDRV
#undef  CONFIG_RISCOM8
#undef  CONFIG_PRINTER
#undef  CONFIG_SPECIALIX
#undef  CONFIG_MOUSE
#undef  CONFIG_UMISC
#undef  CONFIG_QIC02_TAPE
#undef  CONFIG_FTAPE
#undef  CONFIG_WATCHDOG
#undef  CONFIG_RTC

/*
 * Sound
 */
#undef  CONFIG_SOUND

/*
 * Kernel hacking
 */
#undef  CONFIG_PROFILE
#undef  CONFIG_SADISTIC_KMALLOC
#undef  CONFIG_SKB_CHECK
