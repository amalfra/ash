#define ARG_MAX_COUNT 1024  
#define CMD_DELIM " "  
#define CMD_PIPE_DELIM "|"   
#define STR_END_MARKER '\0' 

// to store parsed command
struct Command {
	int argc;
	char *name;
	char *argv[ARG_MAX_COUNT];
};

// to store list of read commands
struct Commands {
	int count;
	struct Command *cmds[];
};

struct Command *parse_command(char *input);

struct Commands *parse_commands_with_pipes(char *input);

void cleanup_commands(struct Commands *cmds);

int exec_command(struct Commands *cmds, struct Command *cmd, int (*pipes)[2]);
