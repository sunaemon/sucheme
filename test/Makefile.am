AM_CPPFLAGS = -I$(srcdir) -I$(srcdir)/include
AM_CFLAGS   = -O2
AM_CXXFLAGS = $(CFLAGS) -W -Wall -std=c++0x -lpthread -DGTEST_HAS_PTHREAD=1

lib_LTLIBRARIES = lib/libgtest.la lib/libgtest_main.la
lib_libgtest_la_SOURCES = src/gtest-all.cc

TESTS = test
TESTS_ENVIRONMENT = GTEST_SOURCE_DIR="$(srcdir)/test" \
                    GTEST_BUILD_DIR="$(top_builddir)/test"
check_PROGRAMS= test-sucheme
test_sucheme_SOURCES = parser-test.cpp
test_sucheme_LDADD = lib/libgtest_main.la \
                                 lib/libgtest.la

CLEANFILES = core


INCLUDES = -I$(top_srcdir)/src

check-syntax:
	$(CXXCOMPILE) -o nul -S ${CHK_SOURCES}
