build:
	gcc main.c -o hello
install:
	mkdir -p $(DESTDIR)/usr/bin
	install -m 755 hello $(DESTDIR)/usr/bin/hello
all:
	build
clean:
	rm -rf hello

