#include "lexem.h"

#include <string.h>

void fill_lexem(lexem *lex, int type, int priority, double num) {
  lex->type = type;
  lex->priority = priority;
  lex->num = num;
}