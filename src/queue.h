#ifndef QUEUE_H
#define QUEUE_H

#include "lexem.h"

typedef struct queue {
  lexem lex;
  struct queue *next;

} queue;

queue *get_last(queue *);
void del_queue(queue **);
lexem pop_first(queue **);
void push_first(queue **, lexem);

#endif  // QUEUE_H
