CC := g++
SRCDIR := src
OBJDIR := build

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# -g debug, --coverage cobertura
CFLAGS := -Wall -std=c++17
INC := -I include/ -I third_party/

$(OBJDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

main: $(OBJECTS)
	$(CC) $(CFLAGS) $(INC) $(MAIN) $^ -o simulador

all: main

clean:
	$(RM) -r $(OBJDIR)/* simulador coverage/* *.gcda *.gcno

.PHONY: clean