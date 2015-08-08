#ifndef _OBIECTUMSCANNER_CHAR_H_
#define _OBIECTUMSCANNER_CHAR_H_


#include "pos.h"


typedef struct {
    int             ch;
    obscanner_pos_t pos;
} obscanner_char_t;

#define OBSCANNER_CHAR_NULL (obscanner_char_t){0, (obscanner_pos_t){0, 0}}


#endif
