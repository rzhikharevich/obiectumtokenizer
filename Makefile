CPPFLAGS = -I.

OBJS = $(patsubst %.c,%.o,$(shell find . -name '*.c' -type f -print))


all: build


build: libobiectumtokenizer.so

libobiectumtokenizer.so: $(OBJS)
	$(CC) -shared $(OBJS) -o $@

$(OBJS): */*.h
