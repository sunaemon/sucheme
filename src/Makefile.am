AM_CFLAGS   = -O2 -W -Wall -std=c99 -lstdc++
AM_CXXFLAGS = -O2 -W -Wall -std=c++0x

noinst_LTLIBRARIES = libsucheme.la
libsucheme_la_SOURCES = parser.h parser.c eval.h eval.c show.h show.c functions.h functions.c list.h list.c gc.h gc.c environment.h environment.c intern.h intern.cpp exceptions.h exceptions.c macro.h gc_objects.h gc_objects.c

bin_PROGRAMS = sucheme
sucheme_SOURCES = main.c
sucheme_LDADD = libsucheme.la

.PHONY : check-syntax 
check-syntax:
	s=$(suffix $(CHK_SOURCES));\
	if   [ "$$s" = ".cpp"   ]; then\
	    $(CXXCOMPILE)  -Wall -Wextra -pedantic -fsyntax-only -o nul -std=c++11 -S ${CHK_SOURCES};\
	elif [ "$$s" = ".c" ]; then\
	    $(CC)  -Wall -Wextra -pedantic -fsyntax-only -o nul -std=c99 -S ${CHK_SOURCES};\
	else exit 1; fi
