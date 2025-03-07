CC = cc

NAME = libsafearray

PREFIX := /usr/local

VERSION = v0.1.0
CFLAGS  = -std=c17 -O3 -fPIC -Wall -Wextra
LDFLAGS = 

OS := $(shell uname)

.PHONY: tests clean
tests:
	cd $@ && make && ./tests

.PHONY: clean
clean:
	rm -f $(NAME).dylib
	rm -f $(NAME).so
	rm -f tests/tests

.PHONY: example
example: clean
	$(CC) -o $@ example.c $(CFLAGS) $(LDFLAGS)

.PHONY: install
install: clean
ifeq ($(OS),Darwin)
	./build.sh darwin $(BINARY) $(VERSION) $(GIT_SHA)
	cp -f $(BINDIR)/$(BINARY)-darwin $(PREFIX)/$(BINDIR)/$(BINARY)
endif 
ifeq ($(OS),Linux)
	./build.sh linux $(BINARY) $(VERSION) $(GIT_SHA)
	cp -f $(BINDIR)/$(BINARY)-linux $(PREFIX)/$(BINDIR)/$(BINARY)
endif
ifeq ($(OS),FreeBSD)
	./build.sh freebsd $(BINARY) $(VERSION) $(GIT_SHA)
	cp -f $(BINDIR)/$(BINARY)-freebsd $(PREFIX)/$(BINDIR)/$(BINARY)
endif
uninstall: 
	rm -f $(PREFIX)/$(BINDIR)/$(BINARY)*
