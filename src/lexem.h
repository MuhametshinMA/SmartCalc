#ifndef LEXEM_H
#define LEXEM_H

typedef struct {
  int type;
  int priority;
  double num;
} lexem;

void fill_lexem(lexem *lex, int type, int priority, double num);

#endif  // LEXEM_H
