#ifndef KCALC_SYMBOLTABLE_H
#define KCALC_SYMBOLTABLE_H 

#include <map>

namespace kcalc 
{

class SymbolTable
{
public:
  void insert(const std::string& variableName, 
      const Expression& object)
  {
    auto it = m_symbols.find(variableName);
    if (it != m_symbols.end())
      m_symbols.erase(it);
    m_symbols.insert(std::make_pair(
          variableName, 
          std::move(object.cloneExpression())));
  }
  std::unique_ptr<Expression> retrieve(
      const std::string& variableName)
  {
    auto it = m_symbols.find(variableName);
    if (it != m_symbols.end())
    {
      assert(it->second);
      return it->second->cloneExpression();
    }
    return nullptr;
  }
private:
  std::map<std::string, 
    std::unique_ptr<Expression> > m_symbols;
};

} /* namespace kcalc */

#endif // KCALC_SYMBOLTABLE_H  
