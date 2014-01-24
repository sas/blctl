SOURCES		:= blctl.c
DOCSOURCES	:=
PREFIX		:= /usr/local

CC		?= g++
CFLAGS		:= -std=c99 -Wall -Wextra -D_GNU_SOURCE
LDFLAGS		:=

TARGET		:= blctl
OBJECTS		:= $(SOURCES:.c=.o)
DEPENDS		:= $(SOURCES:.c=.d)
DOCTARGETS	:= $(DOCSOURCES:.1.md=.1)

VPATH		:= $(dir $(lastword $(MAKEFILE_LIST)))

default: $(TARGET)

all: $(TARGET) $(DOCTARGETS)

install: all
	install -d $(PREFIX)/bin/
	install -d $(PREFIX)/share/man/man1/
	install -s -t $(PREFIX)/bin/ $(TARGET)
	install -t $(PREFIX)/share/man/man1/ $(DOCTARGETS)

clean:
	rm -f $(OBJECTS)
	rm -f $(DEPENDS)

distclean: clean
	rm -f $(TARGET)
	rm -f $(DOCTARGETS)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -o $@ -c $<

%.1: %.1.md
	@mkdir -p $(dir $@)
	ronn --roff --pipe $< >$@

.PHONY: all install clean distclean

-include $(DEPENDS)
