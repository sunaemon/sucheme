AM_CFLAGS   = -O2
AM_CXXFLAGS = -W -Wall -std=c++0x -pg

noinst_LTLIBRARIES = libsucheme.la
libsucheme_la_SOURCES = parser.cpp eval.cpp show.cpp functions.cpp list.cpp gc.cpp environment.cpp intern.cpp

bin_PROGRAMS = sucheme
sucheme_SOURCES = main.cpp
sucheme_LDADD = libsucheme.la
sucheme_LDFLAGS = -lreadline

check-syntax:
	$(CXXCOMPILE)  -Wall -Wextra -pedantic -fsyntax-only -o nul -S ${CHK_SOURCES}
	cppcheck --enable=all --std=c++11 --template gcc -q $(CHK_SOURCES)
