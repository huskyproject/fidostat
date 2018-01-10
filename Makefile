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

all: fidostat$(_EXE) fidostat.1.gz

%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $(CDEFS) $(SRC_DIR)$*.c

fidostat$(_EXE): $(OBJS)
	$(CC) $(LFLAGS) -o fidostat$(_EXE) fidostat$(_OBJ) $(LIBS)

fidostat.1.gz:	man/fidostat.1
	gzip -c $^ >$@

clean:
	-$(RM) $(RMOPT) *$(_OBJ)
	-$(RM) $(RMOPT) fidostat.1.gz

distclean: clean
	-$(RM) $(RMOPT) fidostat$(_EXE)

install:
	$(INSTALL) $(IBOPT) fidostat$(_EXE) $(BINDIR)
	-$(MKDIR) $(MKDIROPT) $(DESTDIR)$(MANDIR)
	-$(MKDIR) $(MKDIROPT) $(DESTDIR)$(MANDIR)/man1
	$(INSTALL) $(IMOPT) fidostat.1.gz $(DESTDIR)$(MANDIR)/man1

uninstall:
	-$(RM) $(RMOPT) $(BINDIR)$(DIRSEP)fidostat$(_EXE)

