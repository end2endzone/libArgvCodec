# Command Line Rules #



## Linux ##

The following list shows all the special rules implemented by the library for properly:
* parsing a command line into arguments and,
* escaping arguments into a valid command line.

The list of rules applies to the Terminal environment of the Linux platform.

1. [space] or tabs characters are argument delimiters/separators but *ONLY* when outside a string.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| a b c        | a       | b       | c       |

2. If an argument contains [space] or tabs characters, it must be enclosed in a string to form a single argument.
Double quotes character `"` starts/ends a string. The `"` character is omitted from the argument.
Single quote character `'` also starts/ends a string. The `'` character is omitted from the argument.
If a string is juxtaposed to another string or argument (not separated by a delimiter character), they are part of the same argument.

| Command Line               | argv[1]    | argv[2] | argv[3] | argv[4] |
|----------------------------|------------|---------|---------|---------|
| "a b c"                    | a b c      |         |         |         |
| a" b" c                    | a b        | c       |         |         |
| "a "b c                    | a b        | c       |         |         |
| 'a b c'                    | a b c      |         |         |         |
| a' b' c                    | a b        | c       |         |         |
| 'a 'b c                    | a b        | c       |         |         |
| "a b c"'d e'               | a b c d e  |         |         |         |

3. Plain `"` or `'` characters must be escaped with `\` and does not starts/ends a string. (If not escaped, they act as rule #2).
Inside a double-quotes string, single-quote  characters must be interpreted literally and does not requires escaping.
Inside a single-quote  string, double-quotes characters must be interpreted literally and does not requires escaping.
Inside a double-quotes string, double-quote  characters must be escaped with `\` to be properly interpreted.
Inside a single-quote  string, single-quote  characters must be escaped with `\` to be properly interpreted.

| Command Line                  | argv[1]                  | argv[2] | argv[3] |
|-------------------------------|--------------------------|---------|---------|
| a \"b c                       | a                        | "b      | c       |
| "a \"b" c                     | a "b                     | c       |         |
| a \'b c                       | a                        | 'b      | c       |
| 'a \'b' c                     | a 'b                     | c       |         |
| "McDonald's I'm Lovin' It"    | McDonald's I'm Lovin' It |         |         |
| 'Toys "R" Us'                 | Toys "R" Us              |         |         |
| "Toys \"R\" Us"               | Toys "R" Us              |         |         |
| 'McDonald\'s I\'m Lovin\' It' | McDonald's I'm Lovin' It |         |         |

4. Plain `\` character must be escaped with `\\`.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| a\\b         | a\b     |         |         |
| "a\\b"       | a\b     |         |         |
| a\\"b c"     | a\b c   |         |         |
| "a\\\"b c"   | a\"b c  |         |         |

5. The following characters are special shell characters:   &,|,(,),<,>,!,*,$, or \`
    1. The shell characters &,|,(,),<,> or ! does not requires escaping when inside a string.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| "a < b" c    | a < b   | c       |         |
| "a & b" c    | a & b   | c       |         |

5. 2. The shell characters &,|,(,),<,> or ! must be escapsed with `\` when outside a string.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| a \< b       | a       | <       | b       |
| a \& b       | a       | &       | c       |
| a\<b         | a<b     |         |         |
| a\&b         | a&b     |         |         |

5. 3. The shell characters *,$, or \` must **always** be escapsed with `\`. (inside or outside a string)

| Command Line | argv[1]   | argv[2] | argv[3] |
|--------------|-----------|---------|---------|
| "a \* b" c   | a * b     | c       |         |
| "a \$d b" c  | a $d b    | c       |         |
| sum=\$42     | sum=$42   |         |         |
| "sum = \$20" | sum = $20 |         |         |

5. 4. The words `true` and `false` are special values and must be considered as shell reserved words. These words must be enclosed as string to be properly used as arguments.

| Command Line        | argv[1] | argv[2] | argv[3] |
|---------------------|---------|---------|---------|
| "true" "&&" "false" | true    | &&      | false   |
| "false"             | false   |         |         |
| The true meaning    | The     | true    | meaning |

6. Empty arguments must be specified with `""` and must be enclosed by argument delimiters or located at the start/end of the command line. Empty arguments can also be specified with `''`.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| a "" b       | a       | [empty] | c       |
| "" a b       | [empty] | a       | b       |
| a b ""       | a       | b       | [empty] |
| a '' b       | a       | [empty] | b       |
| '' a b       | [empty] | a       | b       |
| a b ''       | a       | b       | [empty] |

7. All other characters must be read as plain text.
