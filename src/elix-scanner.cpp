#include "elix.hpp"
#include<cctype>
#include<sstream>
#include<iomanip>

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
    auto pos = this->m_pos;
    auto col = this->m_col;
    auto row = this->m_row;
    auto token = this->peek_next_token(row, col, pos);
    this->update(row, col, pos);
    this->advance();
    return token;
}

// -*-
void Tokenizer::reset(u32& row, u32& col, u32& pos){
    std::swap(row, this->m_row);
    std::swap(col, this->m_col);
    std::swap(pos, this->m_pos);
}

// -*-
void Tokenizer::update(u32 row, u32 col, u32 pos){
    this->m_row = row;
    this->m_col = col;
    this->m_pos = pos;
}

// -*-
Token Tokenizer::peek_next_token(u32& row, u32& col, u32& pos){
    Token token;
    this->skip_whitespace();
    this->skip_comment();
    if(this->is_at_end()){
        token = Token(TokenKind::End, "", this->m_row, this->m_col);
        this->reset(row, col, pos);
        return token;
    }
    auto c = this->peek();
    if(c=='\0'){
        token.row = this->m_row;
        token.col = this->m_col;
        token.kind = TokenKind::End;
        token.lexeme = "";
        this->reset(row, col, pos);
        return token;
    }
    switch(c){
    case '(':
        pos = this->m_pos;this->reset(row, col, pos);
        return Token(TokenKind::LParen, "(", row, col);
    case ')':
        pos = this->m_pos;
        this->reset(row, col, pos);
        return Token(TokenKind::RParen, ")", row, col);
    case '[':
        pos = this->m_pos;
        this->reset(row, col, pos);
        return Token(TokenKind::LBracket, "[", row, col);
    case ']':
        pos = this->m_pos;
        this->reset(row, col, pos);
        return Token(TokenKind::LBracket, "[", row, col);
    case '{':
        pos = this->m_pos;
        this->reset(row, col, pos);
        return Token(TokenKind::LParen, "{", row, col);
    case '}':
        pos = this->m_pos;
        this->reset(row, col, pos);
        return Token(TokenKind::LParen, "}", row, col);
    case '#':
        // hashset or symbol
        // hashset-literal: #{key1 key2 key3 ... }
        // symbol: #name
        if(this->peek_next()=='{'){ // hashset-literal
            this->reset(row, col, pos);
            return Token(TokenKind::PoundBrace, "#{", (row-1), (col-1));
        }else if(this->peek_next()=='('){ // hashset-literal
            this->reset(row, col, pos);
            return Token(TokenKind::PoundParen, "#(", (row-1), (col-1));
        }
        // symbol starting with '#'
        token = this->read_literal(pos);
        this->reset(row, col, pos);
        return token;
    case '"': // string-literal
        token = this->read_string(pos);
        this->reset(row, col, pos);
        return token;
    default:{
            // Integer format:
            //      hex-integer format:  0x01f1
            //      oct-integer format: 0o01237
            //      binary-integer format: 0b011101
            //      decimal-integer format: 1028381
            bool check = (
                ((c=='-' || c=='+') && std::isdigit(this->peek_next())) || // decimal-integer format ?
                (c=='0'  || this->peek_next()=='x') ||  // hex-integer format
                (c=='0'  || this->peek_next()=='o') ||  // oct-integer format
                (c=='0'  || this->peek_next()=='b')     // binary-integer format
            );
            if(check){
                token = this->read_number(pos);
                this->reset(row, col, pos);
                return token;
            }
            token = this->read_literal(pos);
            this->reset(row, col, pos);
            return token;
        }//
    }
}

// -*-
Token Tokenizer::read_number(u32& myPos){
    auto r = this->m_row;
    auto c = this->m_col;
    auto token = this->read_literal(myPos);
    auto possiblyFloat = [](const std::string& numstr){
        if(numstr.find('.')!=std::string::npos){ return true; }
        else if(numstr.find('e')!=std::string::npos){ return true; }
        else if(numstr.find('E')!=std::string::npos){ return true; }
        else{ return false; }
    };
    std::string text(token.lexeme);
    auto len = text.length();
    std::string errmsg{};
    if(len > 2 && text[0]=='0' && text[1]=='x'){
        std::string hexstr = "0123456789abcdefABCDEF";
        if(len > 3){
            auto failed{false};
            for(auto i=2; i < len; i++){
                if(hexstr.find(text[i])==std::string::npos){
                    failed = true;
                    break;
                }
            }
            if(!failed){
                token.kind = TokenKind::Integer;
                return token;
            }else{
                errmsg = "malformed integer in hexa-decimal format";
            }
        }
    }else if(len > 2 && text[0]=='0' && text[1]=='o'){
        std::string octstr = "01234567";
        bool failed{false};
        for(auto i=2; i < len; i++){
            if(octstr.find(text[i])==std::string::npos){
                failed = true;
                break;
            }
        }
        if(!failed){
            token.kind = TokenKind::Integer;
            return token;
        }else{
            errmsg = "malformed integer in octal-decimal format";
        }
    }else if(len > 2 && text[0]=='0' && text[1]=='b'){
        std::string binstr = "01";
        bool failed{false};
        for(auto i=2; i < len; i++){
            if(binstr.find(text[i])==std::string::npos){
                failed = true;
                break;
            }
        }
        if(!failed){
            token.kind = TokenKind::Integer;
            return token;
        }else{
            errmsg = "malformed integer in binary-decimal format";
        }
    }else if(possiblyFloat(text)){
        bool failed{false};
        bool hasE{false};
        int dotNum{0};  // number of '.'
        int eNum{0};    // number of 'e' or 'E' 
        int mNum{0};    // number of '-'
        int pNum{0};    // number of '+'
        size_t pos = 0;
        // the decimal part
        for(;pos < text.length(); pos++){
            auto c = text[pos];
            if(c=='-'){ mNum++; }
            if(c=='-'){ pNum++; }
            if(pNum > 1 && mNum > 1){
                failed = true;
                errmsg = "malformed floating-point number";
                break;
            }
            if(c=='.'){
                dotNum++;
                pos++;
                break;
            }
            if(c=='e' || c=='E'){
                eNum++;
                pos++;
                break;
            }
        }

        // fractional end optional exponential part
        if(dotNum > 0 && pos < text.length()){
            auto c = text[pos++];
            while(true){
                if(c == 'e' || c=='E'){
                    eNum++;
                    // in scientific format: expect the exponential part
                    if(pos==text.length()){
                        errmsg = "malformed floating-point number in scientific format";
                        failed = true;
                    }
                    break;
                }
                if(pos==text.length()){ break; }
                c = text[pos++];
            }
        }

        // read the exponential part
        auto c = text[pos++];
        bool hasReadDigit{false};
        while(true){
            if(std::isdigit(c)){ hasReadDigit = true; }
            if(c=='-'){ mNum++;}
            if(c=='+'){ pNum++;}
            if(pNum > 2 || mNum > 2){
                failed = true;
                break;
            }
            if(!hasReadDigit && pos==text.length()){
                errmsg = "malformed floating-point number in scientific format";
                failed = true;
                break;
            }
            if(hasReadDigit && pos==text.length()){
               break;
            }
            c = text[pos++];
        }

        if(!failed){
            token.kind = TokenKind::Float;
            return token;
        }
    }

    errmsg += (
        " at row " + std::to_string(token.row) +
        " and column " + std::to_string(token.col)
    );
    throw ELixError(ELixError::SyntaxError, errmsg);
}

// -*-
Token Tokenizer::read_literal(u32& myPos){
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
    myPos = this->m_pos;
    return token;
}

// -*-
Token Tokenizer::read_string(u32& myPos){
    Token token;
    token.kind = TokenKind::Str;
    token.row = this->m_row;
    token.col = this->m_col;
    this->advance(); // eat '"'
    auto c = this->peek();
    token.lexeme;
    while(!this->is_at_end() && c != '"'){
        if(c=='\\' && this->m_pos+1 < this->m_src.size()){
            // push the escaped character
            token.lexeme.push_back(this->advance());
        }else{
            token.lexeme.push_back(c);
        }
        c = this->advance();
    }
    if(!this->is_at_end() && this->peek()!='"'){
        //! @note: do nothing
        // '"' is handled in this->next_token()
    }else{
        auto r_ = std::to_string(this->m_row);
        auto c_ = std::to_string(this->m_col);
        std::string msg{
            "malformed string literal. Expected a closing '\"'"
            " at row " + r_ + " and column " + c_ + "."
        };
        throw ELixError(ELixError::SyntaxError, msg);
    }
    myPos = this->m_pos;
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
void Tokenizer::skip_comment(void) {
    if(this->peek()==';'){
        while(!this->is_at_end() || (this->peek()!='\n')){
            this->advance();
        }
    }
}

// -*-
Parser::Parser(Tokenizer& tokenizer)
: m_tokenizer{tokenizer}
{}

// -*-
void Parser::next_token(void){
    this->m_token = this->m_tokenizer.next_token();
}

// -*-
bool Parser::is_at_end(void){
    return (
        this->m_token.kind==TokenKind::End ||
        this->m_tokenizer.is_at_end()
    );
}

// -*-
Expression Parser::match(std::initializer_list<TokenKind> kinds){
    for(auto kind: kinds){
        switch(kind){
        case TokenKind::LParen:
            return this->parse_list();
        case TokenKind::LBracket:
            return this->parse_array();
        case TokenKind::False:
        case TokenKind::True:
        case TokenKind::NIL:
        case TokenKind::Sym:
        case TokenKind::Str:
        case TokenKind::Integer:
        case TokenKind::Float:
            return this->parse_literal();
        case TokenKind::LBrace:
            return this->parse_hashmap();
        case TokenKind::PoundBrace:
            return this->parse_hashset();
        case TokenKind::End:
            return std::make_unique<LiteralExpr>(std::move(Object()));
        }
    }
    return std::make_unique<LiteralExpr>(std::move(Object()));
}

// -*-
Expression Parser::parse_list(void){
    Vec<Expression> exprs;
    while(true){
       auto tok = this->m_token = this->m_tokenizer.next_token();
        if(tok.kind==TokenKind::RParen || tok.kind==TokenKind::End){
            break;
        }
        this->m_tokenizer.m_pos -= tok.lexeme.length();
        if(tok.kind==TokenKind::LParen){ this->m_tokenizer.m_pos--; }
        exprs.push_back(this->parse());
    }
    return std::make_unique<ListExpr>(exprs);
}

// -*-
Expression Parser::parse_hashset(void){
    //! @todo
    return nullptr;
}

// -*-
Expression Parser::parse_hashmap(void){
    Vec<Expression> exprs;
    auto token = this->m_token;

    while(true){
       auto tok = this->m_token = this->m_tokenizer.next_token();
        if(tok.kind==TokenKind::RParen || tok.kind==TokenKind::End){
            break;
        }
        this->m_tokenizer.m_pos -= tok.lexeme.length();
        if(tok.kind==TokenKind::LParen){ this->m_tokenizer.m_pos--; }
        exprs.push_back(this->parse());
    }
    return std::make_unique<ListExpr>(exprs);
}

// -*-
Expression Parser::parse_array(void){
    //! @todo
    return nullptr;
}

// -*-
Expression Parser::parse_literal(void){
    auto tok = this->m_token;
    switch(tok.kind){
    case TokenKind::Sym:
    case TokenKind::True:
    case TokenKind::False:
    case TokenKind::NIL:
        return this->parse_symbol();
    case TokenKind::Integer:{
            i64 num{};
            if(this->m_token.lexeme.find('x')!=std::string::npos){
                size_t pos{};
                num = std::stoll(this->m_token.lexeme, &pos, 16);
            }else if(this->m_token.lexeme.find('o')!=std::string::npos){
                size_t pos{};
                num = std::stoll(this->m_token.lexeme, &pos, 8);
            }else if(this->m_token.lexeme.find('b')!=std::string::npos){
                size_t pos{};
                num = std::stoll(this->m_token.lexeme, &pos, 2);
            }else{
                size_t pos{};
                num = std::stoll(this->m_token.lexeme, &pos);
            }
            return std::make_unique<LiteralExpr>(std::move(Object(num)));
        }
    case TokenKind::Float:{
            auto num = Number(std::stod(tok.lexeme));
            return std::make_unique<LiteralExpr>(std::move(Object(num)));
        }
    case TokenKind::Str:
        return std::make_unique<LiteralExpr>(std::move(Object(this->m_token.lexeme)));
    default:
        break;
    }
    auto r = this->m_token.row;
    auto c = this->m_token.col;
    std::stringstream ss;
    ss << "error while parsing ELix literal at row " << r;
    ss << " and column " << c << ".\nFound illegal token ";
    ss << std::quoted(this->m_token.lexeme);
    
    throw ELixError(ELixError::SyntaxError, ss.str());
}

// -*-
Expression Parser::parse_symbol(void){
    auto tok = this->m_token;
    switch(tok.kind){
    case TokenKind::True:
        return std::make_unique<LiteralExpr>(std::move(Object(true)));
    case TokenKind::False:
        return std::make_unique<LiteralExpr>(std::move(Object(false)));
    case TokenKind::NIL:
        return std::make_unique<LiteralExpr>(std::move(Object()));
    case TokenKind::Sym:
        return std::make_unique<LiteralExpr>(std::move(Object(Symbol(tok.lexeme))));
    }
    std::stringstream ss;
    ss << "Illegal symbol " << std::quoted(tok.lexeme) << " found at ";
    ss << "row " << tok.row << " and column " << tok.col;
    throw ELixError(ELixError::SyntaxError, ss.str());
}

// -*-
Expression Parser::parse_pair(void){
    //! @todo
    return nullptr;
}

// -*-
void Parser::expect(TokenKind kind){
    std::map<TokenKind, std::string> reservedWords = {
#define ELIX_DEF(tok, word) {TokenKind::tok, word},
        ELIX_RESERVED_WORDS()
#undef ELIX_DEF
    };
    auto entry = reservedWords.find(kind);
    if(entry != reservedWords.end()){ return; }
    std::stringstream ss;
    ss << "expect " << std::quoted(reservedWords[kind]);
    ss << " but found " << this->m_token.lexeme << " at ";
    ss << "row " << this->m_token.row << " and column " << this->m_token.col;
    throw ELixError(ELixError::SyntaxError, ss.str());
}

/*

// --------------
// -*- Parser -*-
// --------------
class Parser{
public:

Expression Parser::parse(void);

private:
    Tokenizer& m_tokenizer;



    
};

*/

// -*--------------------------------------------------------------------------*-
}//-*- end::namespace::ekasoft::elx                                           -*-
// -*--------------------------------------------------------------------------*-