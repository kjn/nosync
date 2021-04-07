srcdir=.
prefix = /usr/local
libdir = $(prefix)/lib
OBJS = fsync.o open.o checkfd.o
CFLAGS = -O2 -g
LIBRARY = nosync.so

nosync.so: $(OBJS)
	$(CC) -shared -fPIC -ldl -lpthread $(CFLAGS) -o $@ $+

%.o: %.c
	$(CC) -c -fPIC $(CFLAGS) -o $@ $+

install: $(LIBRARY)
	install -d $(libdir)/nosync
	install -p $< $(libdir)/nosync/

test_nosync: test_nosync.c
	$(CC) $(CFLAGS) -o $@ $+

test: test_nosync $(LIBRARY)
	LD_LIBRARY=$(srcdir)/$(LIBRARY) ./test_nosync

clean:
	@rm -f $(OBJS) nosync.so test_nosync
