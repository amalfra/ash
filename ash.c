#include <stdio.h>
#include <stdlib.h>

char *input;

void shutdown(int status) {
  exit(status);
}

char *get_input() {
  int buffer_size = 128;
  int current_size = buffer_size;
  char c = EOF;
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

int main() {
  int cmd_ret = 0;

  while (1) {
    printf("$");

    input = get_input();

    // ctrl+d should exit
    if (input == NULL) {
      shutdown(EXIT_SUCCESS);
    }

    printf("%s\n", input);

    free(input);

    if (cmd_ret == -1) {
      break;
    }
  }

  shutdown(EXIT_SUCCESS);
  return 0;
}
