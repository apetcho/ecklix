#ifndef MY_ELIX_HPP
#define MY_ELIX_HPP

#include<initializer_list>
#include<unordered_map>
#include<unordered_set>
#include<functional>
#include<filesystem>
#include<stdexcept>
#include<cstdint>
#include<variant>
#include<utility>
#include<memory>
#include<string>
#include<vector>
#include<map>
#include<set>

#define ELIX_RESERVED_WORDS()                       \
    ELIX_DEF(Progn, "progn")                        \
    ELIX_DEF(If, "if")                              \
    ELIX_DEF(Let, "let")                            \
    ELIX_DEF(Var, "var")                            \
    ELIX_DEF(Cond, "cond")                          \
    ELIX_DEF(While, "while")                        \
    ELIX_DEF(For, "for")                            \
    ELIX_DEF(Lambda, "lambda")                      \
    ELIX_DEF(Fun, "fun")                            \
    ELIX_DEF(Macro, "macro")                        \
    ELIX_DEF(And, "and")                            \
    ELIX_DEF(Or, "or")                              \
    ELIX_DEF(Not, "not")                            \
    ELIX_DEF(NIL, "nil")                            \
    ELIX_DEF(True, "true")                          \
    ELIX_DEF(False, "false")                        \
    ELIX_DEF(Quote, "quote")                        \
    ELIX_DEF(Unquote, "unquote")                    \
    ELIX_DEF(Quasiquote, "quasiquote")              \
    ELIX_DEF(UnquoteSplicing, "unquote-splicing")

//  ELIX_DEF(Dot, ".")
//  ELIX_DEF(Arrow, "=>")

#define ELIX_TOKENS()               \
    ELIX_DEF(LParen, "(")           \
    ELIX_DEF(RParen, ")")           \
    ELIX_DEF(LBracket, "[")         \
    ELIX_DEF(RBracket, "]")         \
    ELIX_DEF(LBrace, "{")           \
    ELIX_DEF(RBrace, "}")           \
    ELIX_DEF(PoundBrace, "#{")      \
    ELIX_DEF(PoundParen, "#(")      \
    ELIX_DEF(Sym, "SYMBOL")         \
    ELIX_DEF(Integer, "INTEGER")    \
    ELIX_DEF(Float, "FLOAT")        \
    ELIX_DEF(Str, "STRING")         \
    ELIX_RESERVED_WORDS()           \
    ELIX_DEF(End, "END")


// -
namespace fs = std::filesystem;

// -*--------------------------------------------------------------------------*-
// -*- begin::namespace::ekasoft::elx                                         -*-
// -*--------------------------------------------------------------------------*-
namespace ekasoft::elx{
// -

class ModuleLoader;
struct ExprBase;
struct ExprVisitor;
class Object;
class Env;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename T>
using Vec = std::vector<T>;

// using Dict = std::map<std::string, Object>;
// using Pair = std::pair<Object, Object>;

using u64 = std::uint64_t;
using i64 = std::int64_t;
using u32 = std::uint32_t;
using i32 = std::int32_t;
using f32 = float;
using f64 = double;
using usize = std::uint64_t;
using isize = std::int64_t;

using Loader = std::shared_ptr<ModuleLoader>;
using Context = std::shared_ptr<Env>;
using Expression = std::unique_ptr<ExprBase>;
using Fn = std::function<Object(const Vec<Object>&, Context)>;
using Visitor = std::unique_ptr<ExprVisitor>;

// -*-----------------------------------------*-
// -*- begin::namespace::ekasoft::elx::utils -*-
// -*-----------------------------------------*-
namespace utils{
// -
struct Hash{
    std::size_t operator()(const Object& key) const;
};

struct Equal{
    bool operator()(const Object& lhs, const Object& rhs) const;
};

// -*-----------------------------------------*-
}//-*- end::namespace::ekasoft::elx::utils   -*-
// -*-----------------------------------------*-

using HashMap = std::unordered_map<Object, Object, utils::Hash, utils::Equal>;
using HashSet = std::unordered_set<Object, utils::Hash, utils::Equal>;

// -*-
struct Nil final{};

// -*-
class Number final{
public:
    explicit Number();
    explicit Number(i64 num);
    explicit Number(f64 num);
    Number(const Number& num) noexcept;
    Number(Number&& num) noexcept;
    Number& operator=(const Number& num) noexcept;
    Number& operator=(Number&& num) noexcept;
    std::string str(void) const;
    std::string repr(void) const;
    operator bool() const;
    operator i64() const;
    operator f64() const;
    bool as_bool(void);
    i64 as_integer(void);
    f64 as_float(void);
    bool is_integer(void) const;

    // -
    Number clone(void) const;

    // -
    Number abs(void) const;
    Number ceil(void) const;
    Number floor(void) const;
    Number round(void) const;
    Number truncate(void) const;
    Number sin(void) const;
    Number cos(void) const;
    Number tan(void) const;
    Number asin(void) const;
    Number acos(void) const;
    Number atan(void) const;
    Number atan2(const Number& rhs) const;
    Number sinh(void) const;
    Number cosh(void) const;
    Number tanh(void) const;
    Number asinh(void) const;
    Number acosh(void) const;
    Number atanh(void) const;
    Number exp(void) const;
    Number expm1(void) const;
    Number exp2(void) const;
    Number log(void) const;
    Number log2(void) const;
    Number log10(void) const;
    Number log1p(void) const;
    Number erf(void) const;
    Number erfc(void) const;
    Number tgamma(void) const;
    Number lgamma(void) const;
    bool isnan(void) const;
    bool isinf(void) const;
    bool isfinite(void) const;
    Number& operator!();
    Number& operator-();
    Number& operator~();

    friend Number operator+(const Number& lhs, const Number& rhs);
    friend Number operator-(const Number& lhs, const Number& rhs);
    friend Number operator*(const Number& lhs, const Number& rhs);
    friend Number operator/(const Number& lhs, const Number& rhs);
    friend Number operator%(const Number& lhs, const Number& rhs);

    friend Number operator&(const Number& lhs, const Number& rhs);
    friend Number operator|(const Number& lhs, const Number& rhs);
    friend Number operator^(const Number& lhs, const Number& rhs);
    friend Number operator<<(const Number& lhs, const Number& rhs);
    friend Number operator>>(const Number& lhs, const Number& rhs);

    friend bool operator&&(const Number& lhs, const Number& rhs);
    friend bool operator&&(const Number& lhs, const Number& rhs);

    friend bool operator==(const Number& lhs, const Number& rhs);
    friend bool operator!=(const Number& lhs, const Number& rhs);
    friend bool operator<(const Number& lhs, const Number& rhs);
    friend bool operator>(const Number& lhs, const Number& rhs);
    friend bool operator<=(const Number& lhs, const Number& rhs);
    friend bool operator>=(const Number& lhs, const Number& rhs);

private:
    using Value = std::variant<i64, f64>;

    Value m_value;
    void value(i64& num){
        num = std::get<i64>(this->m_value);
    }

    void value(f64& num){
        num = std::get<f64>(this->m_value);
    }

    void value(bool& val){
        if(std::holds_alternative<i64>(this->m_value)){
            val = (std::get<i64>(this->m_value)==0);
        }else{
            val = (std::get<f64>(this->m_value)==0.0);
        }
    }
};

// -*-
struct Symbol final {
    std::string value{};
    std::string str(void) const;
    std::string repr(void) const;
    Symbol clone(void) const;

    friend bool operator==(const Symbol& lhs, const Symbol& rhs);
    friend bool operator!=(const Symbol& lhs, const Symbol& rhs);
};

// -*-
struct Lambda final{
    Vec<Symbol> params{};
    Vec<Expression> body{};
    Context ctx{};
    bool named{false}; // false => lambda, true => function
    std::string str(void) const;
    std::string repr(void) const;
    Lambda clone(void) const;

    Object operator()(const Vec<Object>& args);
};

// -*-
struct List final{
    Vec<Object> items{};
    std::string str(void) const;
    std::string repr(void) const;
    List clone(void) const;

    i64 find(const Object& args) const;
    List& reverse(void);
    List& concat(const List& args);
    i64 len(void) const;
    Object head(void) const;
    List tail(void) const;
    Object first(void) const;
    Object last(void) const;
    List& push(const Object& arg);
    List& pop(void);
    List& insert(i64 idx, const Object& obj);
    Object remove(i64 idx);
};

// -*-
struct Array final {
    Vec<Object> items{};
    std::string str(void) const;
    std::string repr(void) const;
    Array clone(void) const;

    i64 find(const Object& arg, i64 from=0);
    Array& reverse(void);
    Array& concat(const Array& array);
    i64 len(void) const;
    Array slice(i64 i, i64 j) const;
    Array& insert(i64 idx,  const Object& arg);
    Object get(i64 idx) const;
    Array& set(i64 idx, const Object& arg);
    Array& splice(i64 idx, const Object& arg);
    bool any(const Object& predicate) const;
    bool all(const Object& predicate) const;
    Object reduce(const Object& fn, const Object& initVal) const;
    String& sort(const Object& fn);
};

// -*-
struct String final{
    std::string text{};

    std::string str(void) const;
    std::string repr(void) const;
    String clone(void) const;

    i64 find(const String& needle, i64 from=0) const;
    String& reverse(void);
    String& concat(const String& arg);
    i64 len(void) const;

    friend bool operator+(const String& lhs, const String& rhs);
    friend bool operator==(const String& lhs, const String& rhs);
    friend bool operator!=(const String& lhs, const String& rhs);

    Vec<String> split(const String delim=String{});
    String slice(i64 i, i64 j) const;
    bool startswith(const String& rhs) const;
    bool endswith(const String& rhs) const;
    String& ltrim(void);
    String& rtrim(void);
    String& trim(void);
    String& lower(void);
    String& upper(void);
    String& capitalize(void);
    String& title(void);
    Vec<i64> find_all(const String& needle);
    String& replace(const String& needle, i64 from=0);
    String& replace_all(void);
    String chr(void) const;
    i64 ord(void) const;
    String substr(i64 i, i64 j) const;
};

// -*-
struct Pair final{
    Object key;
    Object val;
    std::string str(void) const;
    std::string repr(void) const;
    String clone(void) const;
};

// -*-
struct Dict final{ // Dict
    HashMap hmap;
    std::string str(void) const;
    std::string repr(void) const;
    Dict clone(void) const;

    Object find(const Object& key) const;
    Dict& concat(const Dict&& dict);
    i64 len(void) const;
    Object get(const Object& key) const;
    Dict& set(const Object& key, const Object& val);
    Dict& update(const Object& key, const Object& val);
    Vec<Object> keys(void) const;
    Vec<Object> values(void) const;
    Object popitem(const Object& key) const;
    Dict& clear(void);
    Vec<Pair> items(const Vec<Object>& args);
};

// -*-
struct Set final{
    HashSet hset;
    std::string str(void) const;
    std::string repr(void) const;
    Set clone(void) const;

    bool find(const Object& needle) const;
    Set& concat(const Set& args);
    i64 len(void) const;
    Set& add(const Object& key);
    Set set_union(const Set& rhs) const;
    Set intersection(const Set& rhs) const;
    Set symmetric_difference(const Set& rhs) const;
    Set& clear(void);
    Set& discard(const Object& key);
    Object pop(void);
    bool isdisjoint(const Set& rhs) const;
    bool issubset(const Set& rhs) const;
    bool issuperset(const Set& rhs) const;
};

struct Func final{
    Fn fn = nullptr;
    std::string str(void) const;
    std::string repr(void) const;
    Func clone(void) const;

    Object operator()(const Vec<Object>& args);
};

struct Macro final{
    Vec<Symbol> params{};
    Vec<Expression> body{};
    Context ctx{};
    std::string str(void) const;
    std::string repr(void) const;
    Macro clone(void) const;

    Expression expand(const Expression& expr, Context& ctx);
};

// -*-
class Object final{
public:
    Object();
    Object(bool b);
    Object(i64 num);
    Object(f64 num);
    Object(const Number& num);
    Object(const Symbol& sym);
    Object(const char* cstr);
    Object(const std::string& str);
    Object(const String& str);
    Object(const Array& array);
    Object(const List& xs);
    Object(const Dict& xmap);
    Object(const Set& xset);
    Object(const Pair& pair);
    Object(const Lambda& lambda);
    Object(const Macro& macro);
    Object(const Func& func);
    Object(const Object& obj) noexcept;
    Object(Object&& obj) noexcept;
    Object& operator=(const Object& obj) noexcept;
    Object& operator=(Object&& obj) noexcept;

    ~Object(){}

    bool is_nil(void) const;
    bool is_integer(void) const;
    bool is_float(void) const;
    bool is_number(void) const;
    bool is_symbol(void) const;
    bool is_string(void) const;
    bool is_array(void) const;
    bool is_list(void) const;
    bool is_macro(void) const;
    bool is_lambda(void) const;
    bool is_func(void) const;
    bool is_callable(void) const;
    bool is_pair(void) const;

    i64 as_integer(void) const;
    f64 as_float(void) const;
    Symbol as_symbol(void) const;
    String as_string(void) const;
    Array as_array(void) const;
    Dict as_dict(void) const;
    List as_list(void) const;
    Set as_set(void) const;
    Lambda as_lambda(void) const;
    Macro as_macro(void) const;
    Func as_func(void) const;
    Pair as_pair(void) const;

    std::string str(void) const;
    std::string repr(void) const;
    Object clone(void) const;

    bool is_hashable(void) const;
    Symbol type(void) const;

private:
    using Value = std::variant<
        Nil, bool, Number, String, Symbol, Func, Lambda, Macro,
        Pair, List, Array, Dict, Set
    >;

    Value m_value;
};

// -*-
class ELixError final: public std::runtime_error {
public:
    explicit ELixError();
    explicit ELixError(const Symbol& sym);
    explicit ELixError(const Symbol& sym, const std::string& msg);

    static Symbol ValueError;
    static Symbol TypeError;
    static Symbol SyntaxError;
    static Symbol RuntimeError;
    static Symbol KeyError;
    static Symbol IndexError;

    std::string describe(void) const;
private:
    Symbol m_kind;
};

// -----------
// -*- Env -*-
// -----------
class Env final : public std::enable_shared_from_this<Env> {
public:
    Env(Context parent=nullptr);

    void define(const std::string& name, const Object& obj);
    bool update(const std::string& name, const Object& obj);
    Object get(const std::string& name);

private:
    std::map<std::string, Object> m_bindins;
    Context m_parent;
};

// -*-
struct ExprBase{
    virtual ~ExprBase() = default;
    virtual Object eval(Visitor visitor) = 0;
};

// -*-
struct LiteralExpr: public ExprBase{
    Object obj;
    // explicit LiteralExpr(Object&& obj);
    virtual Object eval(Visitor visitor) override;
};

// -*-
struct SymbolExpr: public ExprBase{
    Symbol name;
    // explicit SymbolExpr(Symbol&& sym);
    virtual Object eval(Visitor visitor) override;
};

struct ListExpr: public ExprBase{
    Vec<Expression> items;
    //explicit ListExpr(Vec<Expression>&& exprs);
    virtual Object eval(Visitor visitor) override;
};

// -*-
struct ArrayExpr: public ExprBase{
    Vec<Expression> items;
    //explicit ArrayExpr(Vec<Expression>&& exprs);
    virtual Object eval(Visitor visitor) override;
};

struct PairExpr: public ExprBase{
    Expression key;
    Expression val;
    //explicit PairExpr(Expression key, Expression val);
    virtual Object eval(Visitor visitor) override;
};

struct DictExpr: public ExprBase{
    Vec<Expression> items;
    //explicit MapExpr(Vec<Expression> items);
    virtual Object eval(Visitor visitor) override;
};

struct SetExpr: public ExprBase{
    Vec<Expression> items;
    //explicit SetExpr(Vec<Expression>&& items);
    virtual Object eval(Visitor visitor) override;
};

struct ExprVisitor{
    virtual ~ExprVisitor() = default;
    virtual Object eval(LiteralExpr& ) = 0;
    virtual Object eval(SymbolExpr& ) = 0;
    virtual Object eval(ListExpr& ) = 0;
    virtual Object eval(ArrayExpr& ) = 0;
    virtual Object eval(DictExpr& ) = 0;
    virtual Object eval(SetExpr& ) = 0;
    virtual Object eval(PairExpr& ) = 0;
};

// ------------------
// -*-- Tokenizer -*-
// -*----------------
enum class TokenKind{
#define ELIX_DEF(tok, _) tok,
    ELIX_TOKENS()
#undef ELIX_DEF
};

// -*-
struct Token{
    TokenKind kind;
    std::string lexeme = "";
    u32 row = 1;
    u32 col = 1;

    Token()
    : kind{TokenKind::End}{}

    Token(TokenKind kind, const std::string& lex, u32 r, u32 c)
    : kind{kind}, lexeme{lex}, row{r}, col{c}
    {}
};

class Tokenizer{
public:
    explicit Tokenizer(const std::string& src);

    Token next_token(void);
    Token peek_next_token(u32& row, u32& col, u32& pos);

private:
    std::string m_src;
    u32 m_pos = 0;
    u32 m_row;
    u32 m_col;

    void skip_whitespace(void);
    void skip_comment(void);
    Token read_number(u32& pos);
    Token read_literal(u32& pos);
    Token read_string(u32& pos);
    bool is_at_end(void) const;
    char peek(void) const;
    char peek_next(void) const;
    char advance(void);
    void reset(u32& row, u32& col, u32& pos);
    void update(u32 row, u32 col, u32 pos);
    bool is_symbol_char(char c) const;
    // Token match(const std::string& text, u32 row, i32 col);

    friend class Parser;
};

// --------------
// -*- Parser -*-
// --------------
class Parser{
public:
    explicit Parser(Tokenizer& tokenizer);

    Vec<Expression> parse(void);

private:
    Tokenizer& m_tokenizer;
    Token m_token;

    void expect(TokenKind kind);
    //void expect(const std::string& tok);
    void next_token(void);
    void skip_token(void);

    bool is_at_end(void);
    Expression match(std::initializer_list<TokenKind> kinds);
    Expression match(TokenKind kind);
    Expression parse_list(void);        // (x y z ....)
    Expression parse_set(void);         // #{k1 k2 ...}
    Expression parse_dict(void);        // { pair1 pair2 pair3 ... }
    Expression parse_array(void);       // [v1 v2 v3 ...]
    Expression parse_literal(void);     // true, false, nil, NUMBER, STRING, SYMBOL
    Expression parse_symbol(void);      // SYMBOL
    Expression parse_pair(void);        // #(x y)
    bool is_valid_symbol(void);

    friend class ELix;
};

// ------------------------------
// -*- ELix : the interpreter -*-
// ------------------------------
class ELix final: protected ExprVisitor{
public:
    struct Module{
        UniquePtr<Symbol> name;
        Context ctx;
    };
    static UniquePtr<Symbol> Quote;
    static UniquePtr<Symbol> Unquote;
    static UniquePtr<Symbol> Quasiquote;
    static UniquePtr<Symbol> UnquoteSplicing;

    //static UniquePtr<Module> mathModule;    // "Math"
    // static Context sysModule;      // "System"
    // static Context fileModule;     // "File"
    // static Context datetimeModule; // "DateTime"
    // static Context jsonModule;     // "JSON"

    static Context prelude;
    explicit ELix(Context ctx);
    Object eval(Expression expr); // evalExprRaw

    static void repl(const Vec<Object>& args);
    static void run(const fs::path& scriptpath, const Vec<Object>& args);

    static void setup(void);

    static bool is_reserved_word(const std::string& word);

private:
    Context m_runtime;
    Loader m_loader;

    Object eval(LiteralExpr& expr) override;
    Object eval(SymbolExpr& expr) override;
    Object eval(ListExpr& expr) override;
    Object eval(ArrayExpr& expr) override;
    Object eval(DictExpr& expr) override;
    Object eval(SetExpr& expr) override;

    static void install_builtin(void);
    static void initialize_math(void);
    static void initialize_string(void);
    // static void initialize_file(void);
    // static void initialize_datetime(void);
    // static void initialize_json(void);
    // static void initialize_process(void);

    void print(const Object& obj);

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

    friend class ModuleLoader;
};


// -*-
class ModuleLoader{
public:
    explicit ModuleLoader();

    void load(const Symbol& name,  Context ctx, Context targetCtx);
    void load(const fs::path& scritpepath, Context targetCtx);
    void setup(ELix* elix);

private:
    ELix* m_elix;
    std::map<std::string, Context> m_cache;
};


// -*--------------------------------------------------------------------------*-
}//-*- end::namespace::ekasoft::elx                                           -*-
// -*--------------------------------------------------------------------------*-


#endif