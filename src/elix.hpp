#ifndef MY_ELIX_HPP
#define MY_ELIX_HPP

#include<initializer_list>
#include<unordered_map>
#include<unordered_set>
#include<functional>
#include<filesystem>
#include<stdexcept>
#include<cstdint>
#include<utility>
#include<memory>
#include<string>
#include<vector>
#include<any>
#include<map>
#include<set>

#define ELIX_RESERVED_WORDS()                       \
    ELIX_DEF(Import, "import")                      \
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
    ELIX_DEF(Cycle, "continue")                     \
    ELIX_DEF(Stop, "break")                          \
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
class ELix;

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

using Context = std::shared_ptr<Env>;
using Expression = std::unique_ptr<ExprBase>;
using Fn = std::function<Object(const Vec<Object>&, ELix*)>;
//using Visitor = std::unique_ptr<ExprVisitor>;
typedef ExprVisitor* Visitor;

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

struct Less{
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

    Object operator()(const Vec<Object>& args, ELix* elix);

    // static ELix* elix;
};

// -*-
struct List final{
    Vec<Object> items{};
    std::string str(void) const;
    std::string repr(void) const;
    List clone(void) const;

    i64 find(const Object& args, i64 from=0) const;
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
    List& clear(void);
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
    Array& splice(i64 idx, const Array& rhs);
    bool any(const Object& predicate, ELix* elix) const;
    bool all(const Object& predicate, ELix* elix) const;
    Object reduce(const Object& fn, const Object& initVal, ELix* elix) const;
    Array& sort(const Object& fn, ELix* elix);
    Array& clear(void);
    Array& push(const Object& rhs);
    Array& pop(void);

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

    friend String operator+(const String& lhs, const String& rhs);
    friend bool operator==(const String& lhs, const String& rhs);
    friend bool operator!=(const String& lhs, const String& rhs);
    friend bool operator<=(const String& lhs, const String& rhs);
    friend bool operator>=(const String& lhs, const String& rhs);
    friend bool operator<(const String& lhs, const String& rhs);
    friend bool operator>(const String& lhs, const String& rhs);


    Vec<String> split(const String delim=String{});
    //String slice(i64 i, i64 j) const;
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
    String& replace(const String& old, const String& neo);
    String& replace_all(const String& old, const String& neo);
    i64 ord(void) const;
    String substr(i64 i, i64 j) const;
    String at(i64 i) const;    
};

// -*-
struct Pair;

// -*-
struct Dict final{ // Dict
    HashMap hmap;
    std::string str(void) const;
    std::string repr(void) const;
    Dict clone(void) const;

    bool find(const Object& key) const;
    Dict& concat(const Dict& dict);
    i64 len(void) const;
    void put(const Object& key, const Object& val);
    Object get(const Object& key) const;
    Dict& set(const Object& key, const Object& val);
    Dict& update(const Object& key, const Object& val);
    Vec<Object> keys(void) const;
    Vec<Object> values(void) const;
    Pair popitem(const Object& key);
    Dict& remove(const Object& key);
    Dict& clear(void);
    Vec<Pair> items(void) const;
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
    Set difference(const Set& rhs) const;
    Set& clear(void);
    Set& remove(const Object& key);
    Object pop(void);
    bool isdisjoint(const Set& rhs) const;
    bool issubset(const Set& rhs) const;
    bool issuperset(const Set& rhs) const;
};

struct Func final{
    Fn fn;
    i32 minArgc;
    i32 maxArgc;
    std::string str(void) const;
    std::string repr(void) const;
    Func clone(void) const;

    Object operator()(const Vec<Object>& args, ELix* elix);
};

struct Macro final{
    Symbol name{};
    Vec<Symbol> params{};
    Vec<Expression> body{};
    Context ctx{};
    std::string str(void) const;
    std::string repr(void) const;
    Macro clone(void) const;

    Expression expand(const Vec<Object>& args, ELix* elix);
    Object operator()(const Vec<Object>& args, ELix* elix);

    // static ELix* elix;
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

    ~Object(); //{}

    bool is_nil(void) const;
    bool is_bool(void) const;
    bool is_integer(void) const;
    bool is_float(void) const;
    bool is_number(void) const;
    bool is_symbol(void) const;
    bool is_string(void) const;
    bool is_array(void) const;
    bool is_list(void) const;
    bool is_dict(void) const;
    bool is_set(void) const;
    bool is_macro(void) const;
    bool is_lambda(void) const;
    bool is_function(void) const;
    bool is_func(void) const;
    bool is_callable(void) const;
    bool is_pair(void) const;

    bool as_bool(void) const;
    i64 as_integer(void) const;
    f64 as_float(void) const;
    Number as_number(void) const;
    Symbol as_symbol(void) const;
    String as_string(void) const;
    Array as_array(void) const;
    Dict as_dict(void) const;
    List as_list(void) const;
    Set as_set(void) const;
    Lambda as_lambda(void) const;
    Macro as_macro(void) const;
    Lambda as_function(void) const;
    Func as_func(void) const;
    Pair as_pair(void) const;

    std::string str(void) const;
    std::string repr(void) const;
    Object clone(void) const;

    bool is_hashable(void) const;
    bool is_iterable(void) const;
    Symbol type(void) const;

    friend bool operator==(const Object& lhs, const Object& rhs);
    friend bool operator!=(const Object& lhs, const Object& rhs);
    friend bool operator<=(const Object& lhs, const Object& rhs);
    friend bool operator>=(const Object& lhs, const Object& rhs);
    friend bool operator<(const Object& lhs, const Object& rhs);
    friend bool operator>(const Object& lhs, const Object& rhs);
    friend Object operator+(const Object& lhs, const Object& rhs);
    friend Object operator-(const Object& lhs, const Object& rhs);
    friend Object operator*(const Object& lhs, const Object& rhs);
    friend Object operator/(const Object& lhs, const Object& rhs);
    friend Object operator%(const Object& lhs, const Object& rhs);

    friend Object operator<<(const Object& lhs, const Object& rhs);
    friend Object operator>>(const Object& lhs, const Object& rhs);
    friend Object operator|(const Object& lhs, const Object& rhs);
    friend Object operator&(const Object& lhs, const Object& rhs);
    friend Object operator^(const Object& lhs, const Object& rhs);

    friend Object operator||(const Object& lhs, const Object& rhs);
    friend Object operator&&(const Object& lhs, const Object& rhs);

    Object logical_not(void) const;
    Object negate(void) const;
    Object bitwise_not(void) const;

private:
    // using Value = std::variant<Nil, bool, Number, String, Symbol, Func, Lambda, Macro,
    //     Pair, List, Array, Dict, Set>;

    using Value = std::any;
    Symbol m_type;
    Value m_value;
};

struct Pair final{
    Object key;
    Object val;
    std::string str(void) const;
    std::string repr(void) const;
    Pair clone(void) const;
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
    const Symbol& kind(void) const;
    Symbol& kind(void);

private:
    Symbol m_kind;
};

// -----------
// -*- Env -*-
// -----------
class Env final : public std::enable_shared_from_this<Env> {
public:
    Env(Context parent=nullptr);
    Env(const Env&) noexcept = default;
    Env(Env&&) noexcept = default;
    Env& operator=(const Env&) noexcept = default;
    Env& operator=(Env&&) noexcept = default;
    ~Env() = default;

    void define(const std::string& name, const Object& obj);
    bool update(const std::string& name, const Object& obj);
    Object get(const std::string& name);

    const std::map<std::string, Object>& bindings(void) const{
        return this->m_bindings;
    }

private:
    std::map<std::string, Object> m_bindings;
    Context m_parent;
};

// -*-
struct ExprBase{
    virtual ~ExprBase() = default;
    virtual Object eval(Visitor visitor) = 0;
    virtual std::string str(void) const = 0;
    virtual std::string repr(void) const = 0;
};

// -*-
struct LiteralExpr: public ExprBase{
    Object obj;
    virtual Object eval(Visitor visitor) override;
    std::string str(void) const override;
    std::string repr(void) const override;
};

// -*-
struct SymbolExpr: public ExprBase{
    Symbol name;
    virtual Object eval(Visitor visitor) override;
    std::string str(void) const override;
    std::string repr(void) const override;
};

struct ListExpr: public ExprBase{
    Vec<Expression> items;
    virtual Object eval(Visitor visitor) override;
    std::string str(void) const override;
    std::string repr(void) const override;
};

// -*-
struct ArrayExpr: public ExprBase{
    Vec<Expression> items;
    virtual Object eval(Visitor visitor) override;
    std::string str(void) const override;
    std::string repr(void) const override;
};

struct PairExpr: public ExprBase{
    Expression key;
    Expression val;
    virtual Object eval(Visitor visitor) override;
    std::string str(void) const override;
    std::string repr(void) const override;
};

struct DictExpr: public ExprBase{
    Vec<Expression> items;
    virtual Object eval(Visitor visitor) override;
    std::string str(void) const override;
    std::string repr(void) const override;
};

struct SetExpr: public ExprBase{
    Vec<Expression> items;
    virtual Object eval(Visitor visitor) override;
    std::string str(void) const override;
    std::string repr(void) const override;
};

// -*---------------------------------------------------------------------*-
// -*- ExprVisitor interface: a constract implementer by the interpreter -*-
// -*---------------------------------------------------------------------*-
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
class Module;
class ModuleHash;
class ModuleEqual;

struct BreakSignal final {};
struct ContinueSignal final {};

class ELix final: public ExprVisitor{
public:
    using ModuleSet = std::unordered_set<Module, ModuleHash, ModuleEqual>;
    
    static std::string scriptExt;
    static std::map<std::string, Vec<std::string>> docstrings;

    static Symbol Quote;
    static Symbol Unquote;
    static Symbol Quasiquote;
    static Symbol UnquoteSplicing;

    //static ExprVisitor* visitor;
    static Context prelude;
    
    explicit ELix() = default;
    ~ELix() = default;

    Object eval(Expression expr); // evalExprRaw

    static void setup(void);
    static bool is_reserved_word(const std::string& word);
    static bool is_literal(const Object& obj);
    static void repl(const Vec<Object>& args);
    static void run(const fs::path& scriptpath, const Vec<Object>& args);
    static std::string readfile(const fs::path& filename);
    static std::string readfile(const std::string& filename);
    static std::string readfile(const char* filename);
    static std::string input(void);

    //static Context runtime;
    static ModuleSet BuiltinModules;
    static void add_builtin_module(const Module& mymodule);
    static void validate_argc(bool pred, const std::string& prefix);
    static void validate_type(bool pred, const std::string& prefix, const std::string& emsg);
    static void validate_value(bool pred, const std::string& prefix, const std::string& emsg);

    const Context& runtime(void) const{ return this->m_runtime; }
    Context& runtime(void){ return this->m_runtime; }
private:
    //ModuleLoader m_moduleLoader{};
    Context m_runtime{nullptr};
    ModuleSet m_imported{};

    void load(const Symbol& name);
    void load(const fs::path& script);
    void load(const std::string& script);

    Object eval(LiteralExpr& expr) override;
    Object eval(SymbolExpr& expr) override;
    Object eval(ListExpr& expr) override;
    Object eval(ArrayExpr& expr) override;
    Object eval(DictExpr& expr) override;
    Object eval(SetExpr& expr) override;
    Object eval(PairExpr& expr) override;

    static void setup_prelude(void);
    static void initialize_constructors(void);
    static void initialize_predicates(void);
    static void initialize_operators(void);
    static void initialize_functional(void);
    static void initialize_basic_io(void);
    static void initialize_misc(void);
    static void initialize_string(void);
    static void initialize_symbol(void);
    static void initialize_pair(void);
    static void initialize_array(void);
    static void initialize_list(void);
    static void initialize_dict(void);
    static void initialize_set(void);
    static void initialize_math(void);

    static void init_builtin_modules();
    // static void initialize_file(void);
    // static void initialize_datetime(void);
    // static void initialize_json(void);
    // static void initialize_process(void);

    void print(const Object& obj);
    void println(const Object& obj);
    void check_argc(bool pred, const std::string& prefix);
    void validate_name(const std::string& name);
    void validate_expr(Expression& expr, const Vec<std::string>& exclude, const std::string& where);

    Expression build_expression(const Object& obj);

    // -*-
    Object handle_import(Vec<Expression> exprs);
    Object handle_progn(Vec<Expression> exprs);
    Object handle_if(Vec<Expression> exprs);
    Object handle_let(Vec<Expression> exprs);
    Object handle_var(Vec<Expression> exprs);
    Object handle_cond(Vec<Expression> exprs);
    Object handle_while(Vec<Expression> exprs);
    Object handle_for(Vec<Expression> exprs);
    Object handle_fun(Vec<Expression> exprs);
    Object handle_macro(Vec<Expression> exprs);
    Object handle_lambda(Vec<Expression> exprs);
    Object handle_and(Vec<Expression> exprs);
    Object handle_or(Vec<Expression> exprs);
    Object handle_not(Vec<Expression> exprs);
    Object handle_continue(Vec<Expression> exprs);
    Object handle_break(Vec<Expression> exprs);
    Expression handle_quote(Vec<Expression> exprs);
    Expression handle_quasiquote(Vec<Expression> exprs);
    Expression handle_unquote(Vec<Expression> exprs);
    Expression handle_unquote_splicing(Vec<Expression> exprs);
    Object handle_list(Vec<Expression> exprs); // function-call

    friend class Module;
    friend struct Macro;
    friend struct Lambda;
};

// -*-
class Module final{
public:
    explicit Module(const Symbol& name, ELix* elix);
    explicit Module(const std::string& filename, ELix* elix);
    explicit Module(const fs::path& filepath, ELix* elix);
    Module(const Module& mod) noexcept;
    Module(Module&& mod) noexcept;
    Module& operator=(const Module& mod) noexcept;
    Module& operator=(Module&& mod) noexcept;

    ~Module() = default;

    const std::string& name(void) const{ return this->m_name; }
    const std::string& filename(void) const{ return this->m_filename; };
    const fs::path& fullpath(void) const { return this->m_fullpath; }

    void load(Context& ctx) const;
    void add(const std::string& name, const Object& val);
    const Object& get(const std::string& name) const;

private:
    std::string m_name{};
    std::string m_filename{};
    fs::path m_fullpath{};
    std::map<std::string, Object> m_cache{};
    ELix* m_elix;

    void setup(void); // the cache
    void configure(const Symbol& name);
    void configure(const std::string& filename);
    void configure(const fs::path& filepath);
};

class ModuleHash final{
public:
    size_t operator()(const Module& arg) const;
};

class ModuleEqual final{
public:
    bool operator()(const Module& lhs, const Module& rhs) const;
};


// -*--------------------------------------------------------------------------*-
}//-*- end::namespace::ekasoft::elx                                           -*-
// -*--------------------------------------------------------------------------*-


#endif