PACKAGE = nokryptia
VERSION = 1.3.1rc0
VERSION_FLAGS = -DPACKAGE="\"$(PACKAGE)\"" -DVERSION="\"$(VERSION)\""

# Target destinations
DESTDIR ?=
PREFIX  ?= /usr
BINDIR  ?= $(PREFIX)/bin
MANDIR  ?= $(PREFIX)/share/man

# User supplied variables
CXX        = c++
CPPFLAGS   = -I$(PREFIX)/include -Wall
LDFLAGS    = -L$(PREFIX)/lib
LIBS       = -lid3 -lz

EXE         = nokryptia
SRC         = nokryptia.cpp

all: nokryptia

nokryptia:
	$(CXX) -o $(EXE) $(SRC) \
		-std=c++03 $(CPPFLAGS) $(CXXFLAGS) $(VERSION_FLAGS) \
		$(LDFLAGS) $(LIBS)

install: all
	mkdir -p $(DESTDIR)$(BINDIR)
	cp -p $(EXE) $(DESTDIR)$(BINDIR)/$(EXE)
	mkdir -p $(DESTDIR)$(MANDIR)/man1
	cp -p $(EXE).1 $(DESTDIR)$(MANDIR)/man1

clean:
	rm -f $(EXE) *.o
