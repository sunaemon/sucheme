AM_CFLAGS   = -O2
AM_CXXFLAGS = $(CFLAGS) -W -Wall -std=c++0x
AM_LDFLAGS = -lreadline

bin_PROGRAMS = sucheme
sucheme_SOURCES = ./main.cpp

lib_LTLIBRARIES = sucheme.la
liblinukairo_la_SOURCES = ./parser.cpp
liblinukairo_la_LDFLAGS = -version-info 0:0:0

check-syntax:
	$(CXXCOMPILE) -o nul -S ${CHK_SOURCES}
