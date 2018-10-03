#include "Repl.h"

#include <readline/readline.h>
#include <readline/history.h> 

#include <sstream>

namespace kcalc
{

const std::string& Prompt::get()
{
  std::stringstream stream; 
  ++m_counter;
  stream << " [" << m_counter << "] ";
  m_string = stream.str();
  return m_string;
}  

void Repl::run(
    void (&callBack)(Prompt&, const char *)) 
{
  char * input;
  while ((input = readline(
          m_prompt.get().c_str()))
      != nullptr) {
    if (strlen(input) > 0) {
      add_history(input);
      callBack(m_prompt, input);
    }
    free(input); 
  }
}

} /* namespace kcalc */

