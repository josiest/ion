# Define the project structure variables
LIBDIR = lib
BINDIR = bin
SRCDIR = src
INC = -Iinclude
LOGFILE = error.log

# Define the extension names
SRCEXT = cpp

# Define the compilers and their arguments
AR = ar rcs
CC = g++
CFLAGS = -Wall -std=c++17

OBJS = $(patsubst $(SRCDIR)/%.$(SRCEXT), $(BINDIR)/%.o, \
	   $(wildcard $(SRCDIR)/*))

libion: $(OBJS)
	mkdir -p $(LIBDIR)
	$(AR) $(LIBDIR)/$@.a $^ 2> $(LOGFILE)

$(BINDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC) 2> $(LOGFILE)

.PHONY: clean

clean:
	rm -r $(BINDIR) $(LIBDIR)
