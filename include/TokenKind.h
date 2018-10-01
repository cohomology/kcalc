#if defined(DEFINE_TOKENKIND_MANUAL) && \
    defined(DEFINE_TOKENKIND_COMPLEX) && \
    defined(DEFINE_TOKENKIND_SIMPLE)
DEFINE_TOKENKIND_MANUAL(Unknown)

DEFINE_TOKENKIND_COMPLEX(Identifier, "[A-Za-z_][A-Za-z0-9_]*")
DEFINE_TOKENKIND_COMPLEX(Number, "((0(\\.[0-9]*)?)|(([1-9][0-9]*)(\\.([0-9]*))?))([Ee][+-]?[1-9][0-9]*)?i?")
DEFINE_TOKENKIND_COMPLEX(Newline, "(\\r\\n)|(\\n)")
DEFINE_TOKENKIND_COMPLEX(Whitespace, "[\\s]+")  

DEFINE_TOKENKIND_SIMPLE(Equals, '=')
DEFINE_TOKENKIND_SIMPLE(Plus, '+')
DEFINE_TOKENKIND_SIMPLE(Minus, '-')
DEFINE_TOKENKIND_SIMPLE(Asterisk, '*')
DEFINE_TOKENKIND_SIMPLE(Slash, '/')
DEFINE_TOKENKIND_SIMPLE(LeftParen, '(')
DEFINE_TOKENKIND_SIMPLE(RightParen, ')')
DEFINE_TOKENKIND_SIMPLE(LeftBracket, '{')
DEFINE_TOKENKIND_SIMPLE(RightBracket, '}')
DEFINE_TOKENKIND_SIMPLE(Comma, ',')
DEFINE_TOKENKIND_SIMPLE(Power, '^') 

DEFINE_TOKENKIND_MANUAL(EndOfInput) // must be last element
#else
#  ifdef DEFINE_TOKENKIND
#    define DEFINE_TOKENKIND_MANUAL(a)     DEFINE_TOKENKIND(a)
#    define DEFINE_TOKENKIND_COMPLEX(a,b)  DEFINE_TOKENKIND(a)
#    define DEFINE_TOKENKIND_SIMPLE(a,b)   DEFINE_TOKENKIND(a)
#    include "TokenKind.h"
#    undef DEFINE_TOKENKIND_SIMPLE
#    undef DEFINE_TOKENKIND_MANUAL
#    undef DEFINE_TOKENKIND_COMPLEX  
#  endif
#endif
