#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "helpers.c"

struct Command *parse_command(char *input) {
	int tokenCount = 0;
	char *token;

	struct Command *cmd = calloc(sizeof(struct Command) + ARG_MAX_COUNT * sizeof(char *), 1);

	if (cmd == NULL) {
    fprintf(stderr, "Failed to allocate command buffer\n");
    shutdown(EXIT_FAILURE);
	}

	// split based on space
	token = strtok(input, CMD_DELIM);

	while (token != NULL && tokenCount < ARG_MAX_COUNT) {
		cmd->argv[tokenCount++] = token;
		token = strtok(NULL, CMD_DELIM);
	}

  // the command will be first token; eg: cd myfolder/myfolder2
	cmd->name = cmd->argv[0];
	cmd->argc = tokenCount;

	return cmd;
}

struct Commands *parse_commands_with_pipes(char *input) {
	int count = 0;
  int i = 0;
	char *token;
	char *c = input;
  struct Commands *cmds;

  // count the number of pipes to find the number of commands
	while (*c != STR_END_MARKER) {
		if (strncmp(c, CMD_PIPE_DELIM, 1) == 0) {
		  count++;
    }
		c++;
	}

	count++;

	cmds = calloc(sizeof(struct Commands) + count * sizeof(struct Command *), 1);

	if (cmds == NULL) {
    fprintf(stderr, "Failed to allocate pipe commands buffer\n");
    shutdown(EXIT_FAILURE);
	}

  // split based on pipe operator
	token = strtok(input, CMD_PIPE_DELIM);

	while (token != NULL && i < count) {
		cmds->cmds[i++] = parse_command(token);
		token = strtok(NULL, CMD_PIPE_DELIM);
	}

	cmds->count = count;

	return cmds;
}
