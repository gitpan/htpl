# Generated automatically from Makefile.in by configure.
# Makefile for HTPL

# Autoconf output

PERL = /usr/bin/perl
PERL_BIN = /usr/bin/perl
TEMP = /tmp
CCPROG = gcc
INSBIN = /usr/bin/install -c
INSTALLDIR = /usr/local/apache/cgi-bin
HTDOCS = /usr/local/apache/htdocs/htpl
HTLOGS = /usr/local/apache/logs/htpl_log
CONF = /usr/local/apache/conf/httpd.conf
OBJEMBED = 
EGO = htpl-author.ego
XML = 1
DEPDBDEF = 
INSTALLBIN = /usr/local/bin
MODDIR = 

# Defines

BIN = htpl.cgi
DBG = htpldbg
RUN = htplrun
COMP = htplp
CMP = htplc
SRVC = htplsrvc
GEN = htpgen
SH = /bin/sh
CONDS = -DTMP_DIR=\"$(TEMP)\" -DPERL_BIN=\"$(PERL_BIN)\" $(DEFEMBED)
INFO = -DCOMPILATION=`$(PERL) -e "print time;"`  -DBUILD=`cat build` 
CCFLAGS = -O3 $(LIBS) $(CONDS) $(INFO)
CC = $(CCPROG) $(CCFLAGS)
CS = htpl.c htpl-l.c htpl-sub.c htpl-parse.c $(OBJEMBED)
HS = htpl.h htpl-sh.h
OBV = Makefile
DEP = $(CS) $(HS) $(OBV) $(OBJEMBED)
REP = htreporter
SCRIPTS = $(RUN) $(CMP) $(COMP) $(REP) $(SRVC) $(GEN)
TARGETS = $(BIN) $(DBG) $(PMS) $(SCRIPTS)
CWD = `pwd`
DEFEMBED = `test -z "$(OBJEMBED)" || echo -D__PERLEMBED__`
PL = `test -z "$(OBJEMBED)" || $(PERL) -MExtUtils::Embed -e ccopts -e ldopts`
LIBS = `test -z "$(DEPDBDEF)" || echo -ldb` `test -z "$(OBJEMBED)" || echo -lperl`
MODPERL = 
MODULES = HTPL-modules HTPL-SQL Hebrew Calendar $(MODPERL)

# Rules

all : $(TARGETS) $(EGO)

clean : clean-modules
	/bin/rm -f $(TARGETS) $(SCRIPTS) || true
	test -n "$(XML)" && /bin/rm -f htpl-sh.h htpl-parse.c htplparse.pm perl.sh htpl-config.pl || true
	/bin/rm -f Makefile core || true
	/bin/rm -f config.status config.cache || true

realclean : clean
	/bin/rm -f core *.perl *.ht.pl perl.sh || true

release: 
	$(PERL) -MHTML::HTPL::Lib -e 'increasefile("release.#")'
	perl -np -e 's/^(.)/$$1./; s/:.*$$//;' release.# > release.v
	echo -n "htpl-"`cat release.v` > release.name

dist :
	$(PERL) -e "print time;" > build
	mkdir `cat release.name`
	cp -r `cat MANIFEST` `cat release.name`
	tar zcvf `cat release.name`.tar.gz `cat release.name`
	/bin/rm -rf `cat release.name`

$(BIN) : $(DEP)
	$(CC) -o $(BIN) $(CS) $(PL)

$(DBG) : $(DEP)
	$(CC) -D__DEBUG__ -o $(DBG) $(CS) $(PL) 

# Cancel this dependency if you fail to compile with no XML
htpl-parse.c : macros.xpl htpl-crp.pl htplparse.pm
	test -n "$(XML)" && $(PERL) htpl-crp.pl

htplparse.pm: htplparse.yp
	test -n "$(XML)" && yapp htplparse.yp

htpl-sh.h : htpl-parse.c

htpl-xsi.c : 
	$(PERL) -MExtUtils::Embed -e xsinit -- -o htpl-xsi.c

$(RUN) : $(OBV)
	echo "#!/bin/sh" > $(RUN)
	echo "$(INSTALLDIR)/$(DBG) -rwb$(INSTALLDIR) \$$*" >> $(RUN)
	chmod 755 $(RUN)

$(CMP) : $(OBV)
	echo "#!/bin/sh" > $(CMP)
	echo "$(INSTALLDIR)/$(DBG) -twb$(INSTALLDIR) \$$1" >> $(CMP)
	chmod 755 $(CMP)

install-it : 
	for f in $(DBG) $(BIN) ; do \
	$(INSBIN) -m 755 $$f $(INSTALLDIR) ; \
	done
	for f in htpl.head htpl.offhead htpl-pm.code htpl-xsub.pl ; do \
	$(INSBIN) -m 644 $$f $(INSTALLDIR) ; \
	done
	if [ ! -f $(INSTALLDIR)/htpl-config.pl ] ; then \
	$(INSBIN) -m 644 htpl-config.pl $(INSTALLDIR) ; \
	fi
	touch $(HTLOGS)

install-root : install-it install-samples

install-samples : $(SCRIPTS)
	if [ -z "$(MODPERL)" ]; then \
	$(PERL) config-srv.pl $(CONF) $(INSTALLDIR)/$(BIN) $(HTDOCS) ; \
	else \
	$(PERL) config-mod.pl $(CONF) ; \
	fi
	test -d $(HTDOCS) || mkdir $(HTDOCS)
	test -d $(HTDOCS)/htpl-cache || mkdir $(HTDOCS)/htpl-cache 
	install -m 755 procs.htpm $(HTDOCS)
	install -m 755 server_procs $(HTDOCS)
	install -m 755 server.htpl $(HTDOCS)
	install -m 755 list.htpl $(HTDOCS)
	for f in $(SCRIPTS) ; do \
	$(INSBIN) -m 755 $$f $(INSTALLBIN) ; \
	if [ ! -f $(INSTALLDIR)/$$f ] ; then \
	ln -s $(INSTALLBIN)/$$f $(INSTALLDIR) ; \
	fi ; \
	done
	if [ ! -f $(INSTALLBIN)/$(DBG) ]; then \
	ln -s $(INSTALLDIR)/$(DBG) $(INSTALLBIN) ; \
	fi

build : build-root

install : all install-root

pureinstall: install-root
	
htpl-author.ego :
	touch htpl-author.ego
	echo "$$USER@$$HOSTNAME installed htpl." | mail htpl@atheist.org.il

build-it :
	chmod +x perl.sh
	for pk in $(MODULES) ; do \
	cd $$pk ; \
	$(SH) ../instnonroot $(MODDIR) ; \
	make ; \
	make test ; \
	make install ; \
	cd .. ; \
	done

build-root :
	for pk in $(MODULES) ; do \
	cd $$pk ; \
	$(PERL) Makefile.PL ; \
	make ; \
	make test ; \
	make install ; \
	cd .. ; \
	done

clean-modules:
	for pk in $(MODULES); do \
	cd $$pk; \
	make realclean || true; \
	cd .. ; \
	done
	true

Makefile: Makefile.in config.status
	./config.status

config.status: configure
	./config.status --recheck || ./configure

CPAN : CPAN-root

CPAN-root :
	$(PERL) pre-install.pl $(NOPREREQ)

CPAN-it :
	$(PERL) pre-install.pl NOPREREQ $(MODDIR)

win:
	$(CCPROG) -o htpl.exe htpl.c htpl-l.c htpl-parse.c htpl-sub.c htpl-win.c
	$(CCPROG) -D__DEBUG__ -o htpldbg.exe htpl.c htpl-l.c htpl-parse.c htpl-sub.c htpl-win.c

