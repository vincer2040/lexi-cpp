#include "parser.hh"
#include "lexitype.hh"
#include <variant>

Parser::Parser(Lexer& l) : l(l) {
    this->next_token();
    this->next_token();
}

LexiType Parser::parse() {
    LexiType res = {LexiTypeT::Invalid, std::monostate()};
    switch (this->cur.type) {
    case TokenT::BulkType: {
        if (!this->expect_peek(TokenT::Len)) {
            return res;
        }
        if (!this->expect_peek(TokenT::Retcar)) {
            return res;
        }
        if (!this->expect_peek(TokenT::NewL)) {
            return res;
        }
        if (!this->expect_peek(TokenT::Bulk)) {
            return res;
        }
        std::string s = std::get<std::string>(cur.literal);
        if (!this->expect_peek(TokenT::Retcar)) {
            return res;
        }
        if (!this->expect_peek(TokenT::NewL)) {
            return res;
        }
        res.type = LexiTypeT::Bulk;
        res.data = s;
        this->next_token();
    } break;
    default:
        break;
    }
    return res;
}

bool Parser::peek_tok_is(TokenT type) { return type == this->peek.type; }

bool Parser::cur_tok_is(TokenT type) { return type == this->cur.type; }

bool Parser::expect_peek(TokenT type) {
    if (this->peek_tok_is(type)) {
        this->next_token();
        return true;
    } else {
        return false;
    }
}

void Parser::next_token(void) {
    this->cur = this->peek;
    this->peek = this->l.next_token();
}
