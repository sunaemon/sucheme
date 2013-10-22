AM_CFLAGS   = -O2
AM_CXXFLAGS = $(CFLAGS) -W -Wall -std=c++0x
AM_LDFLAGS = $(LDFLAGS) -lboost_thread-mt -lboost_serialization

TESTS = run-test.sh
TESTS_ENVIRONMENT = NO_MAKE=yes CUTTER="$(CUTTER)"

LDFLAGS = -module -rpath $(libdir) -avoid-version

noinst_LTLIBRARIES = test.la
test_la_SOURCES =$(shell find ./ -maxdepth 1 -name "*.h" -or -name "*.hpp") 


INCLUDES = $(CUTTER_CFLAGS) -I$(top_srcdir)/src
LIBS = $(CUTTER_LIBS)

echo-cutter:
	@echo $(CUTTER)

check-syntax:
	$(CXXCOMPILE) -o nul -S ${CHK_SOURCES}

