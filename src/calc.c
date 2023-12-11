#include "calc.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

int is_brackets_eq(char *str) {
  int open_brakets_ch = 0;
  int close_brackets_ch = 0;
  size_t str_len = strlen(str);
  for (size_t i = 0; i < str_len; i++) {
    if (str[i] == ')') {
      close_brackets_ch++;
    } else if (str[i] == '(') {
      open_brakets_ch++;
    }
  }
  return close_brackets_ch == open_brakets_ch ? 1 : 0;
}

int get_expression_and_calculate(char *str, double x, double *num) {
  queue *que = NULL;
  queue *que_back_polish = NULL;
  queue *stack = NULL;
  int erc = 0;
  int str_len = strlen(str);
  if (str_len != 0 && str_len < 255 && is_brackets_eq(str)) {
    if (!(erc = shatter_to_lexemes(&str, &que, &stack))) {
      while (que) {
        push_first(&que_back_polish, que->lex);
        pop_first(&que);
      }
      *num = calc_exp(x, &que_back_polish);
    } else {
      del_queue(&que);
    }

  } else {
    erc = 1;
  }
  del_queue(&que_back_polish);
  del_queue(&stack);
  return erc;
}

double calc_exp(double x, queue **que) {
  queue *stack = NULL;
  while (*que) {
    if ((*que)->lex.type == t_num) {
      push_first(&stack, (*que)->lex);
      pop_first(que);
    } else if ((*que)->lex.type == t_x) {
      lexem lex = {0};
      fill_lexem(&lex, t_x, p_very_low, x);
      push_first(&stack, lex);
      pop_first(que);
    } else if (((*que)->lex.type == t_sum) && ((*que)->lex.priority == p_low)) {
      double num = stack->lex.num;
      pop_two_stack(que, &stack);
      stack->lex.num += num;
    } else if (((*que)->lex.type == t_sub) && ((*que)->lex.priority == p_low)) {
      double num = stack->lex.num;
      pop_two_stack(que, &stack);
      stack->lex.num -= num;
    } else if ((*que)->lex.type == t_mul) {
      double num = stack->lex.num;
      pop_two_stack(que, &stack);
      stack->lex.num *= num;
    } else if ((*que)->lex.type == t_div) {
      double num = stack->lex.num;
      pop_two_stack(que, &stack);
      stack->lex.num /= num;
    } else if ((*que)->lex.type == t_mod) {
      double num = stack->lex.num;
      pop_two_stack(que, &stack);
      stack->lex.num = fmod(stack->lex.num, num);
    } else if (((*que)->lex.type == t_sum) &&
               ((*que)->lex.priority == p_higher)) {
      pop_first(que);
    } else if (((*que)->lex.type == t_sub) &&
               ((*que)->lex.priority == p_higher)) {
      stack->lex.num *= -1;
      pop_first(que);
    } else if ((*que)->lex.type == t_pow) {
      double num = stack->lex.num;
      pop_two_stack(que, &stack);
      stack->lex.num = pow(stack->lex.num, num);
    } else if ((*que)->lex.type == t_cos) {
      stack->lex.num = cos(stack->lex.num);
      pop_first(que);
    } else if ((*que)->lex.type == t_sin) {
      stack->lex.num = sin(stack->lex.num);
      pop_first(que);
    } else if ((*que)->lex.type == t_tan) {
      stack->lex.num = tan(stack->lex.num);
      pop_first(que);
    } else if ((*que)->lex.type == t_sqrt) {
      stack->lex.num = sqrt(stack->lex.num);
      pop_first(que);
    } else if ((*que)->lex.type == t_ln) {
      stack->lex.num = log(stack->lex.num);
      pop_first(que);
    } else if ((*que)->lex.type == t_log) {
      stack->lex.num = log10(stack->lex.num);
      pop_first(que);
    } else if ((*que)->lex.type == t_acos) {
      stack->lex.num = acos(stack->lex.num);
      pop_first(que);
    } else if ((*que)->lex.type == t_asin) {
      stack->lex.num = asin(stack->lex.num);
      pop_first(que);
    } else if ((*que)->lex.type == t_atan) {
      stack->lex.num = atan(stack->lex.num);
      pop_first(que);
    }
  }
  double num = stack->lex.num;
  del_queue(&stack);
  return num;
}

void pop_two_stack(queue **que, queue **stack) {
  pop_first(stack);
  pop_first(que);
}
