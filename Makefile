EXECUTABLE=ash
DIST_DIR=dist

.PHONY: clean lint

all: shell

shell: ash.c
	${MAKE} clean
	${MAKE} lint
	mkdir ${DIST_DIR}
	gcc -o $(DIST_DIR)/$(EXECUTABLE) ash.c

run: ash.c
	${MAKE} shell
	$(DIST_DIR)/$(EXECUTABLE)

clean:
	rm -rf $(DIST_DIR)

lint: ash.c
	cppcheck --enable=all --error-exitcode=1 .
