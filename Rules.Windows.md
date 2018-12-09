# Command Line Rules #



## Windows ##

The following list shows all the special rules implemented by the library for properly:
* parsing a command line into arguments and,
* escaping arguments into a valid command line.

The list of rules applies to the Command Prompt environment of the Windows platform.

1. [space] or tabs characters are argument delimiters/separators but *ONLY* when outside a string and outside a caret-string.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| a b c        | a       | b       | c       |

1. 1. Double quotes character `"` starts/ends a string. The `"` character is omitted from the argument. Strings **may** not always be ended (the end of the command line may be hit without the end-of-string character). If a string is juxtaposed to another argument (not separated by a delimiter character), they are part of the same argument.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| "a b c"      | a b c   |         |         |
| a" b" c      | a b     | c       |         |
| "a "b c      | a b     | c       |         |

2. Plain `"` character must be escaped with `\"` (or escaped with `""`) and does not ends the string. (If not escaped, they act as rule #1). Characters escaped with `\"` can be seen inside or outside strings but characters escaped with `""` can **ONLY** be seen inside a string.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| a \"b c      | a       | "b      | c       |
| "a \"b" c    | a "b    | c       |         |
| "a ""b" c    | a "b    | c       |         |
| a ""b c      | a       | b       | c       |
| """"         | "       |         |         |

2. 1. Plain `"` character must be escaped with `\^"` when inside a caret-string.

Note: The character sequence `\^"` can also be visible outside a string.

| Command Line  | argv[1] | argv[2] | argv[3] |
|---------------|---------|---------|---------|
| ^"a \^" b^" c | a " b   | c       |         |
| ^"a \" b^" c  | a " b^  | c       |         |
| a b\^"c       | a       | b"c     |         |

3. Plain `\` character must be escaped with `\\` **ONLY** if they precedes a `"` character or `^"` character sequence. (string or caret-string termination).

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| a\\b         | a\\b    |         |         |
| "a\\b"       | a\\b    |         |         |
| a\\"b c      | a\b c   |         |         |
| a\\"b c"     | a\b c   |         |         |
| "a\\\"b c"   | a\"b c  |         |         |
| a\\^"b c     | a\b c   |         |         |
| a\\^"b c"    | a\b c   |         |         |
| "a\\\^"b c"  | a\\\^b  | c       |         |

4. The character sequence `^"` starts/ends a caret-string. Caret-strings are different than normal strings. Caret-strings may be ended with an unescaped `"` character. Caret-string may not be ended. If a caret-string is juxtaposed to another argument (not separated by a delimiter character), they are part of the same argument.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| ^"a b^" c    | a b     | c       |         |
| ^"a b" c     | a b     | c       |         |
| ^"a b^"c     | a bc    |         |         |
| a^"b c^"     | ab c    |         |         |

5. The following characters are special shell characters:   &,<,>,(,),| or %
    1. Shell characters must be read as plain text when inside a string.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| "a < b" c    | a < b   | c       |         |
| "a ^< b" c   | a ^< b  | c       |         |

5. 2. Plain shell characters must be escaped with ^ when inside a caret-string or outside a string.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| a ^< b       | a       | <       | b       |
| ^"a ^< b^" c | a < b   | c       |         |

For examples, the following command line examples are malformed and produces error:

* The command `^"a < b^" c` returns `The system cannot find the file specified`. The character `<` is a shell character which tries to pipe the file named `b c` into `a`.

* The command `^"a & b^" c` returns `'b"' is not recognized as an internal or external command, operable program or batch file`. The `&` character is a shell character which tries to juxtapose two different commands.

* The command `^"a &whoami^"` returns `%USERDOMAIN%\%USERNAME%` (expanded). The `&` character is a shell character which makes the command `whoami` execute. This can be used as an attack. 



5. 3. Non-shell characters that are escaped with `^` when inside a caret-string or outside a string must be read as plain characters.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| ^a^b ^c      | ab      | c       |         |
| "^a^b" ^c    | ^a^b    | c       |         |

6. Empty arguments must be specified with `""` and must be enclosed by argument delimiters or located at the start or the end of the command line. Empty arguments can also be specified with `^"^"` or a combination of the two.

| Command Line | argv[1] | argv[2] | argv[3] |
|--------------|---------|---------|---------|
| a "" b       | a       | [empty] | c       |
| "" a b       | [empty] | a       | b       |
| a b ""       | a       | b       | [empty] |
| a ^"^" b     | a       | [empty] | b       |
| ^"^" a b     | [empty] | a       | b       |
| a b ^"^"     | a       | b       | [empty] |
| a ^"" b      | a       | [empty] | b       |
| ^"" a b      | [empty] | a       | b       |
| a b ^""      | a       | b       | [empty] |
   
7. Two juxtaposed strings (not separated by a delimiter character), will insert a plain `"` character between each other. This is valid for normal strings, caret-strings, or any combinations.

| Command Line   | argv[1]  | argv[2] | argv[3]           |
|----------------|----------|---------|-------------------|
| "a b""c d"     | a b"c d  |         | (as with rule #2) |
| ^"a b^"^"c d^" | a b"c d  |         |                   |
| ^"a b^""c d"   | a b"c d  |         |                   |
| "a b"^"c d^"   | a b"c d  |         |                   |
| "a b^"^"c d"   | a b^"c d |         |                   |

8. All other characters must be read as plain text.

9. Exceptions:
    1. The character sequence `\"` inside a caret-string. The sequence in caret-string should be read as `[close caret-string]` and `[open string]` and plain `"` character.

| Command Line           | argv[1]            | argv[2] | argv[3] |
|------------------------|--------------------|---------|---------|
| ^"test\"<test&whoami^" | test"<test&whoami^ |         |         |

Note: This formatting is against rule 2.1 but **may** be explained by rule 7.
