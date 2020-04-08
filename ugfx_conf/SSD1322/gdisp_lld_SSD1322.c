/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

#include "gfx.h"
#include "uart.h"

#if GFX_USE_GDISP

#define GDISP_DRIVER_VMT			GDISPVMT_SSD1322
#include "gdisp_lld_config.h"
#include "gdisp_driver.h"

#include "board_SSD1322.h"

#include <string.h>   // for memset

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

// #ifndef GDISP_SCREEN_HEIGHT
	#define GDISP_SCREEN_HEIGHT		64		// This controller should support  64
// #endif
// #ifndef GDISP_SCREEN_WIDTH
	#define GDISP_SCREEN_WIDTH		256
// #endif
#ifndef GDISP_INITIAL_CONTRAST
	#define GDISP_INITIAL_CONTRAST	100
#endif
#ifndef GDISP_INITIAL_BACKLIGHT
	#define GDISP_INITIAL_BACKLIGHT	100
#endif
#ifndef SSD1322_USE_DMA
	#define SSD1322_USE_DMA			FALSE
#endif

#define SSD1322_ROW_WIDTH			(GDISP_SCREEN_WIDTH)

#define GDISP_FLG_NEEDFLUSH			(GDISP_FLG_DRIVER<<0)

#include "SSD1322.h"

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

// Some common routines and macros
#define RAM(g)							((uint8_t *)g->priv)

// Some common routines and macros
#define xyaddr(x, y)		((x) + (y)*SSD1322_ROW_WIDTH)
#define xybits(x, y, c)		((c)<<(((~(x)&1))<<2))

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * As this controller can't update on a pixel boundary we need to maintain the
 * the entire display surface in memory so that we can do the necessary bit
 * operations. Fortunately it is a small display in 4 bit grayscale.
 * 64 * 128 / 2 = 4096 bytes.
 */

LLDSPEC bool_t gdisp_lld_init(GDisplay *g) {
	// The private area is the display surface.
	g->priv = gfxAlloc(GDISP_SCREEN_HEIGHT * SSD1322_ROW_WIDTH);

	// Start the NHD_US25664
	start_display(&ugfx_oled);

	/* Initialise the GDISP structure */
	g->g.Width = GDISP_SCREEN_WIDTH;
	g->g.Height = GDISP_SCREEN_HEIGHT;
	g->g.Orientation = GDISP_ROTATE_0;
	g->g.Powermode = powerOn;
	g->g.Backlight = GDISP_INITIAL_BACKLIGHT;
	g->g.Contrast = GDISP_INITIAL_CONTRAST;

	return TRUE;
}

#if GDISP_HARDWARE_FLUSH
	LLDSPEC void gdisp_lld_flush(GDisplay *g) {
		uint8_t * ram;
		unsigned cols,rows;

		// Don't flush if we don't need it.
		if (!(g->flags & GDISP_FLG_NEEDFLUSH))
			return;

		acquire_bus(g);
		write_cmd(g, CMD_SET_COLUMN_ADDR);					// range 28 to 91  for 256 pixels to x
		write_data(g, START_ADDR);
		write_data(g, MAXCOLS);
		write_cmd(g, CMD_SET_ROW_ADDR);						// range 0 to 63   for 64 pixels
		write_data(g, 0);
		write_data(g, GDISP_SCREEN_HEIGHT-1);
		write_cmd(g, CMD_WRITE_RAM);
		ram = RAM(g);
		#if SSD1322_USE_DMA
			write_data_DMA(g, ram);
		#else
			for(rows = 0; rows < 64; rows ++) {
				for(cols = 0;cols < 64; cols ++) {

					uint8_t col_data[2] = {0,0};
					col_data[0] = *(ram + xyaddr(cols*4, rows)) | *(ram + xyaddr(cols*4 + 1, rows));
					col_data[1] = *(ram + xyaddr(cols*4 + 2, rows)) | *(ram + xyaddr(cols*4 + 3, rows));

					write_data(g, col_data[0]);
					write_data(g, col_data[1]);
				}
			}
			// spi_init(SPI3);
			// spi_transmit(SPI3, ram, 4096, 2000);

		#endif
		release_bus(g);
		g->flags &= ~GDISP_FLG_NEEDFLUSH;
	}
#endif

#if GDISP_HARDWARE_DRAWPIXEL
	LLDSPEC void gdisp_lld_draw_pixel(GDisplay *g) {

		coord_t		x, y;
		uint8_t		*ram;

		switch(g->g.Orientation) {
		default:
		case GDISP_ROTATE_0:
			x = g->p.x;
			y = g->p.y;
			break;
		case GDISP_ROTATE_90:
			x = g->p.y;
			y = GDISP_SCREEN_HEIGHT-1 - g->p.x;
			break;
		case GDISP_ROTATE_180:
			x = GDISP_SCREEN_WIDTH-1 - g->p.x;
			y = GDISP_SCREEN_HEIGHT-1 - g->p.y;
			break;
		case GDISP_ROTATE_270:
			x = GDISP_SCREEN_WIDTH-1 - g->p.y;
			y = g->p.x;
			break;
		}
		ram = RAM(g)+xyaddr(x,y);
		*ram &= ~xybits(x, y, LLDCOLOR_MASK());
		*ram |= xybits(x, y, gdispColor2Native(g->p.color));

		g->flags |= GDISP_FLG_NEEDFLUSH;
	}
#endif

#if GDISP_HARDWARE_PIXELREAD
	LLDSPEC color_t gdisp_lld_get_pixel_color(GDisplay *g) {
		coord_t			x, y;
		LLDCOLOR_TYPE	c;	

		switch(g->g.Orientation) {
		default:
		case GDISP_ROTATE_0:
			x = g->p.x;
			y = g->p.y;
			break;
		case GDISP_ROTATE_90:
			x = g->p.y;
			y = GDISP_SCREEN_HEIGHT-1 - g->p.x;
			break;
		case GDISP_ROTATE_180:
			x = GDISP_SCREEN_WIDTH-1 - g->p.x;
			y = GDISP_SCREEN_HEIGHT-1 - g->p.y;
			break;
		case GDISP_ROTATE_270:
			x = GDISP_SCREEN_WIDTH-1 - g->p.y;
			y = g->p.x;
			break;
		}
		c = (RAM(g)[xyaddr(x, y)]>>((x & 1)<<2)) & LLDCOLOR_MASK();
		return gdispNative2Color(c);
	}
#endif

#if GDISP_NEED_CONTROL && GDISP_HARDWARE_CONTROL
	LLDSPEC void gdisp_lld_control(GDisplay *g) {
		switch(g->p.x) {
		case GDISP_CONTROL_POWER:
			if (g->g.Powermode == (powermode_t)g->p.ptr)
				return;
			switch((powermode_t)g->p.ptr) {
			case powerOff:
			case powerSleep:
			case powerDeepSleep:
				acquire_bus(g);
				write_cmd(g, CMD_SET_DISPLAY_MODE_OFF);
				release_bus(g);
				break;
			case powerOn:
				acquire_bus(g);
				write_cmd(g, CMD_SET_DISPLAY_MODE_ON);
				release_bus(g);
				break;
			default:
				return;
			}
			g->g.Powermode = (powermode_t)g->p.ptr;
			return;

		case GDISP_CONTROL_ORIENTATION:
			if (g->g.Orientation == (orientation_t)g->p.ptr)
				return;
			switch((orientation_t)g->p.ptr) {
			/* Rotation is handled by the drawing routines */
			case GDISP_ROTATE_0:
			case GDISP_ROTATE_180:
				g->g.Height = GDISP_SCREEN_HEIGHT;
				g->g.Width = GDISP_SCREEN_WIDTH;
				break;
			case GDISP_ROTATE_90:
			case GDISP_ROTATE_270:
				g->g.Height = GDISP_SCREEN_WIDTH;
				g->g.Width = GDISP_SCREEN_HEIGHT;
				break;
			default:
				return;
			}
			g->g.Orientation = (orientation_t)g->p.ptr;
			return;
		}
	}
#endif // GDISP_NEED_CONTROL

#endif // GFX_USE_GDISP
