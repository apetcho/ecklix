#include "elix.hpp"
#include<cctype>
#include<initializer_list>

// -*--------------------------------------------------------------------------*-
// -*- begin::namespace::ekasoft::elx                                         -*-
// -*--------------------------------------------------------------------------*-
namespace ekasoft::elx{
// -
Tokenizer::Tokenizer(const std::string& src)
: m_src{src}
{}

// -*-
bool Tokenizer::is_at_end(void) const{
    return (this->m_pos >= this->m_src.length());
}

// -*-
void Tokenizer::skip_whitespace(void){
    while(!this->is_at_end() && std::isspace(this->m_src[this->m_pos])){
        this->m_pos++;
    }
}

// -*-
bool Tokenizer::is_symbol_char(char c) const{
    const std::string symchars{
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "*^!%&/=+-.,@$<>_:#?"
        "0123456789"
    };
    return (symchars.find(c)!=std::string::npos);
}

// -*-
Token Tokenizer::next_token(void){
    this->skip_whitespace();
    this->skip_comment(this->peek());
    if(this->is_at_end()){
        return Token(TokenKind::End, "", this->m_row, this->m_col);
    }
    auto c = this->peek();
    switch(c){
    case '(':
        this->advance();
        return Token(TokenKind::LParen, "(", this->m_row, this->m_col);
    case ')':
        this->advance();
        return Token(TokenKind::LParen, ")", this->m_row, this->m_col);
    case '[':
        this->advance();
        return Token(TokenKind::LParen, "[", this->m_row, this->m_col);
    case ']':
        this->advance();
        return Token(TokenKind::LParen, "]", this->m_row, this->m_col);
    case '{':
        this->advance();
        return Token(TokenKind::LParen, "{", this->m_row, this->m_col);
    case '}':
        this->advance();
        return Token(TokenKind::LParen, "}", this->m_row, this->m_col);
    case '"':
        this->advance();
        return this->read_string();
    default:{
            // std::initializer_list<TokenKind> kinds{
            //     TokenKind::Arrow, TokenKind::Dot,
            //     TokenKind::Quote, TokenKind::Quasiquote,
            //     TokenKind::Unquote, TokenKind::UnquoteSplicing,
            //     TokenKind::Progn, TokenKind::If,
            //     TokenKind::Let, TokenKind::Var,
            //     TokenKind::Cond, TokenKind::False,
            //     TokenKind::For, TokenKind::Fun,
            //     TokenKind::Lambda, TokenKind::Macro,
            //     TokenKind::NIL, TokenKind::While, TokenKind::True,
            // };
            
            if((c=='-' || c=='+') && std::isdigit(this->peek_next())){
                return this->read_number();
            }
            return this->read_symbol();
        }//
    }
}

/*
class Tokenizer{
public:



private:
    std::string m_src;
    u32 m_pos = 0;
    u32 m_row = 1;
    u32 m_col = 1;

Token Tokenizer::read_number(void){}
Token Tokenizer::read_symbol(void){}
Token Tokenizer::read_string(void){}
char Tokenizer::peek(void) const{}
char Tokenizer::peek_next(void) const{}
char Tokenizer::advance(void){}
Token Tokenizer::match(std::initializer_list<TokenKind> kind, const std::string& text){}

};

// --------------
// -*- Parser -*-
// --------------
class Parser{
public:
    explicit Parser(Tokenizer& tokenizer);

    Expression parse(void);

private:
    Tokenizer& m_tokenizer;

    bool is_at_end(void);
    Expression match(std::initializer_list<TokenKind> kinds);
    Expression parse_list(void);
    Expression parse_hashset(void);
    Expression parse_hashmap(void);
    Expression parse_array(void);
    Expression parse_literal(void);
    Expression parse_symbol(void);

    // -*-
    Object handle_progn(Vec<Expression> exprs);
    Object handle_if(Vec<Expression> exprs);
    Object handle_let(Vec<Expression> exprs);
    Object handle_var(Vec<Expression> exprs);
    Object handle_cond(Vec<Expression> exprs);
    Object handle_while(Vec<Expression> exprs);
    Object handle_for(Vec<Expression> exprs);
    Object handle_fun(Vec<Expression> exprs);
    Object handle_macro(Vec<Expression> exprs);
    Object handle_quote(Vec<Expression> exprs);
    Object handle_quasiquote(Vec<Expression> exprs);
    Object handle_unquote(Vec<Expression> exprs);
    Object handle_unquote_splicing(Vec<Expression> exprs);
    Object handle_list(Vec<Expression> exprs); // function-call
};

*/

// -*--------------------------------------------------------------------------*-
}//-*- end::namespace::ekasoft::elx                                           -*-
// -*--------------------------------------------------------------------------*-