PREFIX   = /usr/local
CPPFLAGS = -I.
OBJS     = $(patsubst %.c,%.o,$(shell find . -name '*.c' -type f -print))


all: build


install:
	install -C libobiectumtokenizer.so $(PREFIX)/lib
	mkdir -p $(PREFIX)/include/obiectum/scanner
	cp scanner/*.h $(PREFIX)/include/obiectum/scanner
	mkdir -p $(PREFIX)/include/obiectum/tokenizer
	cp tokenizer/*.h $(PREFIX)/include/obiectum/tokenizer


build: libobiectumtokenizer.so

libobiectumtokenizer.so: $(OBJS)
	$(CC) -shared $(OBJS) -o $@

$(OBJS): */*.h
