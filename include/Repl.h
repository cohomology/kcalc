#ifndef KCALC_REPL_H
#define KCALC_REPL_H 

#include <string>
#include <functional>

namespace kcalc 
{

class Prompt
{
public:
  Prompt()
    : m_counter{0}, m_string{}
  { }
  const std::string& get();
  unsigned int length() const
  { return m_string.length(); }
private:
  unsigned int m_counter;
  std::string m_string;
}; 

class Repl
{
public:
  Repl()
    : m_prompt{}
  { }
  void run(const std::function<
      void (Prompt&, const char *)>&);
private:
  Prompt m_prompt;
}; 

} /* namespace kcalc */

#endif // KCALC_REPL_H  
