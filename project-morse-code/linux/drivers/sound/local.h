/* Computer generated file. Please don't edit! */

#define KERNEL_COMPATIBLE_CONFIG

#define SELECTED_SOUND_OPTIONS	0x00000000

#if \
  defined(CONFIG_PSS) || defined(CONFIG_SSCAPE) || \
  defined(CONFIG_CS4232) || defined(CONFIG_MAUI)
#	define CONFIG_MPU_EMU
#endif

#if \
  defined(CONFIG_PSS) || defined(CONFIG_GUS16) || \
  defined(CONFIG_GUSMAX) || defined(CONFIG_MSS) || \
  defined(CONFIG_SSCAPE) || defined(CONFIG_TRIX) || \
  defined(CONFIG_MAD16) || defined(CONFIG_CS4232)
#	define CONFIG_AD1848
#endif

#if \
  defined(CONFIG_SB) || defined(CONFIG_TRIX) || \
  defined(CONFIG_MAD16)
#	define CONFIG_SBDSP
#endif

#if \
  defined(CONFIG_SB) || defined(CONFIG_TRIX) || \
  defined(CONFIG_MAD16)
#	define CONFIG_UART401
#endif

#if \
  defined(CONFIG_PAS) || defined(CONFIG_SB) || \
  defined(CONFIG_ADLIB) || defined(CONFIG_GUS) || \
  defined(CONFIG_MPU401) || defined(CONFIG_PSS) || \
  defined(CONFIG_SSCAPE) || defined(CONFIG_TRIX) || \
  defined(CONFIG_MAD16) || defined(CONFIG_CS4232) || \
  defined(CONFIG_MAUI)
#	define CONFIG_SEQUENCER
#endif

#define SOUND_CONFIG_DATE "Thu Oct 22 16:19:39 CEST 2020"
#define SOUND_CONFIG_BY "root"
#define SOUND_UNAME_A "Linux localhost.localdomain 2.0.40 #1 Sun Oct 2 19:02:13 CEST 2016 i686 unknown"
