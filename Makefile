# include Husky-Makefile-Config
include ../huskymak.cfg

OBJS    = fidostat$(OBJ)

ifeq ($(DEBUG), 1)
  CFLAGS  = $(DEBCFLAGS) $(WARNFLAGS) -I$(INCDIR)
  LFLAGS  = $(DEBLFLAGS)
else
  CFLAGS  = $(OPTCFLAGS) $(WARNFLGAS)-c -I$(INCDIR)
  LFLAGS  = $(OPTLFLAGS)
endif

ifeq ($(SHORTNAME), 1)
  LIBS  = -L$(LIBDIR) -lfidoconf -lsmapi
else
  LIBS  = -L$(LIBDIR) -lfidoconfig -lsmapi
endif

CDEFS=-D$(OSTYPE) $(ADDCDEFS)

all: $(OBJS) fidostat$(EXE)

%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $(CDEFS) $(SRC_DIR)$*.c

fidostat: $(OBJS)
	$(CC) $(LFLAGS) -o fidostat$(EXE) fidostat$(OBJ) $(LIBS)

clean:
	-$(RM) *$(OBJ)
	-$(RM) *~
	-$(RM) core
	-$(RM) *.log

distclean: clean
	-$(RM) fidostat$(EXE)
	-$(RM) *.[0-8].gz

install:
	$(INSTALL) $(IBOPT) fidostat$(EXE) $(BINDIR)

uninstall:
	-$(RM) $(BINDIR)$(DIRSEP)fidostat$(EXE)

