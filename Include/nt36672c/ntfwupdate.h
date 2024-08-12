#pragma once

#include <wdm.h>
#include <wdf.h>
#include <controller.h>

/*
* Rob Green, a member of the NTDEV list, provides the
* following set of macros that'll keep you from having
* to scratch your head and count zeros ever again.
* Using these defintions, all you'll have to do is write:
*
* interval.QuadPart = RELATIVE(SECONDS(5));
*/

#ifndef ABSOLUTE
#define ABSOLUTE(wait) (wait)
#endif

#ifndef RELATIVE
#define RELATIVE(wait) (-(wait))
#endif

#ifndef NANOSECONDS
#define NANOSECONDS(nanos) \
	(((signed __int64)(nanos)) / 100L)
#endif

#ifndef MICROSECONDS
#define MICROSECONDS(micros) \
	(((signed __int64)(micros)) * NANOSECONDS(1000L))
#endif

#ifndef MILLISECONDS
#define MILLISECONDS(milli) \
	(((signed __int64)(milli)) * MICROSECONDS(1000L))
#endif

#ifndef SECONDS
#define SECONDS(seconds) \
	(((signed __int64)(seconds)) * MILLISECONDS(1000L))
#endif

#define SPI_WRITE_MASK(a)	(a | 0x80)
#define SPI_READ_MASK(a)	(a & 0x7F)

#define ENG_RST_ADDR 0x7FFF80
#define SWRST_N8_ADDR 0x03F0FE		//novatek,swrst-n8-addr
#define SPI_RD_FAST_ADDR 0x03F310	//novatek,spi-rd-fast-addr

/* Global pages */
#define nt36672c_PAGE_CHIP_INFO	0x3F004
#define nt36672c_PAGE_CRC	    0x3F135

#define NVT_TRANSFER_LEN (63 * 1024)

#define FWBUFFER_SIZE 0x22000

enum nt36672c_cmds {
    nt36672c_CMD_ENTER_SLEEP = 0x11,
    nt36672c_CMD_ENTER_WKUP_GESTURE = 0x13,
    nt36672c_CMD_UNLOCK = 0x35,
    nt36672c_CMD_BOOTLOADER_RESET = 0x69,
    nt36672c_CMD_SW_RESET = 0xaa,
    nt36672c_CMD_SET_PAGE = 0xff,
};

enum nt36672c_spi_events {
    nt36672c_EVT_REPORT = 0x00,
    nt36672c_EVT_CRC = 0x35,
    nt36672c_EVT_CHIPID = 0x7f,
    nt36672c_EVT_HOST_CMD = 0x50,
    nt36672c_EVT_HS_OR_SUBCMD = 0x51,   /* Handshake or subcommand byte */
    nt36672c_EVT_RESET_COMPLETE = 0x60,
    nt36672c_EVT_FWINFO = 0x78,
    nt36672c_EVT_PROJECTID = 0x9a,
};

NTSTATUS
NVTLoadFirmwareFile(
    WDFDEVICE FxDevice,
    SPB_CONTEXT* SpbContext
);

int nt36672c_bootloader_reset(IN SPB_CONTEXT* SpbContext);

int nt36672c_eng_reset(IN SPB_CONTEXT* SpbContext);

int nt36672c_set_page(IN SPB_CONTEXT* SpbContext, IN unsigned int pageaddr);
