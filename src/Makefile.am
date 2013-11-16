AM_CFLAGS   = -O2
AM_CXXFLAGS = $(CFLAGS) -W -Wall -std=c++0x

noinst_LTLIBRARIES = libsucheme.la
libsucheme_la_SOURCES = ./parser.cpp ./eval.cpp ./show.cpp ./functions.cpp ./list.cpp ./gc.cpp ./environment.cpp

bin_PROGRAMS = sucheme
sucheme_SOURCES = ./main.cpp
sucheme_LDADD = $(top_builddir)/src/libsucheme.la
sucheme_LDFLAGS = -lreadline

check-syntax:
	$(CXXCOMPILE) -o nul -S ${CHK_SOURCES}
