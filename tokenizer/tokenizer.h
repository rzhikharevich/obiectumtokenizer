#ifndef _OBIECTUMTOKENIZER_TOKENIZER_H_
#define _OBIECTUMTOKENIZER_TOKENIZER_H_


#include <stddef.h>
#include <stdbool.h>
#include "token.h"
#include <scanner/defs.h>
#include <scanner/scanner.h>


enum {
    OBTOKENIZER_PARSE_STRINGS = 1 << 0
};

typedef struct {
    obscanner_scanner_t scanner;
    obscanner_char_t    last_ch;
    
    size_t spec_char_n;
    char   *spec_chars;
    
    int flags;
} obtokenizer_tokenizer_t;


OBSCANNER_DECL
(
 int obtokenizer_init(obtokenizer_tokenizer_t *tokenizer, const char *str);
 int obtokenizer_initn(obtokenizer_tokenizer_t *tokenizer, const char *str, size_t n);
 int obtokenizer_add_spec_char(obtokenizer_tokenizer_t *tokenizer, char sch);
 int obtokenizer_get(obtokenizer_tokenizer_t *tokenizer, obtokenizer_token_t *token);
)


#endif
