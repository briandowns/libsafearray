CC = cc

BINDIR := bin
BINARY := libsafearray
PREFIX := /usr/local

VERSION = v0.1.0
GIT_SHA = $(shell git rev-parse HEAD)
LDFLAGS = -ldflags "-X main.gitSHA=$(GIT_SHA) -X main.version=$(VERSION) -X main.name=$(BINARY)"

OS := $(shell uname)

$(BINDIR)/$(BINARY): clean
	$(CC) build $(LDFLAGS) -o $@

.PHONY: test
test:
	// 
	cd test && \
	make

.PHONY: clean
clean:
	rm -f $(BINARY)
	rm -f $(BINDIR)/*

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
