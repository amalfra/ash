#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>

#include "helpers.h"

void shutdown(int status) {
  exit(status);
}

int is_blank(char *input) {
	int n = strlen(input);

	for (int i = 0; i < n; i++) {
		if (!isspace(input[i])) {
			return 0;
    }
	}

	return 1;
}

char *get_input() {
  int buffer_size = 128;
  int current_size = buffer_size;
  int c = EOF;
  int i = 0;

  char *inp = malloc(buffer_size);

  if (inp == NULL) {
    fprintf(stderr, "Failed to allocate read buffer\n");
    shutdown(EXIT_FAILURE);
  }

  // keep reading until enter or end of file is given
  while ((c = getchar()) != '\n' && c != EOF) {
    inp[i++] = c;

    // if the buffer is full allocate more memory
    if (i == current_size) {
      current_size = i + buffer_size;
      inp = realloc(inp, buffer_size);

      if (inp == NULL) {
        fprintf(stderr, "Failed to reallocate read buffer\n");
        shutdown(EXIT_FAILURE);
      }
    }
  }

  // ctrl+d pressed
  if (c == EOF) {
    return NULL;
  }

  inp[i] = '\0';

  return inp;
}

void close_pipes(int (*pipes)[2], int pipe_count) {
	for (int i = 0; i < pipe_count; i++) {
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}
