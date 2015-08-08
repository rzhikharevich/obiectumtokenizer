#include "tokenizer.h"
#include <stdlib.h>
#include <string.h>


static int obtokenizer_init_def(obtokenizer_tokenizer_t *tokenizer) {
    tokenizer->spec_char_n   = 1;
    tokenizer->spec_chars    = malloc(1);
    tokenizer->spec_chars[0] = '\0';
    tokenizer->flags         = OBTOKENIZER_PARSE_STRINGS;
    return obscanner_get(&tokenizer->scanner, &tokenizer->last_ch);
}

int obtokenizer_init(obtokenizer_tokenizer_t *tokenizer, const char *str) {
    obscanner_init(&tokenizer->scanner, str);
    return obtokenizer_init_def(tokenizer);
}

int obtokenizer_initn(obtokenizer_tokenizer_t *tokenizer, const char *str, size_t n) {
    obscanner_initn(&tokenizer->scanner, str, n);
    return obtokenizer_init_def(tokenizer);
}

int obtokenizer_add_spec_char(obtokenizer_tokenizer_t *tokenizer, char sch) {
    tokenizer->spec_chars = reallocf(tokenizer->spec_chars, ++tokenizer->spec_char_n);
    if (!tokenizer->spec_chars)
        return -1;
    
    tokenizer->spec_chars[tokenizer->spec_char_n - 1] = sch;
    
    return 0;
}

static bool obtokenizer_is_spec_char(obtokenizer_tokenizer_t *tokenizer, char ch) {
    for (size_t i = 0; i < tokenizer->spec_char_n; i++)
        if (tokenizer->spec_chars[i] == ch)
            return true;
    
    return false;
}

static int obtokenizer_unichar_size(char ch) {
    OBSCANNER_UNICHAR_DO
    (
     ch,
     return 1;,
     return 2;,
     return 3;,
     return 4;,
     return 0;  // Invalid character.
    )
}

int obtokenizer_get(obtokenizer_tokenizer_t *tokenizer, obtokenizer_token_t *token) {
    token->pos = tokenizer->last_ch.pos;
    size_t sz  = 1;
    
    if (obtokenizer_is_spec_char(tokenizer, tokenizer->last_ch.ch)) {
        token->str = malloc(++sz);
        if (!token->str)
            return -1;
        
        token->str[0] = tokenizer->last_ch.ch;
        
        if (obscanner_get(&tokenizer->scanner, &tokenizer->last_ch))
            return -1;
    } else {
        obscanner_pos_t last_pos;
        token->str = NULL;

        do {
            last_pos = tokenizer->last_ch.pos;
            
            int ch_size = obtokenizer_unichar_size(tokenizer->last_ch.ch);
            if (!ch_size)
                return -1;
            
            token->str = reallocf(token->str, sz += ch_size);
            if (!token->str)
                return -1;
            memcpy(token->str + sz - 1 - ch_size, &tokenizer->last_ch.ch, ch_size);
            
            if (obscanner_get(&tokenizer->scanner, &tokenizer->last_ch))
                return -1;
            
            if (tokenizer->last_ch.pos.line != last_pos.line ||
                tokenizer->last_ch.pos.col  >  last_pos.col + 1)
                break;
        } while (!obtokenizer_is_spec_char(tokenizer, tokenizer->last_ch.ch));
    }
    
finish:
    token->str[sz - 1] = '\0';
    
    return 0;
}
