#pragma once

#include "lexer.hh"
#include "lexitype.hh"

class Parser {
  public:
    Parser(Lexer& l);
    LexiType parse(void);

  private:
    Lexer& l;
    Token cur;
    Token peek;
    void next_token(void);
    bool peek_tok_is(TokenT type);
    bool cur_tok_is(TokenT type);
    bool expect_peek(TokenT type);
};
