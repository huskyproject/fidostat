# include Husky-Makefile-Config
include ../huskymak.cfg

OBJS    = fidostat$(_OBJ)

ifeq ($(DEBUG), 1)
  CFLAGS  = $(DEBCFLAGS) $(WARNFLAGS) -I$(INCDIR)
  LFLAGS  = $(DEBLFLAGS)
else
  CFLAGS  = $(OPTCFLAGS) $(WARNFLGAS)-c -I$(INCDIR)
  LFLAGS  = $(OPTLFLAGS)
endif

ifeq ($(SHORTNAME), 1)
  LIBS  = -L$(LIBDIR) -lfidoconf -lhusky -lsmapi
else
  LIBS  = -L$(LIBDIR) -lfidoconfig -lhusky -lsmapi
endif

CDEFS=-D$(OSTYPE) $(ADDCDEFS)

all: $(OBJS) fidostat$(_EXE)

%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $(CDEFS) $(SRC_DIR)$*.c

fidostat: $(OBJS)
	$(CC) $(LFLAGS) -o fidostat$(_EXE) fidostat$(_OBJ) $(LIBS)

clean:
	-$(RM) $(RMOPT) *$(_OBJ)
	-$(RM) $(RMOPT) *~
	-$(RM) $(RMOPT) core
	-$(RM) $(RMOPT) *.log

distclean: clean
	-$(RM) $(RMOPT) fidostat$(_EXE)
	-$(RM) $(RMOPT) *.[0-8].gz

install:
	$(INSTALL) $(IBOPT) fidostat$(_EXE) $(BINDIR)

uninstall:
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)fidostat$(_EXE)

