AM_CFLAGS   = -O2
AM_CXXFLAGS = $(CFLAGS) -W -Wall -std=c++0x
AM_LDFLAGS = -lreadline

noinst_LTLIBRARIES = libsucheme.la
libsucheme_la_SOURCES = ./parser.cpp ./eval.cpp ./show.cpp

bin_PROGRAMS = sucheme
sucheme_SOURCES = ./main.cpp
sucheme_LDADD = $(top_builddir)/src/libsucheme.la

check-syntax:
	$(CXXCOMPILE) -o nul -S ${CHK_SOURCES}