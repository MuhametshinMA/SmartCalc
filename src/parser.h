#ifndef PARSER_H
#define PARSER_H

#include "queue.h"

typedef enum lex_type {
  t_x,
  t_num,
  t_sum,
  t_sub,
  t_mul,
  t_div,
  t_pow,
  t_mod,
  t_cos,
  t_tan,
  t_sin,
  t_sqrt,
  t_ln,
  t_log,
  t_acos,
  t_asin,
  t_atan,
  t_open_bracket
} lex_type;

typedef enum lex_priority {
  p_very_low,  // t_num, t_x, '(' - t_open_bracket
  p_low,       // t_sum, t_sub
  p_middle,    // t_mul, t_div, t_mod
  p_high,      // t_pow, t_cos, t_sin, t_sqrt, t_ln, t_log, t_acos,
               // t_asin, t_atan
  p_higher     // unar (t_sum, t_sub),
} lex_priority;

/// @brief проверяем на корректность список лексем
/// @param que стэк лексем
/// @return 0 - ошибка, 1 - корректно
int is_correct_lexemes(queue* que);

/// @brief найходит заданную лексему
/// @param head стэк лексем
/// @param type_lex тип лексемы
/// @param prior_lex приоритет лексем
/// @return число найденных лексем или 0 если не нашлось ни одной
int find_type_lex(queue* head, int type_lex, int prior_lex);

/// @brief добавляет оператор в список лексем
/// @param t_type тип оператора
/// @param p_priority приоритет
/// @param shift длина строки обозначения оператора
/// @param str исходное математическое выражение
/// @param que стэк лексем
/// @param stack вспомогательный стэк
void add_op(int t_type, int p_priority, int shift, char** str,
            queue** que, queue** stack);

/// @brief добавляет функцию в очередь или в стек в зависимости от приоритета
/// оператора находящегося в стеке
/// @param t_type - тип функции
/// @param shift - смещение указателя строки, на место где заканчивается
/// обозначение функции
/// @param str - исходная строка с математическим выражением
/// @param que - очередь с лексемами в порядке обратной польской нотации в
/// обратном порядке
/// @param stack - вспомогательный стек для записи в очередь в нужной
/// последовательности
/// @return коды ошибок: 1 - ошибка, 0 - нет ошибки
int add_func(int, int, char**, queue**, queue**);

/// @brief достает значения из стека, перекладывает их в очередь пока не
/// встретит символ "("
/// @param  que - очередь с лексемами в порядке обратной польской нотации в
/// обратном порядке
/// @param  stack - вспомогательный стек для записи в очередь в нужной
/// последовательности
/// @return коды ошибок: 1 - ошибка, 0 - нет ошибки
int pop_all_to_open_bracket(queue**, queue**);

/// @brief добавляет лексему в стек, при условии что в стеке находятся операторы
/// более низкого приоритета, если в стеке находятся операторы с приоритетом
/// выше или равным, то сначала они достаются из стека, пока не опустеет стек
/// или не встретится оператор с приоритетом ниже
/// @param  que - очередь с лексемами в порядке обратной польской нотации в
/// обратном порядке
/// @param  stack - вспомогательный стек для записи в очередь в нужной
/// последовательности
/// @param lex - добавляемая лексема
void add_op_to_lex(queue**, queue**, lexem);

/// @brief добавляет функцию в лексемы
/// @param str - исходная строка с математическим выражением
/// @param  que - очередь с лексемами в порядке обратной польской нотации в
/// обратном порядке
/// @param  stack - вспомогательный стек для записи в очередь в нужной
/// последовательности
/// @return 1 ошибка добавления, 0 успешно
int add_func_to_lexemes(char**, queue**, queue**);

/// @brief добавляет оператор в лексемы
/// @param str - исходная строка с математическим выражением
/// @param  que - очередь с лексемами в порядке обратной польской нотации в
/// обратном порядке
/// @param  stack - вспомогательный стек для записи в очередь в нужной
/// последовательности
/// @return 1 ошибка добавления, 0 успешно
int add_ops_to_lexemes(char**, queue**, queue**);

/// @brief добавляет Х в лексемы
/// @param str - исходная строка с математическим выражением
/// @param  que - очередь с лексемами в порядке обратной польской нотации в
/// обратном порядке
/// @return 1 ошибка добавления, 0 успешно
int add_x_to_lexemes(char** str, queue** que);

/// @brief добавляет число в лексемы
/// @param str - исходная строка с математическим выражением
/// @param  que - очередь с лексемами в порядке обратной польской нотации в
/// обратном порядке
/// @return 1 ошибка добавления, 0 успешно
int add_digits_to_lexemes(char**, queue**);

/// @brief разбивает исходное математическое выражение на лексемы в стэк
/// @param  str - исходная строка с математическим выражением  
/// @param  que - очередь с лексемами в порядке обратной польской нотации в
/// обратном порядке
/// @param  stack - вспомогательный стек для записи в очередь в нужной
/// последовательности
/// @return 1 ошибка добавления, 0 успешно
int shatter_to_lexemes(char**, queue**, queue**);

#endif
