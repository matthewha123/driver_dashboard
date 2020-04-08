/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

#ifndef _GDISP_LLD_BOARD_H
#define _GDISP_LLD_BOARD_H

#include "Dot_Matrix_NHD_US25664.h"

bool debug_pixel_print = false;

//Optional
//#define SSD1322_USE_DMA

#ifndef SSD1322_USE_DMA
	#define SSD1322_USE_DMA			FALSE
#endif

static GFXINLINE void init_board(GDisplay *g) {
	UNUSED(g);

	Pin_Write(OLED_CS_PORT, PIN_OLED_CS, 0);
	Pin_Write(OLED_GPIO_PORT, PIN_OLED_RS, 0);
	Pin_Write(OLED_GPIO_PORT, PIN_OLED_RW, 0);
	Pin_Write(OLED_GPIO_PORT, PIN_OLED_E, 0);
	Pin_Write(OLED_GPIO_PORT, PIN_OLED_RS2, 1);
}

static GFXINLINE void post_init_board(GDisplay *g) {
	UNUSED(g);
}

static GFXINLINE void setpin_reset(GDisplay *g, bool_t state) {
	UNUSED(g);
	UNUSED(state);
}

static GFXINLINE void acquire_bus(GDisplay *g) {
	UNUSED(g);
	Pin_Write(OLED_CS_PORT, PIN_OLED_CS, 0);
}

static GFXINLINE void release_bus(GDisplay *g) {
	UNUSED(g);
	Pin_Write(OLED_CS_PORT, PIN_OLED_CS, 1);
}

static GFXINLINE void write_cmd(GDisplay *g, uint8_t cmd) {
	UNUSED(g);
	oled_send_cmd(&ugfx_oled, cmd);
}

static GFXINLINE void write_data(GDisplay *g, uint8_t data) {
	UNUSED(g);
	oled_send_data(&ugfx_oled, data);
}

#if SSD1322_USE_DMA
	static GFXINLINE void write_data_DMA(GDisplay *g, uint8_t* data) {
		UNUSED(g);
		UNUSED(g);
	}
#endif	// Use DMA

#endif /* _GDISP_LLD_BOARD_H */
