#include <Lexer.h>
#include <iostream>

int main()
{
  for(;;)
  {
    std::string input;
    std::cout << "> ";
    std::getline(std::cin, input);
    kcalc::Lexer lexer(input.c_str());
    for (auto token : lexer)
    {
      std::cout << token << std::endl;
    }
  }
  return 0;
}
