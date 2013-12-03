AM_CFLAGS   = -O2 -W -Wall -pg -std=c99
AM_CXXFLAGS = -std=c++0x

noinst_LTLIBRARIES = libsucheme.la
libsucheme_la_SOURCES = parser.c eval.c show.c functions.c list.c gc.c environment.c intern.cpp exceptions.c

bin_PROGRAMS = sucheme
sucheme_SOURCES = main.cpp
sucheme_LDADD = libsucheme.la
sucheme_LDFLAGS = -lreadline

get_cs_flags = $(foreach target,$(subst .,_,$(subst -,_,$($(2)))),$($(target)_$(1)FLAGS))
get_cs_all_flags = $(foreach type,$(2),$(call get_cs_flags,$(1),$(type)))
get_cs_compile = $(if $(subst C,,$(1)),$($(1)COMPILE),$(COMPILE))
get_cs_cmdline = $(call get_cs_compile,$(1)) $(call get_cs_all_flags,$(1),check_PROGRAMS bin_PROGRAMS lib_LTLIBRARIES) -fsyntax-only

.PHONY : check-syntax 
check-syntax:
	s=$(suffix $(CHK_SOURCES));\
	if   [ "$$s" = ".c"   ]; then $(call get_cs_cmdline,C)	 $(CHK_SOURCES);\
	elif [ "$$s" = ".cpp" ]; then $(call get_cs_cmdline,CXX) $(CHK_SOURCES);\
	else exit 1; fi	
