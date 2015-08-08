#include "token.h"
#include <stdlib.h>


void obtokenizer_free_token(obtokenizer_token_t *token) {
    free(token->str);
}
