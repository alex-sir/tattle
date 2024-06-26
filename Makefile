# Name of executable
BINS = tattle 

# Setting the flags
CFLAGS = -g -Wall -Wstrict-prototypes

# Link in support for debugging
LDFLAGS = -g

# Default target produced by entering "make" alone
.PHONY: default
default: $(BINS)

# Compile *.c into *.o
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Remove *.o files, but leave executable
.PHONY: clean
clean:
	rm -f core* src/*.o *~

# Link *.o files into an executable
tattle: src/tattle.o src/options.o src/helpers.o
	$(CC) $(LDFLAGS) $^ -o $@

# Run tests
testd: tattle # d = default
	./tattle

testf: tattle # f = full arguments
	./tattle -d 3/30/24 -f test/wtmp -t 20:30 -u user1,user2,user3

# Check for memory leaks
memd: tattle
	valgrind --leak-check=full --show-leak-kinds=all -s ./tattle

memf: tattle
	valgrind --leak-check=full --show-leak-kinds=all -s ./tattle -d 3/30/24 -f test/wtmp -t 20:30 -u user1,user2,user3

# Remove all files that can be reconstructed through "make"
.PHONY: immaculate
immaculate: clean
	rm -f $(BINS)
