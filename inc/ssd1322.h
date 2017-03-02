/*
 * ssd1322.h
 *
 *  Created on: May 20, 2016
 *      Author: shapa
 */

#ifndef SSD1322_H_
#define SSD1322_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	SSD1322_ENABLE_GRAYSCALE_TABLE = 0,
	SSD1322_SET_COLUMN_START_END = 0x15, // Followed by start, end e [0; 119]
	SSD1322_WRITE_RAM = 0x5C,
	SSD1322_READ_RAM = 0x5D,
	SSD1322_SET_ROW_START_END = 0x75, // Followed by start, end e [0; 127]
	SSD1322_SET_REMAP_COM_LINE_MODE = 0xA0, // Followed by A, B - pls, refer to .pdf
	SSD1322_SET_DISPLAY_START_LINE = 0xA1, // Followed by x e [0; 127]
	SSD1322_SET_DISPLAY_OFFSET = 0xA2, // Followed by x e [0; 127]
	SSD1322_SET_DISPLAY_OFF = 0xA4, // Apply GS0 to all
	SSD1322_SET_DISPLAY_ON_TEST = 0xA5, // Apply GS15 to all
	SSD1322_SET_DISPLAY_ON_NORMAL = 0xA6,
	SSD1322_SET_DISPLAY_ON_NEGATIVE = 0xA7, // GS0 -> GS15, GS15 -> GS0
	SSD1322_ENABLE_PARTIAL = 0xA8, // Followed by Start, End. Start < end e [0; 127]
	SSD1322_DISBLE_PARTIAL = 0xA9,
	SSD1322_SELECT_VDD = 0xAB,  // Followed by x. 0 - External Vdd. 1 - Internal Vdd
	SSD1322_SET_SLEEP_ON = 0xAE,  // Display Off
	SSD1322_SET_SLEEP_OFF = 0xAF,  // Display On

	SSD1322_SET_PHASE_LENGTH = 0xB1,  // Followed by A - pls, refer to .pdf
	SSD1322_SET_CLOCK_DIVIDER = 0xB3,  // Followed by A. Div = 2^A. A <=10
	SSD1322_SET_ENHANCEMENT_A = 0xB4,  // Followed by A, B
	SSD1322_SET_GPIO = 0xB5,  // Followed by A - pls, refer to .pdf
	SSD1322_SET_PRECHARGE_PERIOD_2 = 0xB6,  // Followed by A clkc. A e [0;15]
	SSD1322_SET_GRAY_SCALE_TABLE = 0xB8,  // Followed by A[16]. Max 180.
										  // A[0] = GS0, ... A[15] = GS15. 0 <= GS1 < GS2 < ... < GS15
	SSD1322_SET_DEFAULT_GRAY_SCALE_TABLE = 0xB9,
	SSD1322_SET_PRECHARGE_VOLTAGE = 0xBB, // Followed by A. Max 31. Voltage = (0.20 + 0.0125*A)*Vcc
	SSD1322_SET_VCOMM_VOLTAGE = 0xBE, // Followed by A. Max 7. Voltage = (0.72 + 0.02*A)*Vcc
	SSD1322_SET_CONTRAST_CURRENT = 0xC1, // Followed by A. [0;255]
	SSD1322_SET_MASTER_CONTRAST_CURRENT = 0xC7, // Followed by A. [0;15]. OutCurrent = Cur*(A+1)/16
	SSD1322_SET_MUX_RATIO = 0xCA, // Followed by A. [15;127]. Mux = A+1
	SSD1322_SET_COMMAND_LOCK = 0xFD, // Followed by A. 0x12 - Unlock. 0x16 - Lock

} SSD1322_command_t;

void SSD1322_InitDisplay(void);
void SSD1322_SetCmdLock(_Bool state);
void SSD1322_SetColumnRange(uint8_t start, uint8_t end);
void SSD1322_SetRowRange(uint8_t start, uint8_t end);
void SSD1322_ClearDisplay(void);
void SSD1322_DrawSurface(const uint8_t *line, const uint16_t height, const uint8_t bytesPerLine);

#ifdef __cplusplus
}
#endif

#endif /* SSD1322_H_ */
