#include "command.c"

char *input;

int main() {
  // int cmd_ret = 0;

  while (1) {
    printf("$");

    input = get_input();

    // ctrl+d should exit
    if (input == NULL) {
      shutdown(EXIT_SUCCESS);
    }

    // process commands
    if (!is_blank(input)) {
			struct Commands *commands = parse_commands_with_pipes(input);
      printf("%d", commands->count);
		}

    free(input);

    /* if (cmd_ret == -1) {
      break;
    } */
  }

  shutdown(EXIT_SUCCESS);
  return 0;
}
