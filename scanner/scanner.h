#ifndef _OBIECTUMSCANNER_SCANNER_H_
#define _OBIECTUMSCANNER_SCANNER_H_


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "defs.h"
#include "pos.h"
#include "char.h"


typedef struct {
    const char      *str;
    const char      *str_last;
    obscanner_pos_t pos;
    
    size_t     single_line_comment_mark_n;
    const char **single_line_comment_marks;
    size_t     multiple_line_comment_mark_n;
    size_t     *multiple_line_comment_mark_l;
    const char **multiple_line_comment_marks;
    
    bool string_open;
} obscanner_scanner_t;

#define OBSCANNER_UNICHAR_DO(_UNICHAR, b1, b2, b3, b4, invl) \
    if (!((uint8_t)(_UNICHAR) & 128))\
        b1\
    else if (((uint8_t)(_UNICHAR) >> 5) == 6)\
        b2\
    else if (((uint8_t)(_UNICHAR) >> 4) == 14)\
        b3\
    else if (((uint8_t)(_UNICHAR) >> 3) == 30)\
        b4\
    else\
        invl

#define OBSCANNER_UNICHAR_NEXT(_UNISTR, invl) OBSCANNER_UNICHAR_DO\
(\
 *_UNISTR,\
 _UNISTR++;,\
 _UNISTR += 2;,\
 _UNISTR += 3;,\
 _UNISTR += 4;,\
 invl\
)

OBSCANNER_DECL
(
 void obscanner_init(obscanner_scanner_t *scanner, const char *str);
 void obscanner_initn(obscanner_scanner_t *scanner, const char *str, size_t n);
 int  obscanner_add_comment_mark(obscanner_scanner_t *scanner, bool multiline, const char *mark);
 int  obscanner_get(obscanner_scanner_t *scanner, obscanner_char_t *ch);
)


#endif
