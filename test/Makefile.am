AM_CFLAGS   = -O2
AM_CXXFLAGS = $(CFLAGS) -W -Wall -std=c++0x
AM_LDFLAGS = $(LDFLAGS) -module -rpath $(libdir) -avoid-version

TESTS = run-test.sh
TESTS_ENVIRONMENT = NO_MAKE=yes CUTTER="$(CUTTER)"

noinst_PROGRAMS = test-sucheme
test_sucheme_SOURCES = test-sucheme.cpp
test_sucheme_LDADD = ../src/libsucheme.la

noinst_LTLIBRARIES = test.la
test_la_SOURCES = parse-test.cpp
test_la_LIBADD = $(top_builddir)/src/libsucheme.la

INCLUDES = $(CUTTER_CFLAGS) -I$(top_srcdir)/src
LIBS = $(CUTTER_LIBS) 

echo-cutter:
	@echo $(CUTTER)

check-syntax:
	$(CXXCOMPILE) -o nul -S ${CHK_SOURCES}

