#include <iostream>

#include <readline/readline.h>
#include <readline/history.h>

#include "Parser.h" 
#include "Exceptions.h"

int main()
{
  char* input;
  while ((input = readline("> ")) != nullptr) {
    if (strlen(input) > 0) {
      add_history(input);
      try
      {
        kcalc::Lexer lexer(input);
        kcalc::Parser parser(lexer);
        std::unique_ptr<kcalc::AstObject> result =
          parser.parse();
        if (result)
          std::cout << *result << std::endl;
        else 
          std::cout << "<<nullptr>>" << std::endl;
      }
      catch(const kcalc::Exception& e)
      {
        std::cout << e.what() << std::endl;
      }
    }
    free(input);
  }
  return 0;
}
