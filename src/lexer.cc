#include "lexer.hh"
#include <variant>

static bool is_digit(std::uint8_t ch);
static bool is_letter(std::uint8_t ch);

Lexer::Lexer(std::uint8_t* input, std::size_t input_len) {
    this->input = input;
    this->input_len = input_len;
    this->pos = 0;
    this->ch = 0;
    this->read_char();
}

Token Lexer::next_token(void) {
    Token tok = {TokenT::Illegal, std::monostate()};
    switch (this->ch) {
    case '*':
        tok.type = TokenT::ArrayType;
        break;
    case '$':
        tok.type = TokenT::BulkType;
        break;
    case '\r':
        tok.type = TokenT::Retcar;
        break;
    case '\n':
        tok.type = TokenT::NewL;
        break;
    case 0:
        tok.type = TokenT::Eof;
    default:
        if (is_letter(this->ch)) {
            std::string value = this->read_string();
            tok.literal = value;
            tok.type = TokenT::Bulk;
            return tok;
        } else if (is_digit(this->ch)) {
            std::string len = this->read_len();
            tok.literal = len;
            tok.type = TokenT::Len;
            return tok;
        } else {
            return tok;
        }
    }

    this->read_char();

    return tok;
}

std::string Lexer::read_string(void) {
    std::string s;
    while (this->ch != '\r') {
        s.push_back(this->ch);
        this->read_char();
    }
    return s;
}

std::string Lexer::read_len(void) {
    std::string s;
    while (is_digit(this->ch)) {
        s.push_back(this->ch);
        this->read_char();
    }
    return s;
}

void Lexer::read_char(void) {
    if (this->pos >= this->input_len) {
        this->ch = 0;
    } else {
        this->ch = this->input[this->pos];
    }
    this->pos += 1;
}

static bool is_digit(std::uint8_t ch) { return '0' <= ch && ch <= '9'; }

static bool is_letter(std::uint8_t ch) {
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
}
