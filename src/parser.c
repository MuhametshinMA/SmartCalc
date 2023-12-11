#include "parser.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

int shatter_to_lexemes(char** str, queue** que, queue** stack) {
  char* digits_and_point = "0123456789.";
  char* ops_and_brackets = "()+-*/^";
  char* func_str = "calmst";
  int errcode = 0;
  char* str_ptr = *str;
  while (*str_ptr && !errcode) {
    if (str_ptr == *str && strchr(")*/^", *str_ptr)) {
      errcode = 1;
      break;
    }

    if (isspace(*str_ptr)) {
      str_ptr++;
      continue;
    }
    if (strchr(digits_and_point, *str_ptr)) {
      errcode = add_digits_to_lexemes(&str_ptr, que);
    } else if (strchr("x", *str_ptr)) {
      errcode = add_x_to_lexemes(&str_ptr, que);
    } else if (strchr(func_str, *str_ptr)) {
      errcode = add_func_to_lexemes(&str_ptr, que, stack);
    } else if (strchr("-+", *str_ptr) &&
               (str_ptr == *str || strchr("dgtns(+-*/^", *(str_ptr - 1)) ||
                (strchr("dgtns(+-*/^", *(str_ptr - 2)) &&
                 strchr(" ", *(str_ptr - 1))))) {
      if (*str_ptr == '-') {
        add_op(t_sub, p_higher, 1, &str_ptr, que, stack);
      } else {
        add_op(t_sum, p_higher, 1, &str_ptr, que, stack);
      }
    } else if (strchr(ops_and_brackets, *str_ptr)) {
      errcode = add_ops_to_lexemes(&str_ptr, que, stack);
    } else {
      errcode = 1;
    }
  }

  while (*stack) {
    push_first(que, (*stack)->lex);
    pop_first(stack);
  }
  if (!errcode) {
    errcode = !is_correct_lexemes(*que);
  }

  return errcode;
}

int find_type_lex(queue* head, int type_lex, int prior_lex) {
  int count_type = 0;
  while (head) {
    if ((head->lex.type == type_lex) && (head->lex.priority == prior_lex)) {
      count_type++;
    }
    head = head->next;
  }
  return count_type;
}

int check_on_binar(queue* que) {
  int was_binar = 0;
  if (find_type_lex(que, t_sum, p_low)) {
    was_binar = 1;
  } else if (find_type_lex(que, t_sub, p_low)) {
    was_binar = 1;
  } else if (find_type_lex(que, t_mul, p_middle)) {
    was_binar = 1;
  } else if (find_type_lex(que, t_div, p_middle)) {
    was_binar = 1;
  } else if (find_type_lex(que, t_pow, p_high)) {
    was_binar = 1;
  } else if (find_type_lex(que, t_mod, p_middle)) {
    was_binar = 1;
  }
  return was_binar;
}

int check_on_unar(queue* que) {
  int was_unar = 0;
  if (find_type_lex(que, t_sum, p_higher)) {
    was_unar = 1;
  } else if (find_type_lex(que, t_sub, p_higher)) {
    was_unar = 1;
  } else if (find_type_lex(que, t_cos, p_high)) {
    was_unar = 1;
  } else if (find_type_lex(que, t_sin, p_high)) {
    was_unar = 1;
  } else if (find_type_lex(que, t_tan, p_high)) {
    was_unar = 1;
  } else if (find_type_lex(que, t_ln, p_high)) {
    was_unar = 1;
  } else if (find_type_lex(que, t_log, p_high)) {
    was_unar = 1;
  } else if (find_type_lex(que, t_acos, p_high)) {
    was_unar = 1;
  } else if (find_type_lex(que, t_asin, p_high)) {
    was_unar = 1;
  } else if (find_type_lex(que, t_atan, p_high)) {
    was_unar = 1;
  }
  return was_unar;
}

int is_correct_lexemes(queue* que) {
  int erc = 1;

  int was_unar = check_on_unar(que);
  if (!find_type_lex(que, t_x, p_very_low) &&
      !find_type_lex(que, t_num, p_very_low) && was_unar) {
    erc = 0;
  }

  int was_num = find_type_lex(que, t_num, p_very_low);
  int was_x = find_type_lex(que, t_x, p_very_low);
  int was_binar = check_on_binar(que);
  if (((was_num + was_x) < 2) && was_binar) {
    erc = 0;
  }

  return erc;
}

int add_x_to_lexemes(char** str, queue** que) {
  int errcode = 0;
  if (*(*str + 1) != 'x') {
    lexem lex = {0};
    fill_lexem(&lex, t_x, p_very_low, 0);
    push_first(que, lex);
    *str += 1;
  } else {
    errcode = 1;
  }

  return errcode;
}

int add_func_to_lexemes(char** str, queue** que, queue** stack) {
  int errcode = 0;
  if (!strncmp(*str, "cos", 3)) {
    errcode = add_func(t_cos, 3, str, que, stack);
  } else if (!strncmp(*str, "sin", 3)) {
    errcode = add_func(t_sin, 3, str, que, stack);
  } else if (!strncmp(*str, "tan", 3)) {
    errcode = add_func(t_tan, 3, str, que, stack);
  } else if (!strncmp(*str, "mod", 3)) {
    errcode = add_func(t_mod, 3, str, que, stack);
  } else if (!strncmp(*str, "log", 3)) {
    errcode = add_func(t_log, 3, str, que, stack);
  } else if (!strncmp(*str, "ln", 2)) {
    errcode = add_func(t_ln, 2, str, que, stack);
  } else if (!strncmp(*str, "asin", 4)) {
    errcode = add_func(t_asin, 4, str, que, stack);
  } else if (!strncmp(*str, "acos", 4)) {
    errcode = add_func(t_acos, 4, str, que, stack);
  } else if (!strncmp(*str, "atan", 4)) {
    errcode = add_func(t_atan, 4, str, que, stack);
  } else if (!strncmp(*str, "sqrt", 4)) {
    errcode = add_func(t_sqrt, 4, str, que, stack);
  } else {
    errcode = 1;
  }

  return errcode;
}

int add_func(int t_type, int shift, char** str, queue** que,
             queue** stack) {
  int errcode = 1;
  char* legal_chars = " 0123456789-calmst(x";
  lexem lex = {0};
  if (strchr(legal_chars, *((*str) + shift))) {
    errcode = 0;

    fill_lexem(&lex, t_type, p_high, 0);
    add_op_to_lex(que, stack, lex);
    *str += shift;
  }

  return errcode;
}

int add_ops_to_lexemes(char** str, queue** que, queue** stack) {
  int errcode = 0;
  char* end_ptr = *str;
  lexem lex = {0};
  char* legal_ch = " 0123456789x)";
  if (*end_ptr == '+') {
    add_op(t_sum, p_low, 1, str, que, stack);
  } else if (*end_ptr == '-') {
    add_op(t_sub, p_low, 1, str, que, stack);
  } else if (*end_ptr == '*' &&
             (strchr(legal_ch, *(end_ptr - 1)) ||
              (strchr(legal_ch, *(end_ptr - 2)) && *(end_ptr - 1) == ' '))) {
    add_op(t_mul, p_middle, 1, str, que, stack);
  } else if (*end_ptr == '/' &&
             (strchr(legal_ch, *(end_ptr - 1)) ||
              (strchr(legal_ch, *(end_ptr - 2)) && *(end_ptr - 1) == ' '))) {
    add_op(t_div, p_middle, 1, str, que, stack);
  } else if (*end_ptr == '^' &&
             (strchr(legal_ch, *(end_ptr - 1)) ||
              (strchr(legal_ch, *(end_ptr - 2)) && *(end_ptr - 1) == ' '))) {
    add_op(t_pow, p_high, 1, str, que, stack);
  } else if (*end_ptr == '(') {
    fill_lexem(&lex, t_open_bracket, p_very_low, 0);
    push_first(stack, lex);
    end_ptr++;
    *str = end_ptr;
  } else if (*end_ptr == ')') {
    errcode = pop_all_to_open_bracket(que, stack);
    end_ptr++;
    *str = end_ptr;
  } else {
    errcode = 1;
    del_queue(que);
    del_queue(stack);
  }

  return errcode;
}

void add_op(int t_type, int p_priority, int shift, char** str,
            queue** que, queue** stack) {
  lexem lex = {0};
  fill_lexem(&lex, t_type, p_priority, 0);
  add_op_to_lex(que, stack, lex);
  *str += shift;
}

int pop_all_to_open_bracket(queue** que, queue** stack) {
  int errcode = 1;
  while (*stack) {
    if ((*stack)->lex.type == t_open_bracket) {
      errcode = 0;
      pop_first(stack);
      break;
    }
    push_first(que, (*stack)->lex);
    pop_first(stack);
  }
  return errcode;
}

void add_op_to_lex(queue** que, queue** stack, lexem lex) {
  if (*stack) {
    if ((*stack)->lex.priority < lex.priority ||
        (((*stack)->lex.priority == p_higher) && (lex.priority == p_higher)) ||
        (((*stack)->lex.priority == p_higher) && (lex.priority == p_high)) ||
        (((*stack)->lex.type == t_pow) && (lex.type == t_pow)) ||
        (((*stack)->lex.priority == p_high) && (lex.priority == p_high) &&
         (lex.type != t_pow))) {
      push_first(stack, lex);
    } else {
      while ((*stack) && ((*stack)->lex.priority >= lex.priority)) {
        lexem lex_tmp = pop_first(stack);
        push_first(que, lex_tmp);
      }
      push_first(stack, lex);
    }
  } else {
    push_first(stack, lex);
  }
}

int add_digits_to_lexemes(char** str, queue** que) {
  int errcode = 0;
  int was_point = 0;
  char* end_ptr = *str;

  while ((isdigit(*end_ptr) || *end_ptr == '.') && (was_point < 2)) {
    if (*end_ptr == '.') {
      was_point++;
      if (was_point > 1) {
        errcode = 1;
        break;
      }
    }
    end_ptr++;
  }
  if (!errcode) {
    double num = strtod(*str, &end_ptr);
    lexem lex = {0};
    *str = end_ptr;
    lex.type = t_num;
    lex.priority = p_very_low;
    lex.num = num;
    push_first(que, lex);
  }
  return errcode;
}
