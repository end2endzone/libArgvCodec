# Command Line Rules #



## Linux ##

The following list shows all the special rules implemented by the library for properly:
* parsing a command line into arguments and,
* escaping arguments into a valid command line.

The list of rules applies to the Terminal environment (bash) of the Linux platform.

1. [space] or tabs characters are argument delimiters/separators but *ONLY* when outside a string.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| a b c        | a       | b       | c       |

2. If an argument contains [space] or tabs characters, it must be enclosed in a string to form a single argument.
Double quotes character `"` starts/ends a string. The `"` character is omitted from the argument.
Single quote character `'` also starts/ends a string. The `'` character is omitted from the argument.
If a string is juxtaposed to another string or argument (not separated by a delimiter character), they form the same argument.

| Command Line               | argv[1]    | argv[2] | argv[3] | argv[4] |
|----------------------------|------------|---------|---------|---------|
| "a b c"                    | a b c      |         |         |         |
| a" b" c                    | a b        | c       |         |         |
| "a "b c                    | a b        | c       |         |         |
| 'a b c'                    | a b c      |         |         |         |
| a' b' c                    | a b        | c       |         |         |
| 'a 'b c                    | a b        | c       |         |         |
| "a b c"'d e'               | a b cd e   |         |         |         |

3. Plain `"` or `'` characters must be escaped with `\` and does not starts/ends a string. (If not escaped, they act as rule #2).
Single-quote  characters inside a double-quotes string,  must be interpreted literally and does not requires escaping.
Double-quotes characters inside a single-quote  string,  must be interpreted literally and does not requires escaping.
Double-quote  characters inside a double-quotes string,  must be escaped with `\` to be properly interpreted.
Single-quote  characters inside a single-quote  string,  **CAN NOT** be escaped with `\`. The single-quote string must be ended, joined with an escaped single-quote and reopened to be properly interpreted.

| Command Line                        | argv[1]                  | argv[2] | argv[3] |
|-------------------------------------|--------------------------|---------|---------|
| a \"b c                             | a                        | "b      | c       |
| "a \"b" c                           | a "b                     | c       |         |
| a \'b c                             | a                        | 'b      | c       |
| 'a '\''b' c                         | a 'b                     | c       |         |
| "McDonald's I'm Lovin' It"          | McDonald's I'm Lovin' It |         |         |
| 'McDonald'\''s I'\''m Lovin'\'' It' | McDonald's I'm Lovin' It |         |         |
| 'Toys "R" Us'                       | Toys "R" Us              |         |         |
| "Toys \"R\" Us"                     | Toys "R" Us              |         |         |

4. The character `\` must be escaped with `\\` when outside a string. Characters escaped with `\` are literal characters.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| a\\b         | a\b     |         |         |
| a\\\b        | a\b     |         |         |
| a\\\\b       | a\\b    |         |         |
| a\bc         | abc     |         |         |

4. 1. The character `\` does not requires escaping when inside a double-quotes string.
      However, two consecutives `\` characters in a double-quotes string must be interpreted as a literal `\` character.
      The character `\` does not requires escaping when inside a single-quote string.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| "a\b\c"      | a\b\c   |         |         |
| "a\\b"       | a\b     |         |         |
| "a\\\b"      | a\\b    |         |         |
| "a\\\\b"     | a\\b    |         |         |
| "a\\\\\b"    | a\\\b   |         |         |
| 'a\b\c'      | a\b\c   |         |         |
| 'a\\b'       | a\\b    |         |         |
| 'a\\\b'      | a\\\b   |         |         |
| 'a\\\\b'     | a\\\\b  |         |         |
| 'a\\\\\b'    | a\\\\\b |         |         |

5. The following characters are special shell characters:   &,|,(,),<,>,*
    1. The shell characters does not requires escaping when inside a string.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| "a & b" c    | a & b   | c       |         |
| "a \| b" c   | a \| b  | c       |         |
| "a ( b" c    | a ( b   | c       |         |
| "a ) b" c    | a ) b   | c       |         |
| "a < b" c    | a < b   | c       |         |
| "a > b" c    | a > b   | c       |         |
| "a * b" c    | a * b   | c       |         |
| 'a & b' c    | a & b   | c       |         |
| 'a \| b' c   | a \| b  | c       |         |
| 'a ( b' c    | a ( b   | c       |         |
| 'a ) b' c    | a ) b   | c       |         |
| 'a < b' c    | a < b   | c       |         |
| 'a > b' c    | a > b   | c       |         |
| 'a * b' c    | a * b   | c       |         |

5. 2. The shell characters &,|,(,),<,> or * must be escapsed with `\` when outside a string.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| a \< b       | a       | <       | b       |
| a \& b       | a       | &       | b       |
| a \* b       | a       | *       | b       |
| a\<b         | a<b     |         |         |
| a\&b         | a&b     |         |         |
| a\*b         | a*b     |         |         |

5. 3. The shell characters $, and \` (backticks) are special shell characters and must **always** be escapsed with `\`. (inside or outside a string)

| Command Line | argv[1]   | argv[2] | argv[3] |
|--------------|-----------|---------|---------|
| "a \$d b" c  | a $d b    | c       |         |
| sum=\$42     | sum=$42   |         |         |
| "sum = \$20" | sum = $20 |         |         |

6. Empty arguments must be specified with `""` and must be enclosed by argument delimiters or located at the start/end of the command line. Empty arguments can also be specified with `''`.

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

This section show multiple other examples which combines multiple rules toghetter.

| Rules    | Command Line | argv[1] | argv[2] | argv[3] |
|----------|--------------|---------|---------|---------|
| 4.1, 5.1 | "a \* b" c   | a \* b  | c       |         |
| 2, 3     | 'abc'\''def' | abc'def |         |         |
| 3, 4.1   | "a\\\"b c"   | a\"b c  |         |         |
