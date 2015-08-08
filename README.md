# ObiectumTokenizer

Just a small library for tokenizing strings.

## Features

* Line and column numbers.
* Custom special characters - they're always counted as separate tokens (if not inside a string).
* Custom comments - both single-line and multi-line.
* Strings - a string is always a monolithic token, wrapped in quotes.
* UTF-8 is the only supported encoding.

## Example

<pre><code>obtokenizer_tokenizer_t tokenizer;
if (obtokenizer_init(&tokenizer, "abc /* comment №1 */ def,123 // №2") ||
    obtokenizer_add_spec_char(&tokenizer, ',')                     || // Count commas as separate tokens.
    obscanner_add_comment_mark(&tokenizer.scanner, false, "//")  || // Enable C-style single-line comments.
    obscanner_add_comment_mark(&tokenizer.scanner, true,  "/*")  || // Enable C-style multi-
    obscanner_add_comment_mark(&tokenizer.scanner, true,  "*/")     // line comments.
    ) {
    // error
}

obtokenizer_token_t token;
while (!obtokenizer_get(&tokenizer, &token)) {
    if (token.str[0] == '\0') {
        // No more tokens.
        obtokenizer_free_token(&token);
        break;
    }

    printf("%d:%d: %s\n", token.pos.line, token.pos.col, token.str);

    obtokenizer_free_token(&token); // Must be called before each reuse of a token structure.
}</code></pre>
Output:
<pre><code>$ ./test
1:1: abc
1:6: def
1:9: ,
1:10: 123</code></pre>