PROJECT = dash
MCU = STM32F413ZHTx

GFXLIB = ../../lib/uGFX
GFXCONF = ./ugfx_conf

include $(GFXLIB)/gfx.mk

include $(GFXCONF)/SSD1322/driver.mk

BOARD_INC_DIRS = inc/periph inc/fonts inc/bitmaps $(GFXCONF) $(GFXINC) ../../lib/uGFX/src/gdisp inc/widgets inc/pages inc/modes
BOARD_SRC_DIRS = src/periph src/fonts src/bitmaps src/widgets src/pages src/modes
WNO_ERROR_BOARD_SRC_FILES = $(GFXSRC)

# This will run board.mk.
include ../../build/board.mk
