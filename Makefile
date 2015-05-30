#testkernel makefile
NAME = bdash

GAME_C_FILES = main.c 
GAME_BINARY_FILES = bdash.tset bdash.tmap clock.spr diams.spr rock.spr $(wildcard snd/*.raw) 

USE_ENGINE=1
USE_SAMPLER=1

include $(BITBOX)/lib/bitbox.mk
main.c:bdash.h

bdash.h bdash.tset bdash.tmap: bdash.tmx bdash.png 
	python $(BITBOX)/scripts/tmx.py bdash.tmx > bdash.h

clock.spr: $(wildcard clock_xcf-??.png)
	python $(BITBOX)/scripts/couples_encode.py $@ $^ 

diams.spr: $(wildcard diams_xcf-??.png)
	python $(BITBOX)/scripts/couples_encode.py $@ $^ 

rock.spr: $(wildcard rock/????.png)
	# RLE?
	bash -c  'for f in rock/????.png ; do convert $$f -ordered-dither o8x8,32,32,32 $${f/.png/_od.png} ;done'
	python $(BITBOX)/scripts/couples_encode.py $@ $(^:.png=_od.png) 

snd/%.raw: snd/%.wav	
	sox $^ -r 30k -c 1 -b 8 --encoding signed-integer $@

clean::
	rm -f bdash.h *.tmap *.tset *.spr *.spr.png