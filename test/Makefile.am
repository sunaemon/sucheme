
AM_CPPFLAGS = -I.
AM_CFLAGS   = -O2
AM_CXXFLAGS = -W -Wall -std=c++0x -lpthread -DGTEST_HAS_PTHREAD=1 -pg

TESTS = test-sucheme
TESTS_ENVIRONMENT = GTEST_SOURCE_DIR="$(srcdir)/test" \
                    GTEST_BUILD_DIR="$(top_builddir)/test"

check_PROGRAMS= test-sucheme
test_sucheme_SOURCES = parser-test.cpp eval-test.cpp gc-test.cpp gtest/gtest-all.cc main.cpp environment-test.cpp
test_sucheme_LDADD = ../src/libsucheme.la

CLEANFILES = core

INCLUDES = -I$(top_srcdir)/src

check-syntax:
	$(CXXCOMPILE)  -Wall -Wextra -pedantic -fsyntax-only -o nul -S ${CHK_SOURCES}
	cppcheck --enable=all --std=c++11 --template gcc -q $(CHK_SOURCES)
