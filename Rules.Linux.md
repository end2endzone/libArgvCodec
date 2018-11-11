# Command Line Rules #



## Linux ##

The following list shows all the special rules implemented by the library for properly:
* parsing a command line into arguments and,
* escaping arguments into a valid command line.

The list of rules applies to the Terminal environment (bash) of the Linux platform.

1. Argument Delimiters:
   1. [space] or tab characters are argument delimiters/separators but *ONLY* when outside a string.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| a b c        | a       | b       | c       |

2. Strings:
   1. If an argument contains [space] or tab characters, it must be enclosed in a string to form a single argument.
   2. Double quotes character `"` starts/ends a string. The `"` character is omitted from the argument.
   3. Single quote character `'` also starts/ends a string. The `'` character is omitted from the argument.
   4. If a string is juxtaposed to another string or argument (not separated by a delimiter character), they form the same argument.

| Command Line               | argv[1]    | argv[2] | argv[3] | argv[4] |
|----------------------------|------------|---------|---------|---------|
| "a b c"                    | a b c      |         |         |         |
| a" b" c                    | a b        | c       |         |         |
| "a "b c                    | a b        | c       |         |         |
| 'a b c'                    | a b c      |         |         |         |
| a' b' c                    | a b        | c       |         |         |
| 'a 'b c                    | a b        | c       |         |         |
| "a b c"'d e'               | a b cd e   |         |         |         |

3. Single-quote `'` and double-quotes `"` characters:
   1. Literal `'` or `"` characters must be escaped with `\` when outside a string. The character does not starts/ends a string.
   2. Single quote  characters inside a double quotes string must be interpreted literally and does not requires escaping.
   3. Double quotes characters inside a single quote  string must be interpreted literally and does not requires escaping.
   4. Double quote  characters inside a double quotes string must be escaped with `\` to be properly interpreted.
   5. Single quote  characters inside a single quote  string **CAN NOT** be escaped with `\`. The single-quote string must be ended, joined with an escaped single-quote and reopened to be properly interpreted.

| Command Line                                       | argv[1]                  | argv[2] | argv[3] |
|----------------------------------------------------|--------------------------|---------|---------|
| a &bsol;"b c                                       | a                        | "b      | c       |
| "a &bsol;"b" c                                     | a "b                     | c       |         |
| a &bsol;'b c                                       | a                        | 'b      | c       |
| 'a '&bsol;''b' c                                   | a 'b                     | c       |         |
| "McDonald's I'm Lovin' It"                         | McDonald's I'm Lovin' It |         |         |
| 'McDonald'&bsol;''s I'&bsol;''m Lovin'&bsol;'' It' | McDonald's I'm Lovin' It |         |         |
| 'Toys "R" Us'                                      | Toys "R" Us              |         |         |
| "Toys &bsol;"R&bsol;" Us"                          | Toys "R" Us              |         |         |

4. The backslash (`\`) character:
   1. The character `\` must be escaped with `\` (resulting in `\\`) when outside a string.
   2. Characters escaped with `\` are literal characters.

| Command Line               | argv[1]        | argv[2] | argv[3] |
|----------------------------|----------------|---------|---------|
| a&bsol;&bsol;b             | a&bsol;b       |         |         |
| a&bsol;&bsol;&bsol;b       | a&bsol;b       |         |         |
| a&bsol;&bsol;&bsol;&bsol;b | a&bsol;&bsol;b |         |         |
| a&bsol;bc                  | abc            |         |         |

4. 3. The character `\` does not requires escaping when inside a double-quotes string.
   4. Two consecutive `\` characters in a double-quotes string must be interpreted as a literal `\` character.
   5. The character `\` does not requires escaping when inside a single-quote string.

| Command Line                       | argv[1]                          | argv[2] | argv[3] |
|------------------------------------|----------------------------------|---------|---------|
| "a&bsol;b&bsol;c"                  | a&bsol;b&bsol;c                  |         |         |
| "a&bsol;&bsol;b"                   | a&bsol;b                         |         |         |
| "a&bsol;&bsol;&bsol;b"             | a&bsol;&bsol;b                   |         |         |
| "a&bsol;&bsol;&bsol;&bsol;b"       | a&bsol;&bsol;b                   |         |         |
| "a&bsol;&bsol;&bsol;&bsol;&bsol;b" | a&bsol;&bsol;&bsol;b             |         |         |
| 'a&bsol;b&bsol;c'                  | a&bsol;b&bsol;c                  |         |         |
| 'a&bsol;&bsol;b'                   | a&bsol;&bsol;b                   |         |         |
| 'a&bsol;&bsol;&bsol;b'             | a&bsol;&bsol;&bsol;b             |         |         |
| 'a&bsol;&bsol;&bsol;&bsol;b'       | a&bsol;&bsol;&bsol;&bsol;b       |         |         |
| 'a&bsol;&bsol;&bsol;&bsol;&bsol;b' | a&bsol;&bsol;&bsol;&bsol;&bsol;b |         |         |

5. Shell characters:
   1. The shell characters `&`,`|`,`(`,`)`,`<`,`>` or `*` does not require escaping when inside a string.

| Command Line   | argv[1]    | argv[2] | argv[3] |
|----------------|------------|---------|---------|
| "a & b" c      | a & b      | c       |         |
| "a &vert; b" c | a &vert; b | c       |         |
| "a ( b" c      | a ( b      | c       |         |
| "a ) b" c      | a ) b      | c       |         |
| "a < b" c      | a < b      | c       |         |
| "a > b" c      | a > b      | c       |         |
| "a * b" c      | a * b      | c       |         |
| 'a & b' c      | a & b      | c       |         |
| 'a &vert; b' c | a &vert; b | c       |         |
| 'a ( b' c      | a ( b      | c       |         |
| 'a ) b' c      | a ) b      | c       |         |
| 'a < b' c      | a < b      | c       |         |
| 'a > b' c      | a > b      | c       |         |
| 'a * b' c      | a * b      | c       |         |

5. 2. The shell characters `&`,`|`,`(`,`)`,`<`,`>` or `*` must be escapsed with `\` when outside a string.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| a &bsol;< b  | a       | <       | b       |
| a &bsol;& b  | a       | &       | b       |
| a &bsol;* b  | a       | *       | b       |
| a&bsol;<b    | a<b     |         |         |
| a&bsol;&b    | a&b     |         |         |
| a&bsol;*b    | a*b     |         |         |

5. 3. The shell characters `$`, and `` ` `` (backtick) are special shell characters and must *always* be escapsed with `\`.

| Command Line      | argv[1]   | argv[2] | argv[3] |
|-------------------|-----------|---------|---------|
| "a &bsol;$d b" c  | a $d b    | c       |         |
| sum=&bsol;$42     | sum=$42   |         |         |
| "sum = &bsol;$20" | sum = $20 |         |         |

6. Empty arguments:
   1. Empty arguments must be specified with `""` and must be surrounded by argument delimiters or located at the start/or end of the command line.
   2. Empty arguments can also be specified with `''`.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| a "" b       | a       | [empty] | b       |
| "" a b       | [empty] | a       | b       |
| a b ""       | a       | b       | [empty] |
| a '' b       | a       | [empty] | b       |
| '' a b       | [empty] | a       | b       |
| a b ''       | a       | b       | [empty] |

7. All other characters must be read as plain text.

## Combinations ##

This section show multiple other examples which combines multiple rules together.

| Rules    | Command Line              | argv[1]     | argv[2] | argv[3] |
|----------|---------------------------|-------------|---------|---------|
| 4.1, 5.1 | "a &bsol;* b" c           | a &bsol;* b | c       |         |
| 2, 3     | 'abc'&bsol;''def'         | abc'def     |         |         |
| 3, 4.1   | "a&bsol;&bsol;&bsol;"b c" | a&bsol;"b c |         |         |
