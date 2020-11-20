#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

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

void cleanup_commands(struct Commands *cmds) {
  for (int i = 0; i < cmds->count; i++) {
    free(cmds->cmds[i]);
  }

  free(cmds);
}

// execute command in child process
int exec_command(struct Commands *cmds, struct Command *cmd, int (*pipes)[2]) {
  pid_t child_pid = fork();

  if (child_pid == -1) {
    fprintf(stderr, "Failed to create new process\n");
    return 0;
  }

  if (child_pid == 0) {
    if (pipes != NULL) {
      int pipe_count = cmds->count - 1;
      close_pipes(pipes, pipe_count);
    }

    // execute the command
    char exec_path[50];
    char binPath[50];
    sprintf(binPath, "%c%s%c%s", pathSeparator, "usr", pathSeparator, "bin");
    sprintf(exec_path, "%s%c%s", binPath, pathSeparator, cmd->name);
    execv(exec_path, cmd->argv);

    // execv returns only if an error occurs
    fprintf(stderr, "Failed to execute command due to error: %s\n", strerror(errno));

    // cleanup in the child to avoid memory leaks
    free(pipes);
    cleanup_commands(cmds);

    // exit from child so that the parent can handle the scenario
    exit(EXIT_FAILURE);
  }

  return child_pid;
}
