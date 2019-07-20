include ../../Makefile.common

LIBRARY = libshell.a

all: $(LIBRARY) test

include Makefile.shared

test: $(TEST_OBJS) $(LIBRARY)
	$(LD) -o $@ $(LDFLAGS) -L. $(TEST_OBJS) -lshell -lc

$(LIBRARY): $(LIBSHELL_OBJS)
	@echo "LIB $@"; $(AR) rcs $@ $(LIBSHELL_OBJS)

install: $(LIBRARY)
	mkdir -p ../../Root/usr/include/LibShell
	# Copy headers
	rsync -r -a --include '*/' --include '*.h' --exclude '*' . ../../Root/usr/include/LibShell
	# Install the library
	cp $(LIBRARY) ../../Root/usr/lib

