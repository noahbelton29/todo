#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define USAGE "Usage: ./todo add|list|done|clear\n"
#define BUF_SIZE 100

// adds a todo to a todos.txt file
int cmd_add(const char *task) {
  // open a file and add the todo
  FILE *fptr = fopen("todos.txt", "a");
  if (fptr == NULL) {
    fprintf(stderr, "The file failed to open successfully.");
    return 1;
  }

  // write the data
  fputs(task, fptr);
  fputs("\n", fptr);

  fclose(fptr);
  return 0;
}

// lists all current todos
int cmd_list() {
  // open the file in read mode
  FILE* fptr = fopen("todos.txt", "r");
  if (fptr == NULL) {
    fprintf(stderr, "The file failed to open successfully.");
    return 1;
  }

  // list all the todos
  char buf[BUF_SIZE];
  int line = 0;
  while (fgets(buf, sizeof(buf), fptr) != NULL) {
    line++;
    printf("%d. %s", line, buf);
  }

  fclose(fptr);
  return 0;
}

// marks a todo as done and replaces the file with a new one without the done task
int cmd_done(const char *task) {
  // read todo file and create temporary todo file
  FILE* fptr = fopen("todos.txt", "r");
  FILE* tmp_fptr = fopen("todos.tmp", "w");

  // validate input as number
  char *end;
  const long target = strtol(task, &end, 10);
  if (end == task) {
    fprintf(stderr, "Please provide a valid number.\n");
    return 1;
  }

  // copy all non target lines into the temporary file
  int line = 0;
  char buf[BUF_SIZE];
  while (fgets(buf, sizeof(buf), fptr) != NULL) {
    line++;
    if (line != target) {
      fputs(buf, tmp_fptr);
    }
  }

  // check if the target was valid
  if (line < target) {
    fprintf(stderr, "Todo %ld does not exist.\n", target);
    return 1;
  }

  fclose(tmp_fptr);
  fclose(fptr);

  // remove old todos file and replace it with the one without the target to remove
  remove("todos.txt");
  rename("todos.tmp", "todos.txt");
  remove("todos.tmp");
  return 0;
}

// clears all todos
int cmd_clear() {
  // clear the contents of the file
  fclose(fopen("todos.txt", "w"));
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, USAGE);
    return 1;
  }

  // select which task based on the first character of the task input
  switch (argv[1][0]) {
    case 'a': {
      if (argc < 3) { fprintf(stderr, USAGE); return 1; }
      if (strcmp(argv[1], "add") != 0) {
        fprintf(stderr, "\"add\" is the only valid option for this.\n");
        return 1;
      }
      return cmd_add(argv[2]);
    }
    case 'l': {
      if (strcmp(argv[1], "list") != 0) {
        fprintf(stderr, "\"list\" is the only valid option for this.\n");
        return 1;
      }
      return cmd_list();
    }
    case 'd': {
      if (argc < 3) { fprintf(stderr, USAGE); return 1; }
      if (strcmp(argv[1], "done") != 0) {
        fprintf(stderr, "\"done\" is the only valid option for this.");
        return 1;
      }
      return cmd_done(argv[2]);
    }
    case 'c': {
      if (strcmp(argv[1], "clear") != 0) {
        fprintf(stderr, "\"clear\" is the only valid option for this.\n");
        return 1;
      }
      return cmd_clear();
    }
    default:
      fprintf(stderr, "Unknown command: %s\n", argv[1]);
      return 1;
  }
}
