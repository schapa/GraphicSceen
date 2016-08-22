/*
 * ili9341.h
 *
 *  Created on: Aug 23, 2016
 *      Author: shapa
 */

#ifndef ILI9341_H_
#define ILI9341_H_

typedef enum {
	ILI9341_NOP = 0x00,
	ILI9341_SOFT_RESET = 0x01,
	ILI9341_READ_ID = 0x04, // read 4 bytes, mask 0xFF FFFF
	ILI9341_READ_STATUS = 0x09, // read 5 bytes, mask 0xFF FFFF
	ILI9341_READ_POWER_MODE = 0x0A, // read 2 bytes. mask 0xFF
	ILI9341_READ_MADCTL = 0x0B, // read 2 bytes. mask 0xFF
	ILI9341_READ_PIXEL_FORMAT = 0x0C, // read 2 bytes. mask 0xFF
	ILI9341_READ_IMAGE_FORMAT = 0x0D, // read 2 bytes. mask 0xFF
	ILI9341_READ_SIGNAL_MODE = 0x0E, // read 2 bytes. mask 0xFF
	ILI9341_READ_SELF_DIAG_SIG = 0x0F, // read 2 bytes. mask 0xFF

	ILI9341_ENTER_SLEEP_MODE = 0x10,
	ILI9341_SLEEP_OUT = 0x11,
	ILI9341_PARTIAL_MODE_ON = 0x12,
	ILI9341_NORMAL_MODE_ON = 0x13,

	ILI9341_DISPLAY_INVERSION_OFF = 0x20,
	ILI9341_DISPLAY_INVERSION_ON = 0x21,

	ILI9341_SET_GAMMA = 0x26, // Followed by 1 byte

	ILI9341_DISPLAY_OFF = 0x28,
	ILI9341_DISPLAY_ON = 0x29,

	ILI9341_SET_COLUMN_ADDRESS = 0x2A, // Followed by 4 bytes. SC[15:0] EC[15:0]
	ILI9341_SET_PAGE_ADDRESS = 0x2B, // Followed by 4 bytes. SP[15:0] EP[15:0]
	ILI9341_MEMORY_WRITE = 0x2C, // ON D[17:0] lines
	ILI9341_SET_COLOR = 0x2D, // Followed by R32 G65 B32
	ILI9341_MEMORY_READ = 0x2E, // ON D[17:0] lines

	ILI9341_PARTIAL_AREA = 0x30, // Followed by 4 bytes. SR[15:0] ER[15:0]
	ILI9341_VERTICAL_SCROLL = 0x33, // Followed by 6 bytes. TFA[15:0] VSA[15:0] BFA[15:0]
	ILI9341_TEARING_OFF = 0x34,
	ILI9341_TEARING_ON = 0x35, // Followed by 1 byte. LSB - M
	ILI9341_MEMORY_ACCESS_CTRL = 0x36, // Followed by 1 byte. LSB - M
	ILI9341_VERTICAL_SCROLL_START_ADR = 0x37, // Followed by 2 bytes. VSP[15:0]

	ILI9341_IDLE_OFF = 0x38,
	ILI9341_IDLE_ON = 0x39,
	ILI9341_SET_PIXEL_FORMAT = 0x3A, // Followed by 1 byte. DPI[2:0]<<4 | DBI[2:0]
	ILI9341_MEMORY_WRITE_CONTINUE = 0x3C, // ON D[17:0] lines
	ILI9341_MEMORY_READ_CONTINUE = 0x3D, // Ignore first byte. ON D[17:0] lines

	ILI9341_SET_TEAR_SCANLINE = 0x44,  // Followed by 2 bytes. STS[8] STS[7:0]
	ILI9341_GET_SCANLINE = 0x45,  // Followed by 3 bytes. Ignore First. GTS[9:8] GTS[7:0]

	ILI9341_SET_BRIGHTNESS = 0x51,  // Followed by 1 byte.
	ILI9341_GET_BRIGHTNESS = 0x52,  // read 2 bytes. mask 0xFF
	ILI9341_SET_WRT_CTRL = 0x53,  // Followed by 1 byte
	ILI9341_GET_CTRL = 0x54,  // read 2 bytes. mask 0xFF
	ILI9341_SET_ADAPTIVE_BRIGHTNESS = 0x55,  // Followed by 1 byte
	ILI9341_GET_ADAPTIVE_BRIGHTNESS = 0x56,  // read 2 bytes. mask 0xFF
	ILI9341_SET_ADAPTIVE_BRIGHTNESS_MIN = 0x5E,  // Followed by 1 byte
	ILI9341_GET_ADAPTIVE_BRIGHTNESS_MIN = 0x5F,  // read 2 bytes. mask 0xFF

	ILI9341_GET_ID1 = 0xDA,  // read 2 bytes. mask 0xFF
	ILI9341_GET_ID2 = 0xDB,  // read 2 bytes. mask 0xFF
	ILI9341_GET_ID3 = 0xDC,  // read 2 bytes. mask 0xFF

	ILI9341_SET_RGB_SIG_CTRL = 0xB0,  // Followed by 1 byte
	ILI9341_SET_FRAME_CTRL_NORMAL = 0xB1,  // Followed by 2 bytes. DIVA[1:0] RTNA[4:0]
	ILI9341_SET_FRAME_CTRL_IDLE = 0xB2,  // Followed by 2 bytes. DIVB[1:0] RTNB[4:0]
	ILI9341_SET_FRAME_CTRL_PARTIAL = 0xB3,  // Followed by 2 bytes. DIVC[1:0] RTNC[4:0]

	ILI9341_SET_INVERSION = 0xB4,  // Followed by 2 bytes. NLA/NLB/NCL [2:0]
	ILI9341_SET_BLANK_PORCH = 0xB5,  // Followed by 4 bytes. VFP[6:0] VBP[6:0] HFP[6:0] HFP[6:0]

	ILI9341_SET_CTRL = 0xB6,  // Followed by 4 bytes. [PTG[3:2] PT[1:0]] [REV,GS,SS,SM,ISC[3:0]] NL[5:0] PCDIV[5:0]
	ILI9341_SET_ENTRY_MODE = 0xB7,  // Followed by 1 byte. GON DTE GAS [2:0]

	ILI9341_SET_BACKLIGHT_CTRL_1 = 0xB8,  // Followed by 2 bytes.
	ILI9341_SET_BACKLIGHT_CTRL_2 = 0xB9,  // Followed by 2 bytes.
	ILI9341_SET_BACKLIGHT_CTRL_3 = 0xBA,  // Followed by 2 bytes.
	ILI9341_SET_BACKLIGHT_CTRL_4 = 0xBB,  // Followed by 2 bytes.
	ILI9341_SET_BACKLIGHT_CTRL_5 = 0xBC,  // Followed by 2 bytes.
	ILI9341_SET_BACKLIGHT_CTRL_6 = 0xBD,  // Followed by 2 bytes.
	ILI9341_SET_BACKLIGHT_CTRL_7 = 0xBE,  // Followed by 1 byte.
	ILI9341_SET_BACKLIGHT_CTRL_8 = 0xBF,  // Followed by 1 byte.

	ILI9341_SET_POWER_CTRL_1 = 0xC0,  // Followed by 1 byte.
	ILI9341_SET_POWER_CTRL_2 = 0xC1,  // Followed by 1 byte.

	ILI9341_SET_VCOM_CTRL_1 = 0xC5,  // Followed by 1 byte.
	ILI9341_SET_VCOM_CTRL_2 = 0xC7,  // Followed by 1 byte.

	ILI9341_MEMORY_NV_WRITE = 0xD0,  // Followed by 2 bytes. PGM_ADR[2:0] PGM_DATA[7:0]
	ILI9341_MEMORY_NV_KEY = 0xD1,  // Followed by 3 bytes. KEY[23:0]
	ILI9341_MEMORY_GET_STATUS = 0xD2,  // Followed by 3 bytes. ignore first

	ILI9341_GET_ID4 = 0xD3,  // Followed by 4 bytes. ignore first

	/* some missed */

	ILI9341_IFACE_CTRL = 0xF6, // Followed by 3 bytes.

} ILI9341_command_t;




#endif /* ILI9341_H_ */
