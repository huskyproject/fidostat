DEBUG   = 1
CC      = gcc 
INSTDIR = /usr/local/bin
MANDIR  = /usr/local/man

OBJS    = fidostat.o

ifeq ($(DEBUG), 1)
  CFLAGS  = -c -I../smapi -I../fidoconfig -Wall -ggdb -O3 -s -DUNIX
  LFLAGS  = 
else
  CFLAGS  = -c -I../smapi -I../fidoconfig -Wall -O3 -s -DUNIX
  LFLAGS  =  
endif

all: $(OBJS) \
     fidostat

%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $(SRC_DIR)$*.c

fidostat: fidostat.o $(OBJS)
	$(CC) $(LFLAGS) -o fidostat fidostat.o -lfidoconfig -lsmapilnx

clean:
	-rm -f *.o
	-rm *~
	-rm core

distclean: clean
	-rm tpkt pktinfo hpt purgeDupes tlog
	-rm *.1.gz
	-rm *.log

install:
	install -s fidostat $(INSTDIR)
