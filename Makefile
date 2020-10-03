EXECUTABLE=ash

all: shell

shell: ash.c
	gcc -g -o $(EXECUTABLE) ash.c -Wall -W

.PHONY: clean
clean:
	rm $(EXECUTABLE)

.PHONY: lint
lint: ash.c
	cppcheck --error-exitcode=1 .
