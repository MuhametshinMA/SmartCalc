#ifndef CALC_H
#define CALC_H

#include "queue.h"

void pop_two_stack(queue **que, queue **stack);
double calc_exp(double x, queue **que);
int get_expression_and_calculate(char *str, double x, double *num);

#endif  // CALC_H
