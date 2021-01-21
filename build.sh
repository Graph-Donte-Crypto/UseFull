find . -name '*.hpp' | grep -vFf .buildignore | xargs clang -S -Wall -Wpedantic -Wextra -Weffc++ -Werror -std=c++20 -Wfatal-errors -fsyntax-only -Og
