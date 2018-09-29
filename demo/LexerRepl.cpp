#include <iostream>

#include <readline/readline.h>
#include <readline/history.h>

#include "Lexer.h" 

int main()
{
  char* input;
  while ((input = readline("> ")) != nullptr) {
    if (strlen(input) > 0) {
      add_history(input);
      kcalc::Lexer lexer(input);
      for (auto token : lexer)
      {
        std::cout << token << std::endl;
      } 
    }
    free(input);
  }
  return 0;
}
