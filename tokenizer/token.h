#ifndef _OBIECTUMTOKENIZER_TOKEN_H_
#define _OBIECTUMTOKENIZER_TOKEN_H_


#include <scanner/defs.h>
#include <scanner/pos.h>


typedef struct {
    char            *str;
    obscanner_pos_t pos;
} obtokenizer_token_t;


OBSCANNER_DECL
(
 void obtokenizer_free_token(obtokenizer_token_t *token);
)


#endif
