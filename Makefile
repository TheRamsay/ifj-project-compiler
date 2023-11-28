# Implementace překladače imperativního jazyka IFJ22
#
# Dominik Huml (xhumld00@stud.fit.vutbr.cz)
# Matyáš Krejza (xkrejz07@stud.fit.vutbr.cz)
# Petra Šimonová (xsimon30@stud.fit.vutbr.cz)
# Lukáš Witpeerd (xwitpe00@stud.fit.vutbr.cz)

# Compiler settings
CC = gcc
CFLAGS = -std=c11 -g3 -O0 -Wall -Wextra -Werror -pedantic

# Get all .c files
SRCS = $(wildcard *.c)
# Get corresponding .o files
OBJS := $(SRCS:%.c=%.o)
# Get corresponding .d files
DEPS := $(SRCS:%.c=%.d)

# These will run every time (not just when the files are newer)
.PHONY: run clean zip test pdf examples

# Main target
main: $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^ -lm

# Dependecies
%.o: %.c %.d
	$(CC) -MT $@ -MMD -MP -MF $*.d $(CFLAGS) $(CPPFLAGS) -c $(OUTPUT_OPTION) $<
$(DEPS):
include $(wildcard $(DEPS))

# Run the program
run: main
	./main

# Clean up
clean:
	rm -rf main scanner *.o *.out *.d *.a *.log xhumld00.zip ./cmake-build vgcore.* 

# PDF documentation
pdf:
	pdflatex -interaction nonstopmode doc/dokumentace.tex

# Pack for submission
zip:
	zip xhumld00.zip *.c *.h dokumentace.pdf Makefile rozdeleni rozsireni

test:
	mkdir -p cmake-build
	cd cmake-build && cmake ./..
	cd cmake-build && make all && ctest --verbose $(CTESTFLAGS)