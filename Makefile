# Project directories
SRCDIR := Source
INCDIR := Include
BUILDDIR := Build
OBJDIR := $(BUILDDIR)/obj

# Toolchain
CC ?= gcc
CFLAGS ?= -Wall -I$(INCDIR) -O2 -fPIC

# Discover sources
SRCS := $(wildcard $(SRCDIR)/*.c)
LIBSRCS := $(filter-out $(SRCDIR)/main.c,$(SRCS))
TESTSRC := $(SRCDIR)/main.c

# Objects
LIBOBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(LIBSRCS))
TESTOBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(TESTSRC))

# Targets
STATIC_LIB := $(BUILDDIR)/libcoroutine.a
SHARED_LIB := $(BUILDDIR)/libcoroutine.so
TEST_BIN := $(BUILDDIR)/test

ALL_TARGETS := $(STATIC_LIB) $(SHARED_LIB) $(TEST_BIN)

.PHONY: all clean distclean

all: $(ALL_TARGETS)

# Create build dirs
$(BUILDDIR) $(OBJDIR):
	mkdir -p $@

# Compile objects
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Static library
$(STATIC_LIB): $(LIBOBJS) | $(BUILDDIR)
	$(AR) $(ARFLAGS) $@ $^

# Shared library
$(SHARED_LIB): $(LIBOBJS) | $(BUILDDIR)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

# Test executable links against static library to avoid runtime .so dependency
$(TEST_BIN): $(TESTOBJ) $(STATIC_LIB) | $(BUILDDIR)
	$(CC) -o $@ $(TESTOBJ) $(STATIC_LIB) $(LDFLAGS)
	
clean:
	rm -rf $(BUILDDIR)/obj

distclean:
	rm -rf $(BUILDDIR)