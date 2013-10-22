AM_CFLAGS   = -O2
AM_CXXFLAGS = $(CFLAGS) -W -Wall -std=c++0x
AM_LDFLAGS = $(LDFLAGS)

TESTS = run-test.sh
TESTS_ENVIRONMENT = NO_MAKE=yes CUTTER="$(CUTTER)"

LDFLAGS = -module -rpath $(libdir) -avoid-version

noinst_LTLIBRARIES = test.la
test_la_SOURCES = parse-test.cpp

INCLUDES = $(CUTTER_CFLAGS) -I$(top_srcdir)/src
LIBS = $(CUTTER_LIBS) $(top_builddir)/src/sucheme

echo-cutter:
	@echo $(CUTTER)

check-syntax:
	$(CXXCOMPILE) -o nul -S ${CHK_SOURCES}

