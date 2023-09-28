#include "parser.hh"
#include "lexitype.hh"
#include <string>
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
        std::shared_ptr<std::string> s =
            std::get<std::shared_ptr<std::string>>(cur.literal);
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
    case TokenT::Int: {
        std::uint64_t tmp = 0;
        std::int64_t data;
        std::size_t i;
        std::uint8_t shift = 56;
        std::vector<std::uint8_t> literal;
        literal = std::get<std::vector<std::uint8_t>>(this->cur.literal);
        for (i = 0; i < 8; ++i, shift -= 8) {
            uint8_t at = literal[i];
            tmp |= (std::uint64_t)(at << shift);
        }
        if (tmp <= 0x7fffffffffffffffu) {
            data = tmp;
        } else {
            data = -1 - (long long int)(0xffffffffffffffffu - tmp);
        }
        if (!this->expect_peek(TokenT::Retcar)) {
            return res;
        }
        if (!this->expect_peek(TokenT::NewL)) {
            return res;
        }
        res.type = LexiTypeT::Int;
        res.data = data;
        this->next_token();
    } break;
    case TokenT::ArrayType: {
        if (!this->expect_peek(TokenT::Len)) {
            return res;
        }
        std::shared_ptr<std::string> s_len =
            std::get<std::shared_ptr<std::string>>(this->cur.literal);
        std::size_t i, len = std::stoull(*s_len);
        if (!this->expect_peek(TokenT::Retcar)) {
            return res;
        }
        if (!this->expect_peek(TokenT::NewL)) {
            return res;
        }
        this->next_token();

        std::vector<LexiType> data;

        data.reserve(len);

        for (i = 0; i < len; ++i) {
            LexiType at = this->parse();
            data.push_back(at);
        }

        res.type = LexiTypeT::Array;
        res.data = data;
    } break;
    case TokenT::Simple: {
        std::shared_ptr<std::string> s =
            std::get<std::shared_ptr<std::string>>(this->cur.literal);
        if (!this->expect_peek(TokenT::Retcar)) {
            return res;
        }
        if (!this->expect_peek(TokenT::NewL)) {
            return res;
        }
        this->next_token();
        res.type = LexiTypeT::Simple;
        res.data = s;
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
