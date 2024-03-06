# Clox

This is an implementation of the clox language, following the book "Crafting Interpreters".

## TODO

- Store lines as run-length encoding instead of a 1 to 1 array where lines\[offset\] contains the line for the instruction in offset.

- Add Operations for '!=' '<=' and '>='

- Use memcmp in the values_equal function, for that it must be guaranteed that
all the padding bits in the Value struct are 0

- Clang-format options:
AllowShortCaseLabelsOnASingleLine
AllowShortBlocksOnASingleLine
AllowShortIfStatementsOnASingleLine
