#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void del_queue(queue **head) {
  while (*head) {
    pop_first(head);
  }
}

lexem pop_first(queue **head) {
  queue *prev = NULL;
  lexem lex = {0};

  if (head) {
    lex = (*head)->lex;
    prev = (*head);
    (*head) = (*head)->next;
    free(prev);
  }
  return lex;
}

void push_first(queue **head, lexem lex) {
  queue *tmp = (queue *)malloc(sizeof(queue));
  tmp->lex = lex;
  tmp->next = (*head);
  (*head) = tmp;
}
