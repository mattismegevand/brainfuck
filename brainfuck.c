#include <stdio.h>
#include <strings.h>

#define MEMSIZE 30000

#define PARENTHESIS_MATCH(COND, OPERATION, OPENING_CHAR, CLOSING_CHAR) \
        if (COND *ptr) {                                               \
          loop = 1;                                                    \
          while (loop > 0) {                                           \
            c = prog[OPERATION pc];                                    \
            if (c == OPENING_CHAR)                                     \
              loop++;                                                  \
            else if (c == CLOSING_CHAR)                                \
              loop--;                                                  \
          }                                                            \
        }

char mem[MEMSIZE] = {0};
char prog[MEMSIZE] = {0};
char *ptr = mem;

int load_program(char *s) {
  FILE *f;
  int c, n = 0;

  f = fopen(s, "r");
  if (f == NULL) {
    fprintf(stderr, "Error: file does not exist or is not accessible\n");
    return -1;
  }

  while ((c = fgetc(f)) != EOF) {
    if (strchr("><+-.,[]", c))
      prog[n++] = c;
  }
  fclose(f);

  if (n == MEMSIZE) {
    fprintf(stderr, "Error: Program can't be bigger than %d bytes\n", MEMSIZE);
    return -1;
  }

  return 0;
}

void execute_program() {
  int pc, loop;
  char c;

  for (pc = 0; (c = prog[pc]) != 0; pc++) {
    switch (c) {
      case '>':
        ++ptr;
        break;
      case '<':
        --ptr;
        break;
      case '+':
        ++*ptr;
        break;
      case '-':
        --*ptr;
        break;
      case '.':
        putchar(*ptr);
        break;
      case ',':
        *ptr = getchar();
        break;
      case '[':
        PARENTHESIS_MATCH(! , ++, '[', ']');
        break;
      case ']':
        PARENTHESIS_MATCH(!!, --, ']', '[');
        break;
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <program>\n", argv[0]);
    return -1;
  }

  if (load_program(argv[1]))
    return -1;

  execute_program();
}
