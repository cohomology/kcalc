#include "Token.h"

namespace kcalc
{

std::ostream& operator<<(std::ostream& out, TokenKind kind)
{ 
  switch(kind)
  {
#define DEFINE_TOKENKIND(a) \
    case TokenKind::a:      \
      out << #a;            \
      break;
#include "TokenKind.h"
#undef DEFINE_TOKENKIND
  }
  return out;
} 

} /* namespace kcalc */
