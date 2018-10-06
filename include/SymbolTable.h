#ifndef KCALC_SYMBOLTABLE_H
#define KCALC_SYMBOLTABLE_H 

#include <map>

namespace kcalc 
{

class SymbolTable
{
public:
  void insert(std::string variableName, 
      const AstObject& object)
  {
    auto it = m_symbols.find(variableName);
    if (it != m_symbols.end())
      m_symbols.erase(it);
    m_symbols.insert(std::make_pair(
          variableName, 
          std::move(object.clone())));
  }
  std::unique_ptr<AstObject> retrieve(
      std::string variableName)
  {
    auto it = m_symbols.find(variableName);
    if (it != m_symbols.end())
    {
      assert(it->second);
      return it->second->clone();
    }
    return nullptr;
  }
private:
  std::map<std::string, 
    std::unique_ptr<AstObject> > m_symbols;
};

} /* namespace kcalc */

#endif // KCALC_SYMBOLTABLE_H  
