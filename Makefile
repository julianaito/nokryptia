PACKAGE = nokryptia
VERSION = 1.3.1rc0

DESTDIR ?=
PREFIX  ?= /usr/local
BINDIR  ?= $(PREFIX)/bin
MANDIR  ?= $(PREFIX)/share/man

PKGCONFIG  ?= pkg-config
CXX        ?= c++
CXXFLAGS   ?=
LDFLAGS    ?=

CXXDEPS     = -DPACKAGE="\"$(PACKAGE)\"" -DVERSION="\"$(VERSION)\""
CXXDEPS    += -I$(PREFIX)/include
LIBS        = -L$(PREFIX)/lib -lid3 -lz

EXE         = nokryptia
SRC         = nokryptia.cpp

all: nokryptia

nokryptia:
		$(CXX) -o $(EXE) $(SRC) $(CXXDEPS) $(CXXFLAGS) $(LIBS) $(LDFLAGS)

install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	cp -p $(EXE) $(DESTDIR)$(BINDIR)/$(EXE)
	mkdir -p $(DESTDIR)$(MANDIR)/man1
	cp -p $(EXE).1 $(DESTDIR)$(MANDIR)/man1

clean:
	rm -f $(EXE) *.o
