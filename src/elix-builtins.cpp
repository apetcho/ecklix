#include "elix.hpp"

// -*--------------------------------------------------------------------------*-
// -*- begin::namespace::ekasoft::elx                                         -*-
// -*--------------------------------------------------------------------------*-
namespace ekasoft::elx{
// -

static void add_builtin(const std::string& name, Fn fn, int minArgc, int maxArgc, const std::string& docstr=""){
    Func func{fn, minArgc, maxArgc};
    ELix::prelude->define(name, Object(func));
    if(ELix::docstrings.find(name)==ELix::docstrings.end()){
        ELix::docstrings[name] = Vec<std::string>{docstr};
    }else{
        ELix::docstrings[name].push_back(docstr);
    }
}

static void add_builtin(const std::string& name, const Object& obj, const std::string& docstr=""){
    ELix::prelude->define(name, obj);
    if(ELix::docstrings.find(name)==ELix::docstrings.end()){
        ELix::docstrings[name] = Vec<std::string>{docstr};
    }else{
        ELix::docstrings[name].push_back(docstr);
    }
}

// -*----------------*-
// -*- Constructors -*-
// -*----------------*-
static Object fn_symbol(const Vec<Object>& args){
    // (symbol args)
    auto pred = (args.size()==1);
    
}

static Object fn_string(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_integer(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_float(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}
static Object fn_array(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashmap(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_pair(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
void ELix::initialize_constructors(void){
    //! @todo
    /*
static Object fn_symbol(const Vec<Object>& args);
static Object fn_string(const Vec<Object>& args);
static Object fn_integer(const Vec<Object>& args);
static Object fn_float(const Vec<Object>& args);
static Object fn_array(const Vec<Object>& args);
static Object fn_hashmap(const Vec<Object>& args);
static Object fn_hashset(const Vec<Object>& args);
static Object fn_list(const Vec<Object>& args);
static Object fn_pair(const Vec<Object>& args);
    */
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}


// -*----------------------*-
// -*- Builtin predicates -*-
// -*----------------------*-
static Object fn_is_empty(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_is_hashable(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_is_iterable(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_contains(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
void ELix::initialize_predicates(void){
    //! @todo
    /*
static Object fn_is_empty(const Vec<Object>& args);
static Object fn_is_hashable(const Vec<Object>& args);
static Object fn_is_iterable(const Vec<Object>& args);
static Object fn_contains(const Vec<Object>& args);
    */
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*---------------------*-
// -*- Builtin operators -*-
// -*---------------------*-
static Object fn_add(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_sub(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_mul(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_div(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_mod(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_band(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_bor(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_xor(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_lshift(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_rshift(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_not(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_and(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_or(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_lt(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_le(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_gt(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_ge(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_eq(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_ne(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
void ELix::initialize_operators(void){
    //! @todo
    /*
static Object fn_add(const Vec<Object>& args);
static Object fn_sub(const Vec<Object>& args);
static Object fn_mul(const Vec<Object>& args);
static Object fn_div(const Vec<Object>& args);
static Object fn_mod(const Vec<Object>& args);
static Object fn_band(const Vec<Object>& args);
static Object fn_bor(const Vec<Object>& args);
static Object fn_xor(const Vec<Object>& args);
static Object fn_lshift(const Vec<Object>& args);
static Object fn_rshift(const Vec<Object>& args);
static Object fn_not(const Vec<Object>& args);
static Object fn_and(const Vec<Object>& args);
static Object fn_or(const Vec<Object>& args);
static Object fn_lt(const Vec<Object>& args);
static Object fn_le(const Vec<Object>& args);
static Object fn_gt(const Vec<Object>& args);
static Object fn_ge(const Vec<Object>& args);
static Object fn_eq(const Vec<Object>& args);
static Object fn_ne(const Vec<Object>& args);
    */
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-------------------------------------*-
// -*- Basic functional programming APIs -*-
// -*-------------------------------------*-
static Object fn_map(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_filter(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_zip(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_enumerate(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_len(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_push(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_pop(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_clear(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_concat(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_reverse(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_find(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_min(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_max(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_range(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_linspace(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_repeat(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_partial(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
void ELix::initialize_functional(void){
    //! @todo
    /*
static Object fn_map(const Vec<Object>& args);
static Object fn_filter(const Vec<Object>& args);
static Object fn_zip(const Vec<Object>& args);
static Object fn_enumerate(const Vec<Object>& args);
static Object fn_len(const Vec<Object>& args);
static Object fn_push(const Vec<Object>& args);
static Object fn_pop(const Vec<Object>& args);
static Object fn_clear(const Vec<Object>& args);
static Object fn_concat(const Vec<Object>& args);
static Object fn_reverse(const Vec<Object>& args);
static Object fn_find(const Vec<Object>& args);
static Object fn_min(const Vec<Object>& args);
static Object fn_max(const Vec<Object>& args);
static Object fn_range(const Vec<Object>& args);
static Object fn_linspace(const Vec<Object>& args);
static Object fn_repeat(const Vec<Object>& args);
static Object fn_partial(const Vec<Object>& args);
    */
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*----------------------------------------*-
// -*- Basic builtin Input/Output functions -*-
// -*----------------------------------------*-
static Object fn_println(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_eprintln(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_print(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_eprint(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_input(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
void ELix::initialize_basic_io(void){
    //! @todo
    /*
static Object fn_println(const Vec<Object>& args);
static Object fn_eprintln(const Vec<Object>& args);
static Object fn_print(const Vec<Object>& args);
static Object fn_eprint(const Vec<Object>& args);
static Object fn_input(const Vec<Object>& args);
    */
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-----------------------------------*-
// -*- Miscellaneous builtin functions -*-
// -*-----------------------------------*-
static Object fn_clone(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_typeof(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_isinstance(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_symbols(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_macro_expand(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_ascii_chr(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_ascii_ord(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_panic(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
struct Formatter{
    explicit Formatter(const std::string& src) noexcept: m_src{src}{}
    
    ~Formatter() = default;

    // -*-
    std::string format(void) const{
        //! @todo
        throw ELixError(Symbol{"NotImplementedError"}, __func__);
    }

    const std::string& src(void) const;

private:
    std::string m_src;

    // -*-
    // (format "Some fancy text with {var} embedded")
    std::string format(const Object& obj) const{
        //! @todo
        throw ELixError(Symbol{"NotImplementedError"}, __func__);
    }

    // SPEC:
    //  'f': Float
    //  'e': Float in scientific notation
    //  'i': Integer
    //  's': String or Symbol
    //  'r': Raw string
    // (format "var: {var:f}")
    // (format "var: {var:e}")
    // (format "var: {var:i}")
    // (format "var: {var:s}")
    // (format "var: {var:r}")
    // (format "var: {var}")
    std::string format(const Object& obj, char spec) const{
        //! @todo
        throw ELixError(Symbol{"NotImplementedError"}, __func__);
    }

    // JUSTIFCATION:
    //  '^': Center-align
    //  '<': Left-align
    //  '>': Right-align
    // (format "var: {var:^f")
    // (format "var: {var:<i}")
    // (format "var: {var:>s}")
    std::string format(const Object& obj, char spec, char justify) const{
        //! @todo
        throw ELixError(Symbol{"NotImplementedError"}, __func__);
    }
    
    // (format "var: {var:20s}")
    // (format "var: {var:20f}")
    // (format "var: {var:20i}")
    std::string format(const Object& obj, char spec, u32 width) const{
        //! @todo
        throw ELixError(Symbol{"NotImplementedError"}, __func__);
    }

    // (format "var: {var:20s}")
    // (format "var: {var:20.3f}")
    // (format "var: {var:20i}")
    // (format "var: {var:<20s}")
    // (format "var: {var:>20.3f}")
    // (format "var: {var:^20i}")
    std::string format(const Object& obj, char spec_or_justify, u32 width, u32 prec) const{
        //! @todo
        throw ELixError(Symbol{"NotImplementedError"}, __func__);
    }

    // (format "var : {var:.3f")
    std::string format(const Object& obj, u32 width, u32 prec) const{
        //! @todo
        throw ELixError(Symbol{"NotImplementedError"}, __func__);
    }
    
    // (format "var: {var:^10.2f")
    std::string format(const Object& obj, char spec, char justify, u32 width, u32 prec) const{
        //! @todo
        throw ELixError(Symbol{"NotImplementedError"}, __func__);
    }

    enum class Kind{
        EOS,        // END_OF_STRING
        LBRACE,     // '{'
        RBRACE,     // '}'
        SYMBOL,     // SYMBOL,
        COLON,      // ':'
        DOT,        // '.'
        NUM,        // NUMBER
        STR,        // STRING
    };
    struct Token{
        Kind kind;
        u32 row;
        u32 col;
        std::string lexeme;
        i32 prec;
        i32 width;
        char justify;
        char spec;
    };

    Vec<Token> tokenize(void){
        //! @todo
        throw ELixError(Symbol{"NotImplementedError"}, __func__);
    }

    Object parse(const Vec<Token>& tokens){
        //! @todo
        throw ELixError(Symbol{"NotImplementedError"}, __func__);
    }
};

static Object fn_format(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_help(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_eval(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_getenv(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_setenv(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_quit(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_exit(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_bye(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
void ELix::initialize_misc(void){
    //! @todo
    /*
static Object fn_clone(const Vec<Object>& args);
static Object fn_typeof(const Vec<Object>& args);
static Object fn_isinstance(const Vec<Object>& args);
static Object fn_symbols(const Vec<Object>& args);
static Object fn_macro_expand(const Vec<Object>& args);
static Object fn_ascii_chr(const Vec<Object>& args);
static Object fn_ascii_ord(const Vec<Object>& args);
static Object fn_panic(const Vec<Object>& args);
static Object fn_format(const Vec<Object>& args);
static Object fn_help(const Vec<Object>& args);
static Object fn_eval(const Vec<Object>& args);
static Object fn_getenv(const Vec<Object>& args){}
static Object fn_setenv(const Vec<Object>& args){}
static Object fn_quit(const Vec<Object>& args){}
static Object fn_exit(const Vec<Object>& args){}
static Object fn_bye(const Vec<Object>& args){}
    */
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*---------------*-
// -*- String APIs -*-
// -*---------------*-
static Object fn_str_find(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_reverse(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_concat(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_len(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_eq(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_ne(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_split(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_startswith(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_endswith(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_ltrim(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_rtrim(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_trim(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_lower(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_upper(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_capitalize(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_title(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_find_all(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_replace(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_replace_all(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_substr(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_at(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

void ELix::initialize_string(void){
    //! @todo
    /*
static Object fn_str_find(const Vec<Object>& args);
static Object fn_str_reverse(const Vec<Object>& args);
static Object fn_str_concat(const Vec<Object>& args);
static Object fn_str_len(const Vec<Object>& args);
static Object fn_str_eq(const Vec<Object>& args);
static Object fn_str_ne(const Vec<Object>& args);
static Object fn_str_split(const Vec<Object>& args);
static Object fn_str_startswith(const Vec<Object>& args);
static Object fn_str_endswith(const Vec<Object>& args);
static Object fn_str_ltrim(const Vec<Object>& args);
static Object fn_str_rtrim(const Vec<Object>& args);
static Object fn_str_trim(const Vec<Object>& args);
static Object fn_str_lower(const Vec<Object>& args){}
static Object fn_str_upper(const Vec<Object>& args){}
static Object fn_str_capitalize(const Vec<Object>& args){}
static Object fn_str_title(const Vec<Object>& args){}
static Object fn_str_find_all(const Vec<Object>& args){}
static Object fn_str_replace(const Vec<Object>& args){}
static Object fn_str_replace_all(const Vec<Object>& args){}
static Object fn_str_substr(const Vec<Object>& args){}
static Object fn_str_at(const Vec<Object>& args){}
    */
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*---------------*-
// -*- Symbol APIs -*-
// -*---------------*-
static Object fn_symbol_eq(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_symbol_ne(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_symbol_name(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

void ELix::initialize_symbol(void){
    //! @todo
    /*
static Object fn_symbol_eq(const Vec<Object>& args){}
static Object fn_symbol_ne(const Vec<Object>& args){}
static Object fn_symbol_name(const Vec<Object>& args){}
    */
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-------------*-
// -*- Pair APIs -*-
// -*-------------*-
static Object fn_pair_key(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_pair_value(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

void ELix::initialize_pair(void){
    //! @todo
    /*
static Object fn_pair_key(const Vec<Object>& args){}
static Object fn_pair_value(const Vec<Object>& args){}
    */
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*--------------*-
// -*- Array APIs -*-
// -*--------------*-
static Object fn_array_find(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_reverse(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_concat(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_len(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_slice(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_insert(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_get(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_set(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_splice(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_any(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_all(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_sort(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_reduce(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_push(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_pop(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

void ELix::initialize_array(void){
    //! @todo
    /*
static Object fn_array_find(const Vec<Object>& args){}
static Object fn_array_reverse(const Vec<Object>& args){}
static Object fn_array_concat(const Vec<Object>& args){}
static Object fn_array_len(const Vec<Object>& args){}
static Object fn_array_slice(const Vec<Object>& args){}
static Object fn_array_insert(const Vec<Object>& args){}
static Object fn_array_get(const Vec<Object>& args){}
static Object fn_array_set(const Vec<Object>& args){}
static Object fn_array_splice(const Vec<Object>& args){}
static Object fn_array_any(const Vec<Object>& args){}
static Object fn_array_all(const Vec<Object>& args){}
static Object fn_array_sort(const Vec<Object>& args){}
static Object fn_array_reduce(const Vec<Object>& args){}
static Object fn_array_push(const Vec<Object>& args){}
static Object fn_array_pop(const Vec<Object>& args){}
    */
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-------------*-
// -*- List APIs -*-
// -*-------------*-
static Object fn_list_find(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_reverse(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_concat(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_len(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_head(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_tail(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_first(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_last(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_push(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_pop(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_insert(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_remove(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
void ELix::initialize_list(void){
    //! @todo
    /*
static Object fn_list_find(const Vec<Object>& args){}
static Object fn_list_reverse(const Vec<Object>& args){}
static Object fn_list_concat(const Vec<Object>& args){}
static Object fn_list_len(const Vec<Object>& args){}
static Object fn_list_head(const Vec<Object>& args){}
static Object fn_list_tail(const Vec<Object>& args){}
static Object fn_list_first(const Vec<Object>& args){}
static Object fn_list_last(const Vec<Object>& args){}
static Object fn_list_push(const Vec<Object>& args){}
static Object fn_list_pop(const Vec<Object>& args){}
static Object fn_list_insert(const Vec<Object>& args){}
static Object fn_list_remove(const Vec<Object>& args){}
    */
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-------------*-
// -*- Dict APIs -*-
// -*-------------*-
static Object fn_dict_find(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_concat(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_len(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_get(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_set(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_update(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_keys(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_values(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_popitem(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_clear(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_remove(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_items(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
void ELix::initialize_dict(void){
    //! @todo
    /*
static Object fn_dict_find(const Vec<Object>& args){}
static Object fn_dict_concat(const Vec<Object>& args){}
static Object fn_dict_len(const Vec<Object>& args){}
static Object fn_dict_get(const Vec<Object>& args){}
static Object fn_dict_set(const Vec<Object>& args){}
static Object fn_dict_update(const Vec<Object>& args){}
static Object fn_dict_keys(const Vec<Object>& args){}
static Object fn_dict_values(const Vec<Object>& args){}
static Object fn_dict_popitem(const Vec<Object>& args){}
static Object fn_dict_clear(const Vec<Object>& args){}
static Object fn_dict_remove(const Vec<Object>& args){}
static Object fn_dict_items(const Vec<Object>& args){}
    */
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*------------*-
// -*- Set APIs -*-
// -*------------*-
static Object fn_hashset_find(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_concat(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_len(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_add(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_union(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_intersection(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_symmetric_difference(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_clear(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_remove(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_pop(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_isdisjoint(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_issubset(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_issuperset(const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
void ELix::initialize_set(void){
    //! @todo
    /*
static Object fn_hashset_find(const Vec<Object>& args){}
static Object fn_hashset_concat(const Vec<Object>& args){}
static Object fn_hashset_len(const Vec<Object>& args){}
static Object fn_hashset_add(const Vec<Object>& args){}
static Object fn_hashset_union(const Vec<Object>& args){}
static Object fn_hashset_intersection(const Vec<Object>& args){}
static Object fn_hashset_symmetric_difference(const Vec<Object>& args){}
static Object fn_hashset_clear(const Vec<Object>& args){}
static Object fn_hashset_remove(const Vec<Object>& args){}
static Object fn_hashset_pop(const Vec<Object>& args){}
static Object fn_hashset_isdisjoint(const Vec<Object>& args){}
static Object fn_hashset_issubset(const Vec<Object>& args){}
static Object fn_hashset_issuperset(const Vec<Object>& args){}
    */
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*--------------------------------------------------------------------------*-
}//-*- end::namespace::ekasoft::elx                                           -*-
// -*--------------------------------------------------------------------------*-