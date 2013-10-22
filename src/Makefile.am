AM_CFLAGS   = -O2
AM_CXXFLAGS = $(CFLAGS) -W -Wall -std=c++0x
AM_LDFLAGS = -lboost_thread-mt -lboost_serialization

bin_PROGRAMS = sucheme
sucheme_SOURCES = $(shell find . -name "*.cpp")
check-syntax:
	$(CXXCOMPILE) -o nul -S ${CHK_SOURCES}
