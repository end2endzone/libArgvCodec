# Unit tests #



## Windows ##

The following table shows all the command lines that were used as a baseline for creating unit tests for the Windows platform.

| Command Line           | argv[1]         | argv[2] | argv[3] |
|------------------------|-----------------|---------|---------|
| aaa"                   | aaa             |         |         |
| aaa\"                  | aaa"            |         |         |
| aaa\\"                 | aaa\            |         |         |
| aaa\\\"                | aaa\"           |         |         |
| "aaa\"                 | aaa"            |         |         |
| "aaa\\"                | aaa\            |         |         |
| "aaa\\\"               | aaa\"           |         |         |
| a"bc                   | abc             |         |         |
| a"bc\                  | abc\            |         |         |
| a"bc\\                 | abc\\           |         |         |
| a"bc\\\                | abc\\\          |         |         |
| a"bc\"d                | abc"d           |         |         |
| a"bc\\ ddd" e          | abc\\ ddd       | e       |         |
| a"bc\"\ ddd" e         | abc"\ ddd       | e       |         |
| ab\\c d                | ab\\c           | d       |         |
| ab\\ c                 | ab\\            | c       |         |
| \\ab c                 | \\ab            | c       |         |
| a ""bc\"o" e"          | a               | bc"o e  |         |
| a "bc\"o" e"           | a               | bc"o    | e       |
| "a"""b                 | a"b             |         |         |
| "a\"b"                 | a"b             |         |         |
| "a""" b"               | a"              | b       |         |
| """                    | "               |         |         |
| """"                   | "               |         |         |
| """""                  | ""              |         |         |
| """"""                 | ""              |         |         |
| ^"ab cd\^"^&echo foo^" | ab cd"&echo foo |         |         |
| "ab cd\^"^&echo foo^"  | ab cd\^&echo    | foo     |         |
| ^"ab cd\^" ee"         | ab cd" ee       |         |         |
| "ab cd\" ee"           | ab cd" ee       |         |         |
| "ab cd\^" ee"          | ab cd\^         | ee      |         |
| "a^b" c                | a^b             | c       |         |
| ^"a^b" c               | ab              | c       |         |
| ^"ab^" c               | ab              | c       |         |
| ^"ab^"" c              | ab" c           |         |         |
| a \bb c                | a               | \bb     | c       |
| a \"bb c" d            | a               | "bb     | c d     |
| a ^"bb^" c" d          | a               | bb      | c d     |
| a \"b c                | a               | "b      | c       |
| a""b c                 | ab              | c       |         |
| a\"b c                 | a"b             | c       |         |
| "a""b" c               | a"b             | c       |         |
| "a\"b" c               | a"b             | c       |         |
| a ^"" b                | a               | [empty] | b       |
| a ^"^" b               | a               | [empty] | b       |
| ^"" a                  |                 | a       |         |
| ^"^" a                 |                 | a       |         |
| ^"test\^"^&whoami^"    | test"&whoami    |         |         |
| ^"test\\^"^&whoami^"   | test\&whoami    |         |         |
| ^"test\"^&whoami^"     | test"^&whoami^  |         |         |
| a\\\\"b                | a\\b            |         |         |
| ^"a\\\\"b"             | a\\b            |         |         |
| ^"a\^\^\\"b"           | a\\b            |         |         |
| ^"a\^\\^\"b"           | a\\b            |         |         |
| ^"a\^\\\^"b"           | a\\b            |         |         |
