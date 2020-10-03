EXECUTABLE=ash
DIST_DIR=dist

.PHONY: clean lint

all: shell

shell: ash.c
	${MAKE} clean
	mkdir ${DIST_DIR}
	gcc -g -o $(DIST_DIR)/$(EXECUTABLE) ash.c -Wall -W

run: ash.c
	${MAKE} shell
	$(DIST_DIR)/$(EXECUTABLE)

clean:
	rm -rf $(DIST_DIR)

lint: ash.c
	cppcheck --error-exitcode=1 .
