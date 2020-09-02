CXXFLAGS ?= -O2

PREFIX ?= /usr/local
program := settings

all: $(program)

$(program): 
	$(CXX) $(CXXFLAGS) $(program).c -o $(program)

clean:
	rm -f $(program)

install: $(program)
	install -D -t $(DESTDIR)$(PREFIX)/bin $(program)