/*
 * Filename: inner.h
 * Author: Your Name
 * Description:
 */

#ifndef INNER_H
#define INNER_H

#include "cmd.h"
#include <stdbool.h>

bool is_builtin(Command *cmd, char *cmd_buffer);

#endif /* INNER_H */
