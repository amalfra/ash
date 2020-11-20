const char pathSeparator =
#ifdef _WIN32
  '\\';
#else
  '/';
#endif

void shutdown(int status);

int is_blank(char *input);

char *get_input();

void close_pipes(int (*pipes)[2], int pipe_count);
