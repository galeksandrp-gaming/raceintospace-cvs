EXTRA_WARNINGS = -Werror -Wextra -Wno-unused-parameter -Wno-sign-compare \
	-Wno-char-subscripts -Wno-unused-variable -Wno-parentheses \
	-Wno-uninitialized -Wno-missing-braces

CFLAGS = -g -Wall -Dfar= -Dpascal= -I. `sdl-config --cflags`

.c.o:
	@echo "$(CC) ...flags... -c $*.c"
	@$(CC) $(CFLAGS) $(EXTRA_WARNINGS) -O -c -o TMP.o $*.c
	@$(CC) $(CFLAGS) $(EXTRA_WARNINGS) -c $*.c

PROGS = baris imgsplit vtest decode getport mkmovie getvab mtest
all: $(PROGS) 

BARIS_HFILES = Buzz_inc.h cdmaster.h cdrom.h data.h externs.h mis.h mtype.h	\
	music.h nn.h pace.h pcx_hdr.h proto.h records.h replay.h soundfx.h	\
	sv_lib.h uc.h soundint.h

BARIS_OBJS = admin.o aimast.o aimis.o aipur.o ast0.o ast1.o	    \
	ast2.o ast3.o ast4.o budget.o crew.o endgame.o	    \
	futbub.o future.o hardef.o intel.o intro.o main.o mc2.o mc.o \
	mis_c.o mis_m.o museum.o name.o newmis.o news.o news_sup.o	    \
	news_suq.o							    \
	place.o port.o prefs.o prest.o radar.o rdplex.o recods.o \
	replay.o review.o rush.o sel.o start.o   \
	vab.o pace.o gx.o gr.o sdl.o
baris: $(BARIS_OBJS)
	$(CC) $(CFLAGS) -o baris $(BARIS_OBJS) `sdl-config --libs` -lm

# $(BARIS_OBJS): $(BARIS_HFILES)


imgsplit: imgsplit.o
	$(CC) $(CFLAGS) -o imgsplit imgsplit.o -lm

vtest: vtest.o
	$(CC) $(CFLAGS) -o vtest vtest.o -lasound -lopus -lm

decode: decode.o
	$(CC) $(CFLAGS) -o decode decode.o -lopus -lm

getport: getport.o
	$(CC) $(CFLAGS) -o getport getport.o -lopus -lm

mkmovie: mkmovie.o
	$(CC) $(CFLAGS) -o mkmovie mkmovie.o -lm

getvab: getvab.o
	$(CC) $(CFLAGS) -o getvab getvab.o -lm

mtest: mtest.o
	$(CC) $(CFLAGS) -o mtest mtest.o -lm

sdl.o: sdl.c
	$(CC) $(CFLAGS) `sdl-config --cflags` -c sdl.c

sdl: sdl.o
	$(CC) $(CFLAGS) -o sdl sdl.o `sdl-config --libs` -lm

clean:
	rm -f *.o *~

tar:
	rm -f baris-pace.tar baris-pace.tar.gz
	tar -cf baris-pace.tar Makefile ChangeLog *.[ch] domount .gdbinit
	gzip baris-pace.tar


