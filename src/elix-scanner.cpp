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
        auto [[maybe_unused]] _ = this->advance();
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
            bool check = (
                ((c=='-' || c=='+') && std::isdigit(this->peek_next())) ||
                (c=='0'  || this->peek_next()=='x') ||
                (c=='0'  || this->peek_next()=='b') ||
                (c=='0'  || this->peek_next()=='o')
            );
            if(check){
                return this->read_number();
            }
            return this->read_token();
        }//
    }
}

// -*-
Token Tokenizer::match(const std::string& text){
    std::map<std::string, TokenKind> kindmap = {
#define ELIX_DEF(tok, name) { name, TokenKind::tok},
        ELIX_RESERVED_WORDS()
#undef ELIX_DEF
        { ".", TokenKind::Dot},
        { "=>", TokenKind::Arrow},
    };
    auto entry = kindmap.find(text);
    if(entry != kindmap.end()){
        return Token(entry->second, text, this->m_row, this->m_row);
    }
    return Token(TokenKind::Sym, text, this->m_row, this->m_row);
}

// -*-
Token Tokenizer::read_number(void){
    auto r = this->m_row;
    auto c = this->m_col;
    auto token = this->read_token();
    token.kind = TokenKind::Sym;
    token.row = r;
    token.col = c;
    std::string text(token.lexeme);
    auto len = text.length();
    if(len > 2 && text[0]=='0' && text[1]=='x'){
        std::string hexstr = "0123456789abcdefABCDEF";
        bool ishex{true};
        for(auto i=2; i < len; i++){
            if(hexstr.find(text[i])==std::string::npos){
                ishex = false;
                break;
            }
        }
        if(ishex){
            token.kind = TokenKind::Integer;
            return token;
        }else{
            token.kind = TokenKind::Sym;
            return token;
        }
    }else if(len > 2 && text[0]=='0' && text[1]=='o'){
        std::string octstr = "01234567";
        bool isoct{true};
        for(auto i=2; i < len; i++){
            if(octstr.find(text[i])==std::string::npos){
                isoct = false;
                break;
            }
        }
        if(isoct){
            token.kind = TokenKind::Integer;
            return token;
        }else{
            return token;
        }
    }else if(len > 2 && text[0]=='0' && text[1]=='b'){
        std::string binstr = "01";
        bool isbin{true};
        for(auto i=2; i < len; i++){
            if(binstr.find(text[i])==std::string::npos){
                isbin = false;
                break;
            }
        }
        if(isbin){
            token.kind = TokenKind::Integer;
            return token;
        }else{
            return token;
        }
    }else{
        if(text.find('.')!=std::string::npos){
            size_t pos = 0;
            auto [[maybe_unused]] num = std::stod(text, &pos);
            if(pos!=text.length()){
                return token;
            }else{
                token.kind = TokenKind::Float;
                return token;
            }
        }else if(text.find('e')!=std::string::npos || text.find('E')!=std::string::npos){
            size_t pos = 0;
            auto [[maybe_unused]] num = std::stod(text, &pos);
            if(pos!=text.length()){
                return token;
            }else{
                token.kind = TokenKind::Float;
                return token;
            }
        }
        return token;
    }
}

// -*-
Token Tokenizer::read_token(void){
    std::string text{};
    Token token;
    token.row = this->m_row;
    token.col = this->m_col;
    auto c = this->advance();
    text.push_back(c);
    while(!this->is_at_end() || !std::isspace(c)){
        c = this->advance();
        text.push_back(c);
    }
    token.kind = TokenKind::Sym;
    return token;
}

// -*-
Token Tokenizer::read_string(void){
    Token token;
    token.kind = TokenKind::Str;
    token.row = this->m_row;
    token.col = this->m_col;
    this->advance(); // eat '"'
    auto c = this->peek();
    token.lexeme;
    while(!this->is_at_end() && c != '"'){
        if(c=='\\' && this->m_pos+1 < this->m_src.size()){
            auto esc = this->advance();
            if(esc=='n'){ token.lexeme.push_back('\n'); }
            else if(esc=='r'){ token.lexeme.push_back('\r'); }
            else if(esc=='b'){ token.lexeme.push_back('\b'); }
            else if(esc=='t'){ token.lexeme.push_back('\t'); }
            else if(esc=='f'){ token.lexeme.push_back('\f'); }
            else{ token.lexeme.push_back(esc); }
            this->m_pos++;
        }else{
            token.lexeme.push_back(c);
            c = this->advance();
        }
    }
    if(!this->is_at_end() && this->peek()=='"'){
        this->advance(); // eat 
    }
    
    return token;
}

// -*-
char Tokenizer::peek(void) const{
    if(this->is_at_end()){ return '\0'; }
    return this->m_src[this->m_pos];
}

// -*-
char Tokenizer::peek_next(void) const{
    if((this->m_pos + 1) >= this->m_src.length()){
        return '\0';
    }
    return this->m_src[(this->m_pos+1)];
}

// -*-
char Tokenizer::advance(void){
    auto c = this->m_src[this->m_pos];
    if(c == '\n'){
        this->m_row += 1;
        this->m_col = 1;
    }else{
        this->m_col += 1;
    }
    this->m_pos += 1;
    return c;
}

// -*-
void Tokenizer::skip_comment(char c) {
    if(c==';'){
        while(!this->is_at_end() || (this->peek()!='\n')){
            this->advance();
        }
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