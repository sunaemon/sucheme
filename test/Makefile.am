AM_CFLAGS   = -O2
AM_CXXFLAGS = $(CFLAGS) -W -Wall -std=c++0x

noinst_PROGRAMS = test-sucheme
test_sucheme_SOURCES = parser-test.cpp main.cpp
test_sucheme_LDADD = $(top_builddir)/src/libsucheme.la 
test_sucheme_LDFLAGS = -L$(libdir) -lcppunit -ldl

INCLUDES = -I$(top_srcdir)/src

echo-cutter:
	@echo $(CUTTER)

check-syntax:
	$(CXXCOMPILE) -o nul -S ${CHK_SOURCES}
