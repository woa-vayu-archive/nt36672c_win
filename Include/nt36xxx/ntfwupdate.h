#pragma once

#include <wdm.h>
#include <wdf.h>
#include <controller.h>

#define SPI_WRITE_MASK(a)	(a | 0x80)
#define SPI_READ_MASK(a)	(a & 0x7F)

#define SWRST_N8_ADDR 0x03F0FE		//novatek,swrst-n8-addr
#define SPI_RD_FAST_ADDR 0x03F310	//novatek,spi-rd-fast-addr

/* Global pages */
#define NT36XXX_PAGE_CHIP_INFO	0x3F004
#define NT36XXX_PAGE_CRC	    0x3F135

#define NVT_TRANSFER_LEN (63 * 1024)

#define FWBUFFER_SIZE 0x22000

enum nt36xxx_cmds {
    NT36XXX_CMD_ENTER_SLEEP = 0x11,
    NT36XXX_CMD_ENTER_WKUP_GESTURE = 0x13,
    NT36XXX_CMD_UNLOCK = 0x35,
    NT36XXX_CMD_BOOTLOADER_RESET = 0x69,
    NT36XXX_CMD_SW_RESET = 0xaa,
    NT36XXX_CMD_SET_PAGE = 0xff,
};

enum nt36xxx_spi_events {
    NT36XXX_EVT_REPORT = 0x00,
    NT36XXX_EVT_CRC = 0x35,
    NT36XXX_EVT_CHIPID = 0x7f,
    NT36XXX_EVT_HOST_CMD = 0x50,
    NT36XXX_EVT_HS_OR_SUBCMD = 0x51,   /* Handshake or subcommand byte */
    NT36XXX_EVT_RESET_COMPLETE = 0x60,
    NT36XXX_EVT_FWINFO = 0x78,
    NT36XXX_EVT_PROJECTID = 0x9a,
};

NTSTATUS
NVTLoadFirmwareFile(
    WDFDEVICE FxDevice,
    SPB_CONTEXT* SpbContext
);

int nt36xxx_bootloader_reset(IN SPB_CONTEXT* SpbContext);

int nt36xxx_sw_reset_idle(IN SPB_CONTEXT* SpbContext);

int nt36xxx_set_page(IN SPB_CONTEXT* SpbContext, IN unsigned int pageaddr);
