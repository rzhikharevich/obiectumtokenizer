#include "scanner.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>


static void obscanner_init_def(obscanner_scanner_t *scanner) {
    scanner->pos = (obscanner_pos_t){1, 1};
    
    scanner->single_line_comment_mark_n   = 0;
    scanner->single_line_comment_marks    = NULL;
    scanner->multiple_line_comment_mark_n = 0;
    scanner->multiple_line_comment_marks  = NULL;
    
    scanner->string_open = false;
}

void obscanner_init(obscanner_scanner_t *scanner, const char *str) {
    scanner->str      = str;
    scanner->str_last = str + strlen(str);
    obscanner_init_def(scanner);
}

void obscanner_initn(obscanner_scanner_t *scanner, const char *str, size_t n) {
    scanner->str      = str;
    scanner->str_last = str + n;
    obscanner_init_def(scanner);
}

int obscanner_add_comment_mark(obscanner_scanner_t *scanner, bool multiline, const char *mark) {
    size_t     *mark_n  = multiline? &scanner->multiple_line_comment_mark_n : &scanner->single_line_comment_mark_n;
    const char ***marks = multiline? &scanner->multiple_line_comment_marks  : &scanner->single_line_comment_marks;
    
    *marks = reallocf(*marks, ++*mark_n * sizeof(char *));
    if (!*marks)
        return -1;
    
    (*marks)[*mark_n - 1] = mark;
    
    return 0;
}

static int obscanner_comment_type(obscanner_scanner_t *scanner, size_t *mark_index) {
    for (*mark_index = 0; *mark_index < scanner->single_line_comment_mark_n; (*mark_index)++)
        for (size_t i = 0; scanner->str[i] == scanner->single_line_comment_marks[*mark_index][i]; i++)
            if (!scanner->single_line_comment_marks[*mark_index][i + 1])
                return 0;
    
    for (*mark_index = 0; *mark_index < scanner->multiple_line_comment_mark_n; *mark_index += 2)
        for (size_t i = 0; scanner->str[i] == scanner->multiple_line_comment_marks[*mark_index][i]; i++)
            if (!scanner->multiple_line_comment_marks[*mark_index][i + 1])
                return 1;
        
    return -1;
}

static void obscanner_find_ch(obscanner_scanner_t *scanner) {
    if (scanner->string_open)
        return;
    
    while (*scanner->str) {
        size_t cmt_mark_index;
        int cmt_type = obscanner_comment_type(scanner, &cmt_mark_index);
        if (cmt_type >= 0) {
            if (cmt_type == 0) {
                scanner->str = memchr(scanner->str, '\n', scanner->str_last - scanner->str);
                if (!scanner->str)
                    scanner->str = scanner->str_last;
            } else {
                scanner->str = strnstr(scanner->str,
                                       scanner->multiple_line_comment_marks[cmt_mark_index + 1],
                                       scanner->str_last - scanner->str);
                if (!scanner->str)
                    scanner->str = scanner->str_last;
                else
                    scanner->str += strlen(scanner->multiple_line_comment_marks[cmt_mark_index + 1]);
            }
        }
        
        if (!isspace(*scanner->str))
            break;
        
        if (*scanner->str == '\n') {
            scanner->pos.line++;
            scanner->pos.col = 1;
        } else
            scanner->pos.col++;
        
        scanner->str++;
    }
}

int obscanner_get(obscanner_scanner_t *scanner, obscanner_char_t *ch) {
    if (scanner->str >= scanner->str_last) {
        ch->ch  = '\0';
        ch->pos = scanner->pos;
        
        return 0;
    }
    
    obscanner_find_ch(scanner);
    
    OBSCANNER_UNICHAR_DO
    (*scanner->str,
     // 1 byte.
     ch->ch = *scanner->str;,
     // 2 bytes.
     ch->ch = *(uint16_t *)scanner->str;,
     // 3 bytes.
     {
         ch->ch = *(uint16_t *)scanner->str;
         ((char *)&ch->ch)[2] = scanner->str[2];
     },
     // 4 bytes.
     ch->ch = *(uint32_t *)scanner->str;,
     // Invalid char.
     return -1;
    )
    
    ch->pos = scanner->pos;
    
    if (scanner->str < scanner->str_last) {
        if (ch->ch == '"' || ch->ch == '\'')
            scanner->string_open = !scanner->string_open;
        
        OBSCANNER_UNICHAR_NEXT(scanner->str, return -1;)
        scanner->pos.col++;
    }
    
    return 0;
}
