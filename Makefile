#testkernel makefile
NAME = bdash

GAME_C_FILES = main.c bdash_song.c
GAME_C_FILES += sdk/lib/blitter/blitter.c sdk/lib/blitter/blitter_tmap.c sdk/lib/blitter/blitter_sprites.c sdk/lib/chiptune/chiptune.c sdk/lib/chiptune/player.c
GAME_BINARY_FILES = bdash.tset bdash.tmap clock.spr diams.spr rock.spr
#$(wildcard snd/*.raw)

include sdk/kernel/bitbox.mk
main.c:bdash.h bdash_song.c

bdash.h bdash.tset bdash.tmap: bdash.tmx bdash.png
	python sdk/lib/blitter/scripts/tmx.py bdash.tmx > bdash.h

bdash_song.c: bdash.chp
	python sdk/lib/chiptune/song2C.py $^ > $@

clock.spr: $(sort $(wildcard clock_xcf-??.png))
	python sdk/lib/blitter/scripts/couples_encode.py $@ $^

diams.spr: $(sort $(wildcard diams_xcf-??.png))
	python sdk/lib/blitter/scripts/couples_encode.py $@ $^

rock.spr: $(sort $(wildcard rock/????.png))
	# RLE?
	bash -c  'for f in rock/????.png ; do convert $$f -ordered-dither o8x8,32,32,32 $${f/.png/_od.png} ;done'
	python sdk/lib/blitter/scripts/couples_encode.py $@ $(^:.png=_od.png)

snd/%.raw: snd/%.wav
	sox $^ -r 30k -c 1 -b 8 --encoding signed-integer $@

clean::
	rm -f bdash.h *.tmap *.tset *.spr *.spr.png