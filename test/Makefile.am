AM_CPPFLAGS = -I.
AM_CFLAGS   = -O2
AM_CXXFLAGS = $(CFLAGS) -W -Wall -std=c++0x -lpthread -DGTEST_HAS_PTHREAD=1

TESTS = test-sucheme
TESTS_ENVIRONMENT = GTEST_SOURCE_DIR="$(srcdir)/test" \
                    GTEST_BUILD_DIR="$(top_builddir)/test"

check_PROGRAMS= test-sucheme
test_sucheme_SOURCES = parser-test.cpp eval-test.cpp gc-test.cpp gtest/gtest-all.cc gtest/gtest_main.cc
test_sucheme_LDADD = $(top_builddir)/src/libsucheme.la

CLEANFILES = core

INCLUDES = -I$(top_srcdir)/src

check-syntax:
	$(CXXCOMPILE) -o nul -S ${CHK_SOURCES}
