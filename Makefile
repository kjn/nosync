prefix = /usr/local
libdir = $(prefix)/lib
OBJS = fsync.o open.o
CFLAGS = -O2 -g

nosync.so: $(OBJS)
	$(CC) -shared -fPIC -lpthread $(CFLAGS) -o $@ $+ -ldl

%.o: %.c
	$(CC) -c -fPIC $(CFLAGS) -o $@ $+

install: nosync.so
	install -d $(libdir)/nosync
	install -p $< $(libdir)/nosync/

clean:
	@rm -f $(OBJS) nosync.so
