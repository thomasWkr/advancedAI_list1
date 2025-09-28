CC = g++
CFLAGS = -I.
DEPS = main.cpp header.hpp structs.hpp
	
main: $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS)
	
.PHONY: clean

clean:
	rm -f *.o *~ main