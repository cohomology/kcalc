#include <iostream>

#include "Parser.h" 
#include "Exceptions.h"
#include "Repl.h"
#include "SymbolTable.h"
#include "SemanticAnalyzer.h"

static void renderError(
    const kcalc::ParseError& e,
    unsigned int promptLength)
{
  const std::optional<kcalc::Token> token 
    = e.token();
  for (unsigned int i = 0; i < (token ? 
        token->offset() : 0) + promptLength; ++i)
  {
    std::cout << "_";
  }
  std::cout << "^" << std::endl;
  std::cout << e.what() << std::endl;
}

static void kcalcRepl(
    kcalc::SymbolTable& symbolTable,
    kcalc::SemanticAnalyzer& analyzer,
    kcalc::Prompt& prompt,
    const char * input)
{
  try
  {
    kcalc::Lexer lexer(input);
    kcalc::Parser parser(lexer);
    std::unique_ptr<kcalc::AstObject> result =
      parser.parse();
    if (result)
    {
      result->accept(analyzer);
      if (result->kind() != kcalc::ObjectKind::Assignment)
      {
        std::unique_ptr<kcalc::Expression> eval =
          result->eval(symbolTable); 
        if (eval)
        {
          std::cout 
            << eval->to_string()
            << std::endl;
        }
      }
    }
  }
  catch(const kcalc::ParseError& e)
  {
    renderError(e, prompt.length());
  }
  catch(const kcalc::Exception& e)
  {
    std::cout << e.what() << std::endl;
  }
}

int main()
{
  using namespace std::placeholders;
  kcalc::SymbolTable symbolTable;
  kcalc::SemanticAnalyzer analyzer(symbolTable);
  kcalc::Repl repl;
  repl.run(std::bind(&kcalcRepl, std::ref(symbolTable), 
        std::ref(analyzer), _1, _2));
  return 0;
} 
