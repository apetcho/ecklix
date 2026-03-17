#include "elix.hpp"
#include<algorithm>
#include<iostream>
#include<sstream>
#include<iomanip>
#include<random>
#include<cctype>
#include<numeric>
#include<cstdlib>
#include<stdlib.h>

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
static Object fn_symbol(const Vec<Object>& args, ELix* elix){
    // (Symbol args)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Symbol.new");
    pred = (args[0].is_symbol() || args[0].is_string());
    ELix::validate_type(pred, "Symbol.new", "argument must a String or a Symbol.");
    Object result{};
    if(args[0].is_symbol()){
        result = Object(args[0].as_symbol());
    }else{
        result = Object(Symbol{args[0].str()});
    }
    elix->runtime()->define(result.str(), result);
    return result;
}

static Object fn_string(const Vec<Object>& args, ELix* elix){
    /*
        (String.new)
        (String.new arg)

        type(arg) = Any
    */
    [[maybe_unused]] auto _ = elix;
    auto pred = (args.size()<=1);
    ELix::validate_argc(pred, "String.new");
    Object result{Object(String{""})};
    if(!args.empty()){
        result = Object(String{args[0].str()});
    }
    return result;
}

static Object fn_integer(const Vec<Object>& args, ELix* elix){
    // (Integer.new )
    // (Integer.new arg)
    auto pred = (args.size() <= 1);
    ELix::validate_argc(pred, "Integer.new");
    if(args.empty()){
        return Object(Number{});
    }
    pred = (
        args[0].is_bool() || args[0].is_number() ||
        args[0].is_string()
    );
    ELix::validate_type(pred, "Integer.new", "argument must be a boolean, a number or a string");
    auto arg = args[0];
    if(arg.is_bool()){
        auto num = (arg.as_bool() ? i64(1) : i64(0));
        return Object(Number{num});
    }else if(arg.is_number()){
        auto num = arg.as_integer();
        return Object(Number{num});
    }
    auto xstr = arg.as_string().split();
    if(xstr.size() > 1){
        std::stringstream ss;
        ss << "Invalid argument to `Integer.new': " << arg.str() << " is not a numeric string.";
        throw ELixError(ELixError::ValueError, ss.str());
    }
    auto numstr = xstr[0].str();
    Tokenizer tokenizer(numstr);
    Parser parser(tokenizer);
    auto self = parser.parse()[0]->eval(elix);
    if(!self.is_number()){
        std::stringstream ss;
        ss << "Invalid argument to `Integer.new': " << arg.str() << " is not a numeric string.";
        throw ELixError(ELixError::ValueError, ss.str());
    }
    auto num = self.as_integer();
    return Object(Number{num});
}

static Object fn_float(const Vec<Object>& args, ELix* elix){
    // (Float.new )
    // (Float.new arg)
    auto pred = (args.size() <= 1);
    ELix::validate_argc(pred, "Float.new");
    if(args.empty()){
        return Object(Number{0.0});
    }
    pred = (
        args[0].is_bool() || args[0].is_number() ||
        args[0].is_string()
    );
    ELix::validate_type(pred, "Float.new", "argument must be a boolean, a number or a string");
    auto arg = args[0];
    if(arg.is_bool()){
        auto num = (arg.as_bool() ? 1.0 : 0.0);
        return Object(Number{num});
    }else if(arg.is_number()){
        auto num = arg.as_float();
        return Object(Number{num});
    }
    auto xstr = arg.as_string().split();
    if(xstr.size() > 1){
        std::stringstream ss;
        ss << "Invalid argument to `Float.new': " << arg.str() << " is not a numeric string.";
        throw ELixError(ELixError::ValueError, ss.str());
    }
    auto numstr = xstr[0].str();
    Tokenizer tokenizer(numstr);
    Parser parser(tokenizer);
    auto self = parser.parse()[0]->eval(elix);
    if(!self.is_number()){
        std::stringstream ss;
        ss << "Invalid argument to `Float.new': " << arg.str() << " is not a numeric string.";
        throw ELixError(ELixError::ValueError, ss.str());
    }
    auto num = self.as_float();
    return Object(Number{num});
}

static Object fn_array(const Vec<Object>& args, ELix* elix){
    // (Array.new)
    // (Array.new size item)
    // (Array.new iterable)
    auto pred = (args.size() <= 2);
    ELix::validate_argc(pred, "Array.new");
    if(args.empty()){
        return Object(Array{});
    }
    if(args.size()==1){
        auto arg = args[0];
        if(!arg.is_iterable()){
            std::stringstream ss;
            ss << "`(Array.new arg)': expect an iterable object but got " << std::quoted(arg.type().str());
            throw ELixError(ELixError::TypeError, ss.str());
        }
        Vec<Object> vec{};
        if(arg.is_string()){
            auto text = arg.str();
            for(const auto& c: text){
                vec.push_back(Object{String{std::string(1, c)}});
            }
        }else if(arg.is_array()){
            return Object(arg.as_array());
        }else if(arg.is_list()){
            vec = arg.as_list().items;
        }else if(arg.is_set()){
            for(const auto& obj: arg.as_set().hset){
                vec.push_back(obj);
            }
        }else if(arg.is_dict()){
            for(const auto& [key, val]: arg.as_dict().hmap){
                vec.push_back(Object(Pair{key, val}));
            }
        }
        return Object(Array{vec});
    }else{
        if(!args[0].is_integer()){
            std::stringstream ss;
            ss << "`(Array.new size obj)': size must be an integer. Got " << std::quoted(args[0].type().str());
            throw ELixError(ELixError::TypeError, ss.str());
        }
        auto size = args[0].as_integer();
        if(size <= 0){
            std::stringstream ss;
            ss << "`(Array.new size obj)': Except size > 0 but got " << size;
            throw ELixError(ELixError::ValueError, ss.str());
        }
        Vec<Object> vec{};
        for(decltype(size) i=0; i < size; i++){
            vec.push_back(args[1]);
        }
        return Object(Array{vec});
    }
}

static Object fn_hashmap(const Vec<Object>& args, ELix* elix){
    // (Dict.new )
    // (Dict.new iterable)
    auto pred = (args.size() <= 1);
    ELix::validate_argc(pred, "Dict.new");
    if(args.empty()){
        return Object(Dict{});
    }
    auto arg = args[0];
    if(!arg.is_iterable()){
        std::stringstream ss;
        ss << "`(Dict.new iterable)': expect an iterable object as argument but got ";
        ss << std::quoted(arg.type().str());
        throw ELixError(ELixError::TypeError, ss.str());
    }
    // -*-
    HashMap hmap{};
    if(arg.is_string()){
        std::map<char, i64> dict{};
        auto text = arg.str();
        for(auto c: text){
            if(std::isspace(c)){ continue; }
            if(dict.find(c)==dict.end()){ dict[c] = 1; }
            else{ dict[c] += 1; }
        }
        for(const auto& [key, val]: dict){
            hmap[Object(String{std::string(1, key)})] = Object(Number{val});
        }
    }else if(arg.is_array()){
        auto vec = arg.as_array().items;
        for(const auto& entry: vec){
            if(!entry.is_pair()){
                std::stringstream ss;
                ss << "`(Dict.new iterable)': Expect the elements of the iterable to be a pair.\n";
                ss << "Got " << std::quoted(entry.type().str());
                throw ELixError(ELixError::TypeError, ss.str());
            }
            auto pair = entry.as_pair();
            hmap[pair.key] = pair.val;
        }
    }else if(arg.is_list()){
        auto vec = arg.as_list().items;
        for(const auto& entry: vec){
            if(!entry.is_pair()){
                std::stringstream ss;
                ss << "`(Dict.new iterable)': Expect the elements of the iterable to be a pair.\n";
                ss << "Got " << std::quoted(entry.type().str());
                throw ELixError(ELixError::TypeError, ss.str());
            }
            auto pair = entry.as_pair();
            hmap[pair.key] = pair.val;
        }
    }else if(arg.is_set()){
        for(const auto& entry: arg.as_set().hset){
            if(!entry.is_pair()){
                std::stringstream ss;
                ss << "`(Dict.new iterable)': Expect the elements of the iterable to be a pair.\n";
                ss << "Got " << std::quoted(entry.type().str());
                throw ELixError(ELixError::TypeError, ss.str());
            }
            auto pair = entry.as_pair();
            hmap[pair.key] = pair.val;
        }
    }else if(arg.is_dict()){
        hmap = arg.as_dict().hmap;
    }

    return Object(Dict{hmap});
}

// -*-
static Object fn_hashset(const Vec<Object>& args, ELix* elix){
    // (Set.new )
    // (Set.new iterable)
    auto pred = (args.size() <= 1);
    ELix::validate_argc(pred, "Set.new");
    if(args.empty()){
        return Object(Set{});
    }
    auto arg = args[0];
    if(!arg.is_iterable()){
        std::stringstream ss;
        ss << "`(Set.new iterable)': expect an iterable object as argument but got ";
        ss << std::quoted(arg.type().str());
        throw ELixError(ELixError::TypeError, ss.str());
    }
    // -*-
    HashSet hset{};
    if(arg.is_string()){
        std::set<char> myset{};
        auto text = arg.str();
        for(auto c: text){
            if(std::isspace(c)){ continue; }
            myset.insert(c);
        }
        for(const auto& c: myset){
            hset.insert(Object(String{std::string(1, c)}));
        }
    }else if(arg.is_array()){
        auto vec = arg.as_array().items;
        for(const auto& entry: vec){
            hset.insert(entry);
        }
    }else if(arg.is_list()){
        auto vec = arg.as_list().items;
        for(const auto& entry: vec){
            hset.insert(entry);
        }
    }else if(arg.is_set()){
        hset = arg.as_set().hset;
    }else if(arg.is_dict()){
        for(auto& [key, val]: arg.as_dict().hmap){
            hset.insert(Object(Pair{key, val}));
        }
    }

    return Object(Set{hset});
}

static Object fn_list(const Vec<Object>& args, ELix* elix){
    // (List.new)
    // (List.new size item)
    // (List.new iterable)
    auto pred = (args.size() <= 2);
    ELix::validate_argc(pred, "List.new");
    if(args.empty()){
        return Object(List{});
    }
    if(args.size()==1){
        auto arg = args[0];
        if(!arg.is_iterable()){
            std::stringstream ss;
            ss << "`(List.new arg)': expect an iterable object but got " << std::quoted(arg.type().str());
            throw ELixError(ELixError::TypeError, ss.str());
        }
        Vec<Object> vec{};
        if(arg.is_string()){
            auto text = arg.str();
            for(const auto& c: text){
                vec.push_back(Object{String{std::string(1, c)}});
            }
        }else if(arg.is_array()){
            vec = arg.as_array().items;
        }else if(arg.is_list()){
            vec = arg.as_list().items;
        }else if(arg.is_set()){
            for(const auto& obj: arg.as_set().hset){
                vec.push_back(obj);
            }
        }else if(arg.is_dict()){
            for(const auto& [key, val]: arg.as_dict().hmap){
                vec.push_back(Object(Pair{key, val}));
            }
        }
        return Object(List{vec});
    }else{
        if(!args[0].is_integer()){
            std::stringstream ss;
            ss << "`(List.new size obj)': size must be an integer. Got " << std::quoted(args[0].type().str());
            throw ELixError(ELixError::TypeError, ss.str());
        }
        auto size = args[0].as_integer();
        if(size <= 0){
            std::stringstream ss;
            ss << "`(List.new size obj)': Except size > 0 but got " << size;
            throw ELixError(ELixError::ValueError, ss.str());
        }
        Vec<Object> vec{};
        for(decltype(size) i=0; i < size; i++){
            vec.push_back(args[1]);
        }
        return Object(List{vec});
    }
}

static Object fn_pair(const Vec<Object>& args, ELix* elix){
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "Pair.new");
    Pair pair{args[0], args[1]};

    return Object(pair);
}

// -*-
void ELix::initialize_constructors(void){
    add_builtin("Symbol.new", fn_symbol, 1, 1);
    add_builtin("String.new", fn_string, 0, 1);
    add_builtin("Integer.new", fn_symbol, 0, 1);
    add_builtin("Float.new", fn_symbol, 0, 1);
    add_builtin("Array.new", fn_symbol, 0, 2);
    add_builtin("Set.new", fn_symbol, 0, 1);
    add_builtin("Dict.new", fn_symbol, 0, 1);
    add_builtin("List.new", fn_symbol, 0, 2);
    add_builtin("Pair.new", fn_symbol, 2, 2);
}


// -*----------------------*-
// -*- Builtin predicates -*-
// -*----------------------*-
static Object fn_is_empty(const Vec<Object>& args, ELix* elix){
    // (empty? arg)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "empty?");
    auto arg = args[0];
    if(!arg.is_iterable()){
        std::stringstream ss;
        ss << "`empty?': expect an iterable as its arguments";
        throw ELixError(ELixError::TypeError, ss.str());
    }
    if(arg.is_string()){
        return Object((arg.as_string().len()==0 ? true : false));
    }else if(arg.is_list()){
        return Object((arg.as_list().len()==0 ? true : false));
    }else if(arg.is_array()){
        return Object((arg.as_array().len()==0 ? true : false));
    }else if(arg.is_set()){
        return Object((arg.as_set().len()==0 ? true : false));
    }else{
        return Object((arg.as_dict().len()==0 ? true : false));
    }
}

static Object fn_is_hashable(const Vec<Object>& args, ELix* elix){
    // (hashable? arg)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "hashable?");
    auto arg = args[0];

    return Object((arg.is_hashable() ? true : false));
}

static Object fn_is_iterable(const Vec<Object>& args, ELix* elix){
    // (iterable? arg)
    auto pred = (args.size()==0);
    ELix::validate_argc(pred, "iterable?");
    auto arg = args[0];

    return Object(arg.is_iterable() ? true : false);
}

static Object fn_contains(const Vec<Object>& args, ELix* elix){
    // (contains? container item)
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "contains?");
    auto container = args[0];
    auto item = args[1];
    if(!container.is_iterable()){
        std::stringstream ss;
        ss << "`(contains? iterable item)': expect the first argument to be an iterable.";
        throw ELixError(ELixError::TypeError, ss.str());
    }
    if(container.is_string()){
        if(!item.is_string()){
            std::stringstream ss;
            ss << "`(contains? iterable item)': the iterable is a String object but not the `item'.";
            ss << "\nExpect `item' to be a String. Got " << std::quoted(item.type().str()) << " object type.";
            throw ELixError(ELixError::TypeError, ss.str());
        }
        auto ans = container.as_string().find(item.as_string());
        if(ans==-1){ return Object(false); }
        return Object(true);
    }

    if(container.is_array()){
        auto ans = container.as_array().find(item);
        if(ans==-1){ return Object(false); }
        return Object(true);
    }

    if(container.is_list()){
        auto ans = container.as_list().find(item);
        if(ans==-1){ return Object(false); }
        return Object(true);
    }

    if(container.is_set()){
        auto ans = container.as_set().find(item);
        return Object(ans);
    }

    auto ans = container.as_dict().find(item);
    return Object(ans);
}

// -*-
void ELix::initialize_predicates(void){
    add_builtin("empty?", fn_is_empty, 1, 1);
    add_builtin("hashable?", fn_is_hashable, 1, 1);
    add_builtin("iterable?", fn_is_iterable, 1, 1);
    add_builtin("contains?", fn_contains, 1, 1);
}

// -*---------------------*-
// -*- Builtin operators -*-
// -*---------------------*-
static Object fn_add(const Vec<Object>& args, ELix* elix){
    // (+ x ...)
    auto pred = (args.size() >= 1);
    ELix::validate_argc(pred, "+");
    if(args.size()==1){
        auto arg = args[0];
        if(!arg.is_number()){
            std::stringstream ss;
            ss << "`(+ arg)': expect arg to be a number.";
            throw ELixError(ELixError::TypeError, ss.str());
        }
        return Object(arg.as_number());
    }
    auto acc = args[0];
    for(size_t i=1; i < args.size(); i++){
        acc = acc + args[i];
    }
    return Object(acc);
}

static Object fn_sub(const Vec<Object>& args, ELix* elix){
    // (- x ...)
    auto pred = (args.size() >= 1);
    ELix::validate_argc(pred, "-");
    if(args.size()==1){
        return Object(args[0].negate());
    }
    auto acc = args[0];
    for(size_t i=1; i < args.size(); i++){
        acc = acc - args[i];
    }
    return Object(acc);
}

static Object fn_mul(const Vec<Object>& args, ELix* elix){
    // (* x ...)
    auto pred = (args.size() >= 2);
    ELix::validate_argc(pred, "*");
    auto acc = args[0];
    for(size_t i=1; i < args.size(); i++){
        acc = acc * args[i];
    }
    return Object(acc);
}

static Object fn_div(const Vec<Object>& args, ELix* elix){
    // (/ x ...)
    auto pred = (args.size() >= 2);
    ELix::validate_argc(pred, "/");
    auto acc = args[0];
    for(size_t i=1; i < args.size(); i++){
        acc = acc / args[i];
    }
    return Object(acc);
}

static Object fn_mod(const Vec<Object>& args, ELix* elix){
    // (% x ...)
    auto pred = (args.size() >= 2);
    ELix::validate_argc(pred, "%");
    auto acc = args[0];
    for(size_t i=1; i < args.size(); i++){
        acc = acc % args[i];
    }
    return Object(acc);
}

static Object fn_band(const Vec<Object>& args, ELix* elix){
    // (~ x)
    auto pred = (args.size() == 1);
    ELix::validate_argc(pred, "~");
    auto acc = (args[0].bitwise_not());
    return Object(acc);
}

static Object fn_bor(const Vec<Object>& args, ELix* elix){
    // (| x y)
    auto pred = (args.size() == 2);
    ELix::validate_argc(pred, "|");
    auto acc = (args[0] | args[1]);
    return Object(acc);
}

static Object fn_xor(const Vec<Object>& args, ELix* elix){
    // (^ x y)
    auto pred = (args.size() == 2);
    ELix::validate_argc(pred, "^");
    auto acc = (args[0] ^ args[1]);

    return Object(acc);
}

static Object fn_lshift(const Vec<Object>& args, ELix* elix){
    // (<< x y)
    auto pred = (args.size() == 2);
    ELix::validate_argc(pred, "<<");
    auto acc = (args[0] << args[1]);
    return Object(acc);
}

static Object fn_rshift(const Vec<Object>& args, ELix* elix){
    // (>> x y)
    auto pred = (args.size() >= 2);
    ELix::validate_argc(pred, ">>");
    auto acc = (args[0] >> args[1]);
    return Object(acc);
}

static Object fn_not(const Vec<Object>& args, ELix* elix){
    // (not x)
    auto pred = (args.size() == 1);
    ELix::validate_argc(pred, "not");
    auto acc = args[0].logical_not();
    return Object(acc);
}

static Object fn_and(const Vec<Object>& args, ELix* elix){
    // (and x y)
    auto pred = (args.size() == 2);
    ELix::validate_argc(pred, "and");
    auto acc = (args[0] && args[1]);
    return Object(acc);
}

static Object fn_or(const Vec<Object>& args, ELix* elix){
    // (or x y)
    auto pred = (args.size() == 2);
    ELix::validate_argc(pred, "or");
    auto acc = (args[0] || args[1]);
    return Object(acc);
}

static Object fn_lt(const Vec<Object>& args, ELix* elix){
    // (< x y)
    auto pred = (args.size() == 2);
    ELix::validate_argc(pred, "<");
    auto acc = (args[0] < args[1]);
    
    return Object(acc);
}

static Object fn_le(const Vec<Object>& args, ELix* elix){
    // (<= x y)
    auto pred = (args.size() == 2);
    ELix::validate_argc(pred, "<=");
    auto acc = (args[0] <= args[1]);
    return Object(acc);
}

static Object fn_gt(const Vec<Object>& args, ELix* elix){
    // (> x y)
    auto pred = (args.size() == 2);
    ELix::validate_argc(pred, ">");
    auto acc = (args[0] > args[1]);
    return Object(acc);
}

static Object fn_ge(const Vec<Object>& args, ELix* elix){
    // (>= x y)
    auto pred = (args.size() == 2);
    ELix::validate_argc(pred, ">=");
    auto acc = (args[0] >= args[1]);
    return Object(acc);
}

static Object fn_eq(const Vec<Object>& args, ELix* elix){
    // (== x y)
    auto pred = (args.size() == 2);
    ELix::validate_argc(pred, "==");
    auto acc = (args[0]==args[1]);
    return Object(acc);
}

static Object fn_ne(const Vec<Object>& args, ELix* elix){
    // (!= x y)
    auto pred = (args.size() == 2);
    ELix::validate_argc(pred, "!=");
    auto acc = (args[0] != args[1]);
    return Object(acc);
}

// -*-
void ELix::initialize_operators(void){
    add_builtin("+", fn_add, 1, -1);
    add_builtin("-", fn_sub, 1, -1);
    add_builtin("*", fn_mul, 2, -1);
    add_builtin("/", fn_div, 2, -1);
    add_builtin("%", fn_mod, 1, -1);
    add_builtin("~", fn_band, 1, 1);
    add_builtin("|", fn_bor, 2, 2);
    add_builtin("^", fn_xor, 2, 2);
    add_builtin("<<", fn_lshift, 2, 2);
    add_builtin(">>", fn_rshift, 2, 2);
    add_builtin("not", fn_not, 1, 1);
    add_builtin("and", fn_and, 2, 2);
    add_builtin("or", fn_or, 2, 2);
    add_builtin("<", fn_lt, 2, 2);
    add_builtin("<=", fn_le, 2, 2);
    add_builtin(">", fn_gt, 2, 2);
    add_builtin(">=", fn_ge, 2, 2);
    add_builtin("==", fn_eq, 2, 2);
    add_builtin("!=", fn_ne, 2, 2);
}

// -*-------------------------------------*-
// -*- Basic functional programming APIs -*-
// -*-------------------------------------*-
static Object fn_map(const Vec<Object>& args, ELix* elix){
    // (map callable iterable)
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "map");
    auto callable = args[0];
    if(!callable.is_callable()){
        std::stringstream ss;
        ss << "`(map arg1 arg2)': expected the first argument `arg1' to be a callable.\n";
        ss << "Got " << std::quoted(callable.type().str()) << " object type.";
        throw ELixError(ELixError::TypeError, ss.str());
    }

    auto iterable = args[1];
    if(!iterable.is_iterable()){
        std::stringstream ss;
        ss << "`(map arg1 arg2)': expected the second argument `arg2' to be an iterable.\n";
        ss << "Got " << std::quoted(callable.type().str()) << " object type.";
        throw ELixError(ELixError::TypeError, ss.str());
    }

    Object result{};
    if(callable.is_lambda() || callable.is_function()){
        auto fun = callable.as_lambda();
        if(iterable.is_string()){
            [[maybe_unused]] auto _ = std::transform(
                iterable.as_string().text.begin(), iterable.as_string().text.end(),
                iterable.as_string().text.begin(),
                [&fun, elix](const Object& obj) mutable {
                    return fun(Vec<Object>{obj}, elix);
                }
            );
            result = Object(iterable);
        }else if(iterable.is_array()){
            [[maybe_unused]] auto _ = std::transform(
                iterable.as_array().items.begin(), iterable.as_array().items.end(),
                iterable.as_array().items.begin(),
                [&fun, elix](const Object& obj) mutable {
                    return fun(Vec<Object>{obj}, elix);
                }
            );
            result = Object(iterable);
        }else if(iterable.is_list()){
            [[maybe_unused]] auto _ = std::transform(
                iterable.as_list().items.begin(), iterable.as_list().items.end(),
                iterable.as_list().items.begin(),
                [&fun, elix](const Object& obj) mutable {
                    return fun(Vec<Object>{obj}, elix);
                }
            );
            result = Object(iterable);
        }else if(iterable.is_set()){
            [[maybe_unused]] auto _ = std::transform(
                iterable.as_set().hset.begin(), iterable.as_set().hset.end(),
                iterable.as_set().hset.begin(),
                [&fun, elix](const Object& obj) mutable {
                    return fun(Vec<Object>{obj}, elix);
                }
            );
            result = Object(iterable);
        }else{
            [[maybe_unused]] auto _ = std::transform(
                iterable.as_dict().hmap.begin(), iterable.as_dict().hmap.end(),
                iterable.as_dict().hmap.begin(),
                [&fun, elix](const Object& obj) mutable {
                    return fun(Vec<Object>{obj}, elix);
                }
            );
            result = Object(iterable);
        }
    }else if(callable.is_func()){
        auto fun = callable.as_func();
        if(iterable.is_string()){
            [[maybe_unused]] auto _ = std::transform(
                iterable.as_string().text.begin(), iterable.as_string().text.end(),
                iterable.as_string().text.begin(),
                [&fun, elix](const Object& obj) mutable {
                    return fun(Vec<Object>{obj}, elix);
                }
            );
            result = Object(iterable);
        }else if(iterable.is_array()){
            [[maybe_unused]] auto _ = std::transform(
                iterable.as_array().items.begin(), iterable.as_array().items.end(),
                iterable.as_array().items.begin(),
                [&fun, elix](const Object& obj) mutable {
                    return fun(Vec<Object>{obj}, elix);
                }
            );
            result = Object(iterable);
        }else if(iterable.is_list()){
            [[maybe_unused]] auto _ = std::transform(
                iterable.as_list().items.begin(), iterable.as_list().items.end(),
                iterable.as_list().items.begin(),
                [&fun, elix](const Object& obj) mutable {
                    return fun(Vec<Object>{obj}, elix);
                }
            );
            result = Object(iterable);
        }else if(iterable.is_set()){
            [[maybe_unused]] auto _ = std::transform(
                iterable.as_set().hset.begin(), iterable.as_set().hset.end(),
                iterable.as_set().hset.begin(),
                [&fun, elix](const Object& obj) mutable {
                    return fun(Vec<Object>{obj}, elix);
                }
            );
            result = Object(iterable);
        }else{
            [[maybe_unused]] auto _ = std::transform(
                iterable.as_dict().hmap.begin(), iterable.as_dict().hmap.end(),
                iterable.as_dict().hmap.begin(),
                [&fun, elix](const Object& obj) mutable {
                    return fun(Vec<Object>{obj}, elix);
                }
            );
            result = Object(iterable);
        }
    }else{
        auto fun = callable.as_macro();
        if(iterable.is_string()){
            [[maybe_unused]] auto _ = std::transform(
                iterable.as_string().text.begin(), iterable.as_string().text.end(),
                iterable.as_string().text.begin(),
                [&fun, elix](const Object& obj) mutable {
                    return fun(Vec<Object>{obj}, elix);
                }
            );
            result = Object(iterable);
        }else if(iterable.is_array()){
            [[maybe_unused]] auto _ = std::transform(
                iterable.as_array().items.begin(), iterable.as_array().items.end(),
                iterable.as_array().items.begin(),
                [&fun, elix](const Object& obj) mutable {
                    return fun(Vec<Object>{obj}, elix);
                }
            );
            result = Object(iterable);
        }else if(iterable.is_list()){
            [[maybe_unused]] auto _ = std::transform(
                iterable.as_list().items.begin(), iterable.as_list().items.end(),
                iterable.as_list().items.begin(),
                [&fun, elix](const Object& obj) mutable {
                    return fun(Vec<Object>{obj}, elix);
                }
            );
            result = Object(iterable);
        }else if(iterable.is_set()){
            [[maybe_unused]] auto _ = std::transform(
                iterable.as_set().hset.begin(), iterable.as_set().hset.end(),
                iterable.as_set().hset.begin(),
                [&fun, elix](const Object& obj) mutable {
                    return fun(Vec<Object>{obj}, elix);
                }
            );
            result = Object(iterable);
        }else{
            [[maybe_unused]] auto _ = std::transform(
                iterable.as_dict().hmap.begin(), iterable.as_dict().hmap.end(),
                iterable.as_dict().hmap.begin(),
                [&fun, elix](const Object& obj) mutable {
                    return fun(Vec<Object>{obj}, elix);
                }
            );
            result = Object(iterable);
        }
    }

    return result;
}

static Object fn_filter(const Vec<Object>& args, ELix* elix){
    // (filter callable iterable)
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "filter");
    auto callable = args[0];
    if(!callable.is_callable()){
        std::stringstream ss;
        ss << "`(filter arg1 arg2)': expected the first argument `arg1' to be a callable.\n";
        ss << "Got " << std::quoted(callable.type().str()) << " object type.";
        throw ELixError(ELixError::TypeError, ss.str());
    }

    auto iterable = args[1];
    if(!iterable.is_iterable()){
        std::stringstream ss;
        ss << "`(filter arg1 arg2)': expected the second argument `arg2' to be an iterable.\n";
        ss << "Got " << std::quoted(callable.type().str()) << " object type.";
        throw ELixError(ELixError::TypeError, ss.str());
    }

    Object result{};
    if(callable.is_lambda() || callable.is_function()){
        auto fun = callable.as_lambda();
        if(iterable.is_string()){
            auto ptr = std::remove_if(
                iterable.as_string().text.begin(), iterable.as_string().text.end(),
                [&fun, elix](const Object& obj) mutable {
                    auto argv = Vec<Object>{obj};
                    ELix::validate_predicate(fun, argv, elix, "filter");
                    return fun(argv, elix);
                }
            );
            iterable.as_string().text.erase(ptr, iterable.as_string().text.end());
            result = Object(iterable);
        }else if(iterable.is_array()){
            auto ptr = std::remove_if(
                iterable.as_array().items.begin(), iterable.as_array().items.end(),
                [&fun, elix](const Object& obj) mutable {
                    auto argv = Vec<Object>{obj};
                    ELix::validate_predicate(fun, argv, elix, "filter");
                    return fun(argv, elix);
                }
            );
            iterable.as_array().items.erase(ptr, iterable.as_array().items.end());
            result = Object(iterable);
        }else if(iterable.is_list()){
            auto ptr = std::remove_if(
                iterable.as_list().items.begin(), iterable.as_list().items.end(),
                [&fun, elix](const Object& obj) mutable {
                    auto argv = Vec<Object>{obj};
                    ELix::validate_predicate(fun, argv, elix, "filter");
                    return fun(argv, elix);
                }
            );
            iterable.as_list().items.erase(ptr, iterable.as_list().items.end());
            result = Object(iterable);
        }else if(iterable.is_set()){
            auto ptr = std::remove_if(
                iterable.as_set().hset.begin(), iterable.as_set().hset.end(),
                [&fun, elix](const Object& obj) mutable {
                    auto argv = Vec<Object>{obj};
                    ELix::validate_predicate(fun, argv, elix, "filter");
                    return fun(argv, elix);
                }
            );
            iterable.as_set().hset.erase(ptr, iterable.as_set().hset.end());
            result = Object(iterable);
        }else{
            auto ptr = std::remove_if(
                iterable.as_dict().hmap.begin(), iterable.as_dict().hmap.end(),
                [&fun, elix](const Object& obj) mutable {
                    auto argv = Vec<Object>{obj};
                    ELix::validate_predicate(fun, argv, elix, "filter");
                    return fun(argv, elix);
                }
            );
            iterable.as_dict().hmap.erase(ptr, iterable.as_dict().hmap.end());
            result = Object(iterable);
        }
    }else if(callable.is_func()){
        auto fun = callable.as_func();
        if(iterable.is_string()){
            auto ptr = std::remove_if(
                iterable.as_string().text.begin(), iterable.as_string().text.end(),
                [&fun, elix](const Object& obj) mutable {
                    auto argv = Vec<Object>{obj};
                    ELix::validate_predicate(fun, argv, elix, "filter");
                    return fun(argv, elix);
                }
            );
            iterable.as_string().text.erase(ptr, iterable.as_string().text.end());
            result = Object(iterable);
        }else if(iterable.is_array()){
            auto ptr = std::remove_if(
                iterable.as_array().items.begin(), iterable.as_array().items.end(),
                [&fun, elix](const Object& obj) mutable {
                    auto argv = Vec<Object>{obj};
                    ELix::validate_predicate(fun, argv, elix, "filter");
                    return fun(argv, elix);
                }
            );
            iterable.as_array().items.erase(ptr, iterable.as_array().items.end());
            result = Object(iterable);
        }else if(iterable.is_list()){
            auto ptr = std::remove_if(
                iterable.as_list().items.begin(), iterable.as_list().items.end(),
                [&fun, elix](const Object& obj) mutable {
                    auto argv = Vec<Object>{obj};
                    ELix::validate_predicate(fun, argv, elix, "filter");
                    return fun(argv, elix);
                }
            );
            iterable.as_list().items.erase(ptr, iterable.as_list().items.end());
            result = Object(iterable);
        }else if(iterable.is_set()){
            auto ptr = std::remove_if(
                iterable.as_set().hset.begin(), iterable.as_set().hset.end(),
                [&fun, elix](const Object& obj) mutable {
                    auto argv = Vec<Object>{obj};
                    ELix::validate_predicate(fun, argv, elix, "filter");
                    return fun(argv, elix);
                }
            );
            iterable.as_set().hset.erase(ptr, iterable.as_set().hset.end());
            result = Object(iterable);
        }else{
            auto ptr = std::remove_if(
                iterable.as_dict().hmap.begin(), iterable.as_dict().hmap.end(),
                [&fun, elix](const Object& obj) mutable {
                    auto argv = Vec<Object>{obj};
                    ELix::validate_predicate(fun, argv, elix, "filter");
                    return fun(argv, elix);
                }
            );
            iterable.as_dict().hmap.erase(ptr, iterable.as_dict().hmap.end());
            result = Object(iterable);
        }
    }else{
        auto fun = callable.as_macro();
        if(iterable.is_string()){
            auto ptr = std::remove_if(
                iterable.as_string().text.begin(), iterable.as_string().text.end(),
                [&fun, elix](const Object& obj) mutable {
                    auto argv = Vec<Object>{obj};
                    ELix::validate_predicate(fun, argv, elix, "filter");
                    return fun(argv, elix);
                }
            );
            iterable.as_string().text.erase(ptr, iterable.as_string().text.end());
            result = Object(iterable);
        }else if(iterable.is_array()){
            auto ptr = std::remove_if(
                iterable.as_array().items.begin(), iterable.as_array().items.end(),
                [&fun, elix](const Object& obj) mutable {
                    return fun(Vec<Object>{obj}, elix);
                }
            );
            iterable.as_array().items.erase(ptr, iterable.as_array().items.end());
            result = Object(iterable);
        }else if(iterable.is_list()){
            auto ptr = std::remove_if(
                iterable.as_list().items.begin(), iterable.as_list().items.end(),
                [&fun, elix](const Object& obj) mutable {
                    auto argv = Vec<Object>{obj};
                    ELix::validate_predicate(fun, argv, elix, "filter");
                    return fun(argv, elix);
                }
            );
            iterable.as_list().items.erase(ptr, iterable.as_list().items.end());
            result = Object(iterable);
        }else if(iterable.is_set()){
            auto ptr = std::remove_if(
                iterable.as_set().hset.begin(), iterable.as_set().hset.end(),
                [&fun, elix](const Object& obj) mutable {
                    auto argv = Vec<Object>{obj};
                    ELix::validate_predicate(fun, argv, elix, "filter");
                    return fun(argv, elix);
                }
            );
            iterable.as_set().hset.erase(ptr, iterable.as_set().hset.end());
            result = Object(iterable);
        }else{
            auto ptr = std::remove_if(
                iterable.as_dict().hmap.begin(), iterable.as_dict().hmap.end(),
                [&fun, elix](const Object& obj) mutable {
                    auto argv = Vec<Object>{obj};
                    ELix::validate_predicate(fun, argv, elix, "filter");
                    return fun(argv, elix);
                }
            );
            iterable.as_dict().hmap.erase(ptr, iterable.as_dict().hmap.end());
            result = Object(iterable);
        }
    }

    return result;
}

// -*-
static Object fn_zip(const Vec<Object>& args, ELix* elix){
    // (zip iterable1 iterable2 ... iterableN)
    auto pred = (args.size() >= 2);
    ELix::validate_argc(pred, "zip");
    Vec<i64> sizes{};
    auto validate = [](const Object& arg){
        return (arg.is_list() || arg.is_array() || arg.is_string());
    };
    Vec<Object> hsetVec{};
    for(auto arg: args){
        if(!validate(arg)){
            std::stringstream ss;
            ss << "`(zip arg1 arg2 ... argN)': expect all argument to `zip' to be ";
            ss << "a sequenced iterable,\ni.e a String, a List or an Array object.";
            throw ELixError(ELixError::TypeError, ss.str());
        }
        if(arg.is_string()){ sizes.push_back(arg.as_string().len()); }
        else if(arg.is_array()){ sizes.push_back(arg.as_array().len()); }
        else{ sizes.push_back(arg.as_list().len()); }
    }
    std::sort(sizes.begin(), sizes.end(), [](i64 x, i64 y){ return x < y; });
    auto size = sizes[0];
    Vec<Object> result{};
    for(decltype(size) i=0; i < size; i++){
        Vec<Object> entry{};
        for(auto arg: args){
            if(arg.is_string()){
                entry.push_back(Object(String{std::string(1, arg.as_string().text[i])}));
            }else if(arg.is_array()){
                entry.push_back(arg.as_array().items[i]);
            }else if(arg.is_list()){
                entry.push_back(arg.as_list().items[i]);
            }
        }
        result.push_back(Object(Array{entry}));
    }

    return Object(Array{result});
}

static Object fn_enumerate(const Vec<Object>& args, ELix* elix){
    // (enumerate iterable)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "enumerate");
    auto arg = args[0];
    if(!arg.is_iterable()){
        std::stringstream ss;
        ss << "`(enumerate arg)': expect argument to be an iterable.";
        throw ELixError(ELixError::TypeError, ss.str());
    }
    Vec<Object> result{};
    if(arg.is_string()){
        for(i64 i=0; i < arg.as_string().len(); i++){
            Pair entry{
                Object(Number(i)),
                Object(String{std::string(1, arg.as_string().text[i])})
            };
            result.push_back(std::move(entry));
        }
    }else if(arg.is_array()){
        for(i64 i=0; i < arg.as_array().len(); i++){
            Pair entry{ Object(Number(i)), arg.as_array().items[i] };
            result.push_back(std::move(entry));
        }
    }else if(arg.is_list()){
        for(i64 i=0; i < arg.as_list().len(); i++){
            Pair entry{ Object(Number(i)), arg.as_list().items[i] };
            result.push_back(std::move(entry));
        }
    }else if(arg.is_set()){
        i64 i = 0;
        for(const auto& obj: arg.as_set().hset){
            Pair entry{ Object(Number(i)),  obj};
            result.push_back(std::move(entry));
            ++i;
        }
    }else{
        i64 i = 0;
        for(auto pair: arg.as_dict().items()){
            Pair entry{ Object(Number(i)),  Object(pair)};
            result.push_back(std::move(entry));
            ++i;
        }
    }

    return Object(Array{result});
}

static Object fn_len(const Vec<Object>& args, ELix* elix){
    // (len iterable)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "len");
    auto arg = args[0];
    ELix::validate_type(arg.is_iterable(), "`(len arg)'", "expect argument to an iterable object.");
    i64 size{};

    if(arg.is_string()){ size = arg.as_string().len(); }
    else if(arg.is_array()){ size = arg.as_array().len(); }
    else if(arg.is_list()){ size = arg.as_list().len(); }
    else if(arg.is_set()){ size = arg.as_set().len(); }
    else{ size = arg.as_dict().len(); }
    return Object(Number(size));
}

// -*-
static Object fn_push(const Vec<Object>& args, ELix* elix){
    // (push container arg)
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "push");
    auto container = args[0];
    ELix::validate_type(
        container.is_iterable(), "`(push container arg)'",
        "expect `container' to an iterable."
    );
    auto arg = args[1];
    Object result{};
    if(container.is_string()){
        ELix::validate_type(
            arg.is_string(), "`(push string arg)'",
            "expect `arg' to be a String."
        );
        result = Object((container + arg));
    }else if(container.is_array()){
        container.as_array().push(arg);
        result = container;
    }else if(container.is_list()){
        container.as_list().push(arg);
        result = container;
    }else if(container.is_set()){
        container.as_set().add(arg);
        result = container;
    }else{
        ELix::validate_type(
            arg.is_pair(), "`(push dict arg)'", "expect `arg' to be a Pair object."
        );
        auto pair = arg.as_pair();
        container.as_dict().hmap[pair.key] = pair.val;
        result = container;
    }

    return result;
}

// -*-
static Object fn_pop(const Vec<Object>& args, ELix* elix){
    // (pop container)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "pop");
    auto container = args[0];
    ELix::validate_type(
        container.is_iterable(), "`(pop container)'",
        "expect `container' to an iterable."
    );
    Object result{};
    if(container.is_string()){
        container.as_string().text = container.as_string().text.substr(
            0, container.as_string().len()-1
        );
        result = Object((container));
    }else if(container.is_array()){
        container.as_array().pop();
        result = container;
    }else if(container.is_list()){
        container.as_list().pop();
        result = container;
    }else if(container.is_set()){
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<i64> dist(0, container.as_set().len());
        auto n = dist(rng);
        Object key{};
        decltype(n) i = 0;
        for(const auto& val: (container.as_set().hset)){
            if(i==n){ key = val; }
            ++i;
        }
        container.as_set().remove(key);
        result = container;
    }else{
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<i64> dist(0, container.as_set().len());
        auto n = dist(rng);
        Object key{};
        decltype(n) i = 0;
        for(const auto& val: (container.as_dict().hmap)){
            if(i==n){ key = val.first; }
            ++i;
        }
        container.as_dict().remove(key);
        result = container;
    }

    return result;
}

static Object fn_clear(const Vec<Object>& args, ELix* elix){
    // (clear container)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "clear");
    auto container = args[0];
    ELix::validate_type(
        container.is_iterable(), "`(clear container)'",
        "expect `container' to an iterable."
    );
    Object result{};
    if(container.is_string()){
        container.as_string().text.clear();
        result = Object((container));
    }else if(container.is_array()){
        container.as_array().clear();
        result = container;
    }else if(container.is_list()){
        container.as_list().clear();
        result = container;
    }else if(container.is_set()){
        container.as_set().clear();
        result = container;
    }else{
        container.as_dict().clear();
        result = container;
    }

    return result;
}

static Object fn_concat(const Vec<Object>& args, ELix* elix){
    // (concat arg1 arg2 ... argN)
    auto pred = (args.size()>=1);
    ELix::validate_argc(pred, "concat");
    pred = true;
    for(const auto& arg: args){
        pred = pred && (args[0].type()==arg.type());
    }
    ELix::validate_type(
        (pred && args[0].is_iterable()), "`(concat arg1 arg2 ... argN)'",
        "expect all arguments to have the same iterable type."
    );
    Object result{};
    if(args[0].is_string()){
        result = Object(String{});
        for(const auto& arg: args){
            result = result + arg.as_string();
        }
    }else if(args[0].is_array()){
        Array self{};
        for(const auto& arg: args){
            self.concat(arg.as_array());
        }
        result = Object(self);
    }else if(args[0].is_list()){
        List self{};
        for(const auto& arg: args){ self.concat(arg.as_list()); }
        result = Object(self);
    }else if(args[0].is_set()){
        Set self{};
        for(const auto& arg: args){ self.concat(arg.as_set()); }
        result = Object(self);
    }else{
        Dict self{};
        for(const auto& arg: args){ self.concat(arg.as_dict()); }
        result = Object(self);
    }

    return result;
}

static Object fn_reverse(const Vec<Object>& args, ELix* elix){
    // (reverse arg)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "reverse");
    auto arg = args[0];
    ELix::validate_type(
        (arg.is_string() || arg.is_array() || arg.is_list()),
        "`(reverse arg)'",
        "expect `arg' to be either a String, a List or an Array."
    );
    Object result{};
    if(args[0].is_string()){
        arg.as_string().reverse();
        result = Object(arg.as_string());
    }else if(args[0].is_array()){
        arg.as_array().reverse();
        result = Object(arg.as_array());
    }else{
        arg.as_list().reverse();
        result = Object(arg.as_list());
    }

    return result;
}

static Object fn_find(const Vec<Object>& args, ELix* elix){
    // (find haystack needle)
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "find");
    ELix::validate_type(
        args[0].is_iterable(), "`(find haystack needle)'",
        "expect `haystack' to be an iterable object."
    );
    auto haystack = args[0];
    auto needle = args[1];
    Object result{};
    if(haystack.is_string()){
        ELix::validate_type(
            needle.is_string(), "`(find string arg)'",
            "expect `arg' to a string."
        );
        auto ans = haystack.as_string().find(needle.as_string());
        result = Object(Number(ans));
    }else if(haystack.is_array()){
        auto ans = haystack.as_array().find(needle);
        result = Object(Number(ans));
    }else if(haystack.is_list()){
        auto ans = haystack.as_list().find(needle);
        result = Object(Number(ans));
    }else if(haystack.is_set()){
        auto ans = haystack.as_set().find(needle);
        result = Object(ans);
    }else{
        auto ans = haystack.as_dict().find(needle);
        result = Object(ans);
    }

    return result;
}

static Object fn_min(const Vec<Object>& args, ELix* elix){
    // (min list_or_array)
    // (min x1 x2 ... xN)
    auto pred = (args.size() >= 1);
    ELix::validate_argc(pred, "min");

    Object result{};
    if(args.size()==1){
        auto arg = args[0];
        pred = (arg.is_list() || arg.is_array());
        ELix::validate_type(
            pred, "`(min arg)'",
            "expect `arg' to a List or an Array."
        );
        if(arg.is_array()){
            ELix::validate_value(
                (arg.as_array().len()!=0), "(min xarray)",
                "cannot sort an empty array `xarray'."
            );
            std::sort(
                arg.as_array().items.begin(), arg.as_array().items.end(),
                [](const Object& lhs, const Object& rhs){
                    return (lhs < rhs);
                }
            );
            result = arg.as_array().items[0];
        }else{
            ELix::validate_value(
                (arg.as_list().len()!=0), "(min xlist)",
                "cannot sort an empty list `xlist'."
            );
            std::sort(
                arg.as_list().items.begin(), arg.as_list().items.end(),
                [](const Object& lhs, const Object& rhs){
                    return (lhs < rhs);
                }
            );
            result = arg.as_list().items[0];
        }
    }else{
        std::sort(
            args.begin(), args.end(),
            [](const Object& x, const Object& y){ return (x < y); }
        );
        result = args[0];
    }

    return result;
}

// -*-
static Object fn_max(const Vec<Object>& args, ELix* elix){
    // (max list_or_array)
    // (max x1 x2 ... xN)
    auto pred = (args.size() >= 1);
    ELix::validate_argc(pred, "max");

    Object result{};
    if(args.size()==1){
        auto arg = args[0];
        pred = (arg.is_list() || arg.is_array());
        ELix::validate_type(
            pred, "`(max arg)'",
            "expect `arg' to a List or an Array."
        );
        if(arg.is_array()){
            ELix::validate_value(
                (arg.as_array().len()!=0), "(min xarray)",
                "cannot sort an empty array `xarray'."
            );
            std::sort(
                arg.as_array().items.begin(), arg.as_array().items.end(),
                [](const Object& lhs, const Object& rhs){
                    return (lhs > rhs);
                }
            );
            result = arg.as_array().items[0];
        }else{
            ELix::validate_value(
                (arg.as_list().len()!=0), "(max xlist)",
                "cannot sort an empty list `xlist'."
            );
            std::sort(
                arg.as_list().items.begin(), arg.as_list().items.end(),
                [](const Object& lhs, const Object& rhs){
                    return (lhs > rhs);
                }
            );
            result = arg.as_list().items[0];
        }
    }else{
        std::sort(
            args.begin(), args.end(),
            [](const Object& x, const Object& y){ return (x > y); }
        );
        result = args[0];
    }

    return result;
}

// -*-
static Object fn_range(const Vec<Object>& args, ELix* elix){
    // (range n)
    // (range vmin vmax)
    // (range vmin vmax step)
    auto pred = (args.size()>=1 && args.size() < 4);
    Vec<Object> result{};
    ELix::validate_argc(pred, "range");
    if(args.size()==1){
        ELix::validate_type(
            args[0].is_integer(), "`(range n)'",
            "expect `n' to be an integer"
        );
        auto n = args[0].as_integer();
        ELix::validate_value((n > 0), "`(range n)'", "expect n > 0");
        Vec<i64> vec(n);
        std::iota(vec.begin(), vec.end(), 0);
        Vec<Object> self(n);
        for(const auto& val: vec){ self.push_back(Object(Number{val})); }
        result = self;
    }else if(args.size()==2){
        pred = (args[0].is_integer() && args[1].is_integer());
        ELix::validate_type(
            pred, "`(range vmin vmax)'",
            "expect `vmin' and `vmax' to be integers."
        );
        auto vmin = args[0].as_integer();
        auto vmax = args[1].as_integer();
        pred = (vmin < vmax);
        ELix::validate_value(
            pred, "`(range vmin vmax)'",
            "expect vmin < vmax."
        );
        for(auto val=vmin; val < vmax; val++){
            result.push_back(Object(Number(val)));
        }
    }else{
        pred = (args[0].is_integer() && args[1].is_integer(), args[2].is_integer());
        ELix::validate_type(
            pred, "`(range vmin vmax step)'",
            "expect `vmin', `vmax' and `step' to be integers."
        );
        auto vmin = args[0].as_integer();
        auto vmax = args[1].as_integer();
        auto step = args[2].as_integer();
        pred = ((vmin < vmax) && (step > 0));
        ELix::validate_value(
            pred, "`(range vmin vmax step)'",
            "expect vmin < vmax and step > 0"
        );
        for(auto val=vmin; val < vmax; val += step){
            result.push_back(Object(Number(val)));
        }
    }

    return Object(Array{result});
}

// -*-
static Object fn_linspace(const Vec<Object>& args, ELix* elix){
    // (linspace vmin vmax)
    // (linspace vmin vmax N)
    auto pred = (args.size()==2 || args.size()==3);
    ELix::validate_argc(pred, "linspace");
    usize N = 10;
    Vec<Object> result(N);
    if(args.size()==2){
        pred = (args[0].is_number() && args[1].is_number());
        ELix::validate_type(
            pred, "`(linspace vmin vmax)'",
            "expect `vmin' and `vmax' to be number."
        );
        auto vmin = args[0].as_float();
        auto vmax = args[1].as_float();
        pred = (vmin < vmax);
        ELix::validate_value(
            pred, "`(linspace vmin vmax)'",
            "expect vmin < vmax."
        );
        auto dx = (vmax - vmin) / (N - 1);
        for(auto val=vmin; val < vmax; val += dx){
            result.push_back(Object(Number(val)));
        }
    }else{
        pred = (args[0].is_integer() && args[1].is_integer(), args[2].is_integer());
        ELix::validate_type(
            pred, "`(linspace vmin vmax N)'",
            "expect `vmin', `vmax' must be floating-point numbers "
            "and `N' an integer."
        );
        auto vmin = args[0].as_float();
        auto vmax = args[1].as_float();
        auto N = args[2].as_integer();
        pred = ((vmin < vmax) && (N > 1));
        ELix::validate_value(
            pred, "`(linspace vmin vmax N)'",
            "expect vmin < vmax and step > 1"
        );
        result.resize(N);
        auto dx = (vmax - vmin) / (N - 1);
        for(auto val=vmin; val < vmax; val += dx){
            result.push_back(Object(Number(val)));
        }
    }

    return Object(Array{result});
}

static Object fn_repeat(const Vec<Object>& args, ELix* elix){
    // (repeat N obj)
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "repeat");
    pred = (args[0].is_integer());
    ELix::validate_type(
        pred, "`(repeat N obj)'", "expect `N' to be an integer."
    );
    auto N = args[0].as_integer();
    ELix::validate_value(
        (N > 0), "`(repeat N obj)'", "expect N > 0"
    );
    Vec<Object> result(N);
    for(auto i=0; i < N; i++){ result.push_back(args[1]); }

    return Object(Array{result});
}

// static Object fn_partial(const Vec<Object>& args, ELix* elix){
//     // (partial fn args)
//     auto pred = (args.size()==2);
//     ELix::validate_argc(pred, "partial");
//     ELix::validate_type(
//         args[0].is_function(), "`(partial fn args)'",
//         "expect `fn' to be a user-defined function, not a lambda, not a macro,\n"
//         "and not a builtin function."
//     );
//     pred = (
//         args[1].is_array() || args[1].is_list() ||
//         args[1].is_dict() || args[1].is_set()
//     );
//     ELix::validate_type(
//         pred, "`(partial fn args)'",
//         "expect `args' to be a List, an Array, a Dict or a Set."
//     );
//     auto func = args[0].as_function();
//     auto argv = args[1];
//     if(argv.is_array()){
//         pred = (func.params.size()>=argv.as_array().len());
//     }else if(argv.is_list()){
//         pred = (func.params.size()>=argv.as_list().len());
//     }else if(argv.is_set()){
//         pred = (func.params.size()>=argv.as_set().len());
//     }else{
//         pred = (func.params.size()>=argv.as_dict().len());
//     }

//     ELix::validate_type(
//         pred, "`(partial fn args)'",
//         "expect `args' to be a List, an Array, a Dict or a Set."
//     );

//     Vec<Pair> pairs{};
//     if(argv.is_array() || argv.is_list()){
//         auto items = (
//             argv.is_list() ? argv.as_list().items : argv.as_array().items
//         );
//         for(const auto& item: items){
//             ELix::validate_type(
//                 item.is_pair(), "`(partial fn args)'",
//                 "If args is a List or an Array, expect each element to be a Pair."
//             );
//             auto pair = item.as_pair();
//             ELix::validate_type(
//                 pair.key.is_symbol(), "`(partial fn args)'",
//                 "If args is a List or an Array, expect each element to be a Pair\n"
//                 "where the key component of Pair is a symbol."
//             );
//             pairs.push_back(pair);
//         }
//     }else if(argv.is_set()){
//         for(const auto& item: argv.as_set().hset){
//             ELix::validate_type(
//                 item.is_pair(), "`(partial fn args)'",
//                 "If args is a Set, expect each element to be a Pair."
//             );
//             auto pair = item.as_pair();
//             ELix::validate_type(
//                 pair.key.is_symbol(), "`(partial fn args)'",
//                 "If args is a Set, expect each element to be a Pair\n"
//                 "where the key component of Pair is a symbol."
//             );
//             pairs.push_back(pair);
//         }
//     }else{
//         for(const auto pair: argv.as_dict().items()){
//             ELix::validate_type(
//                 pair.key.is_symbol(), "`(partial fn args)'",
//                 "If args is a Dict, expect each the key of each entry to be a Symbol."
//             );
//             pairs.push_back(pair);
//         }
//     }

//     for(;;){}

// }

// -*- (reduce fn arg init)
static Object fn_reduce(const Vec<Object>& args, ELix* elix){
    // (reduce fn arg init)
    auto pred = (args.size()==3);
    ELix::validate_argc(pred, "reduce");
    pred = args[0].is_callable();
    ELix::validate_type(
        pred, "`(reduce fn arg init)'",
        "expect `fn' to be a callable."
    );
    pred = (
        args[1].is_list() || args[1].is_array() ||
        args[1].is_set() || args[1].is_dict()
    );
    ELix::validate_type(
        pred, "`(reduce fn arg init)'",
        "expect `arg' to be a List, an Array, a Set or a Dict."
    );

    Object result{};
    auto argv = args[1];
    auto init = args[2];
    if(args[0].is_lambda() || args[0].is_function()){
        auto func = (
            args[0].is_lambda() ? args[0].as_lambda() : args[0].as_function()
        );
        if(argv.is_list() || argv.is_array()){
            auto items = (argv.is_list() ? argv.as_list().items : argv.as_array().items);
            for(const auto& obj: items){
                init = func(Vec<Object>{init, obj}, elix);
            }
            result = init;
        }else if(argv.is_set()){
            for(const auto& obj: argv.as_set().hset){
                init = func(Vec<Object>{init, obj}, elix);
            }
            result = init;
        }else{
            for(const auto& pair: argv.as_dict().items()){
                init = func(Vec<Object>{init, Object(pair)}, elix);
            }
            result = init;
        }
    }else if(args[0].is_func()){
        auto func = args[0].as_func();
        if(argv.is_list() || argv.is_array()){
            auto items = (argv.is_list() ? argv.as_list().items : argv.as_array().items);
            for(const auto& obj: items){
                init = func(Vec<Object>{init, obj}, elix);
            }
            result = init;
        }else if(argv.is_set()){
            for(const auto& obj: argv.as_set().hset){
                init = func(Vec<Object>{init, obj}, elix);
            }
            result = init;
        }else{
            for(const auto& pair: argv.as_dict().items()){
                init = func(Vec<Object>{init, Object(pair)}, elix);
            }
            result = init;
        }
    }else{
        auto func = args[0].as_macro();
        if(argv.is_list() || argv.is_array()){
            auto items = (argv.is_list() ? argv.as_list().items : argv.as_array().items);
            for(const auto& obj: items){
                init = func(Vec<Object>{init, obj}, elix);
            }
            result = init;
        }else if(argv.is_set()){
            for(const auto& obj: argv.as_set().hset){
                init = func(Vec<Object>{init, obj}, elix);
            }
            result = init;
        }else{
            for(const auto& pair: argv.as_dict().items()){
                init = func(Vec<Object>{init, Object(pair)}, elix);
            }
            result = init;
        }
    }

    return result;
}

// -*-
void ELix::initialize_functional(void){
    add_builtin("map", fn_map, 2, 2);               // (map fn arg)
    add_builtin("filter", fn_filter, 2, 2);         // (filter fn arg)
    add_builtin("reduce", fn_reduce, 3, 3);         // (reduce fn arg init)
    add_builtin("zip", fn_zip, 2, -1);              // (zip arg1 arg2 ... argN)
    add_builtin("enumerate", fn_enumerate, 1, 1);   // (enumerate arg)
    add_builtin("len", fn_len, 1, 1);               // (len arg)
    add_builtin("push", fn_push, 2, 2);             // (push arg x)
    add_builtin("pop", fn_pop, 1, 1);               // (pop arg)
    add_builtin("clear", fn_clear, 1, 1);           // (clear arg)
    add_builtin("concat", fn_concat, 2, -1);        // (concat arg1 arg2 ... argN)
    add_builtin("reverse", fn_reverse, 1, 1);       // (reverse arg)
    add_builtin("find", fn_find, 2, 2);             // (find arg x)
    add_builtin("min", fn_min, 1, -1);              // (min arg [arg2 arg3 ... argN])
    add_builtin("max", fn_max, 1, -1);              // (max arg [arg2 arg3 ... argN])
    add_builtin("range", fn_range, 1, 3);           // (range N) | (range vmin vmax [step])
    add_builtin("linspace", fn_linspace, 2, 3);     // (linspace vmin vmax [N])
    add_builtin("repeat", fn_repeat, 2, 2);         // (repeat N obj)
    /*
static Object fn_partial(const Vec<Object>& args);
    */
}

// -*----------------------------------------*-
// -*- Basic builtin Input/Output functions -*-
// -*----------------------------------------*-
static Object fn_println(const Vec<Object>& args, ELix* elix){
    // (println ...)
    for(usize i=0; i < args.size(); i++){
        if(i > 0){ std::cout << " "; }
        std::cout << args[i].str();
    }
    std::cout << std::endl;
    return Object();
}

static Object fn_eprintln(const Vec<Object>& args, ELix* elix){
    // (eprintln ...)
    for(usize i=0; i < args.size(); i++){
        if(i > 0){ std::cout << " "; }
        std::cerr << args[i].str();
    }
    std::cerr << std::endl;

    return Object();
}

static Object fn_print(const Vec<Object>& args, ELix* elix){
    // (print ...)
    for(usize i=0; i < args.size(); i++){
        if(i > 0){ std::cout << " "; }
        std::cout << args[i].str();
    }
}

static Object fn_eprint(const Vec<Object>& args, ELix* elix){
    // (eprint ...)
    for(usize i=0; i < args.size(); i++){
        if(i > 0){ std::cout << " "; }
        std::cerr << args[i].str();
    }
}

static Object fn_input(const Vec<Object>& args, ELix* elix){
    // (input [prompt])
    auto pred = (args.size() <= 1);
    ELix::validate_argc(pred, "input");
    if(args.size()==1){
        ELix::validate_type(
            args[0].is_string(), "`(input [prompt])'",
            "If prompt provided, it must be a String."
        );
        std::cout << args[0].str();
    }
    std::string line{};
    std::getline(std::cin >> std::ws, line);

    return Object(String{line});
}

// -*-
void ELix::initialize_basic_io(void){
    add_builtin("println", fn_println, -1, -1);         // (println ...)
    add_builtin("eprintln", fn_eprintln, -1, -1);       // (eprintln ...)
    add_builtin("print", fn_print, -1, -1);             // (print ...)
    add_builtin("eprint", fn_eprint, -1, -1);           // (eprint ...)
    add_builtin("input", fn_input, 0, 1);               // (input [prompt])
}

// -*-----------------------------------*-
// -*- Miscellaneous builtin functions -*-
// -*-----------------------------------*-
static Object fn_clone(const Vec<Object>& args, ELix* elix){
    // (clone obj)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "clone");

    return args[0].clone();
}

// -*-
static Object fn_typeof(const Vec<Object>& args, ELix* elix){
    // (typeof obj)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "typeof");

    return Object(args[0].type());
}

// -*-
static Object fn_isinstance(const Vec<Object>& args, ELix* elix){
    // (insintance? obj ty)
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "isinstance?");
    pred = (args[1].is_symbol());
    ELix::validate_type(
        pred, "`(isinstance? obj ty)'",
        "expect `ty' to be the name of type we are comparing against."
    );
    auto ty = args[0].as_symbol();
    std::set<std::string> types = {
        "Nil", "Bool", "Integer", "Float",
        "Array", "List", "Set", "Dict", "Func",
        "Function", "Lambda", "Macro",
        "String", "Symbol", "Pair",
    };
    if(types.find(ty.str())==types.end()){
        std::stringstream ss;
        ss << "`(isinstanace? obj ty)'" << "unknow type name `ty' ";
        ss << std::quoted(ty.str());
        throw ELixError(ELixError::TypeError, ss.str());
    }

    auto ans = (ty==args[0].type());

    return Object(ans);
}

static Object fn_symbols(const Vec<Object>& args, ELix* elix){
    // (symbols)
    auto pred = (args.empty());
    ELix::validate_argc(pred, "symbols");

    Vec<Object> result{};
    for(const auto& [key, _]: elix->runtime()->bindings()){
        result.push_back(Object(Symbol{key}));
    }

    return Object(Array{result});
}

static Object fn_macro_expand(const Vec<Object>& args, ELix* elix){
    // (Macro.expand obj arg1 arg2 ... argN)
    auto pred = (args.size()>=2);
    ELix::validate_argc(pred, "Macro.expand");
    pred = (args[0].is_macro());
    ELix::validate_type(
        pred, "`(Macro.expand obj args)'", "expect `obj' to be a Macro."
    );
    auto argv = Vec<Object>(args.begin()+1, args.end());
    auto expr = args[0].as_macro().expand(argv, elix);
    auto result = expr->repr();

    return Object(String{result});
}

// -*-
static std::map<i64, std::string> ASCII = {
    {0, "NUL"}, {1, "SOH"}, {2, "STX"}, {3, "ETX"}, {4, "EOT"},
    {5, "ENQ"}, {6, "ACK"}, {7, "BEL"}, {8, "BS"}, {9, "HT"},
    {10, "LF/NL"}, {11, "VT"}, {12, "FF"}, {13, "CR"}, {14, "SO"},
    {15, "SI"}, {16, "DLE"}, {17, "DC1"}, {18, "DC2"}, {19, "DC3"},
    {20, "DC4"}, {21, "NAK"}, {22, "SYN"}, {23, "ETB"}, {24, "CAN"},
    {25, "EM"}, {26, "SUB"}, {27, "ESC"}, {28, "FS"}, {29, "GS"},
    {30, "RS"}, {31, "US"}, {32, "SPACE"}, {33, "!"}, {34, "\""},
    {35, "#"}, {36, "$"}, {37, "%"}, {38, "&"}, {39, "'"}, {40, "("},
    {41, ")"}, {42, "*"}, {43, "+"}, {44, ","}, {45, "-"}, {46, "."},
    {47, "/"}, {48, "0"}, {49, "1"}, {50, "2"}, {51, "3"}, {52, "4"},
    {53, "5"}, {54, "6"}, {55, "7"}, {56, "8"}, {57, "9"}, {58, ":"},
    {59, ";"}, {60, "<"}, {61, "="}, {62, ">"}, {63, "?"}, {64, "@"},
    {65, "A"}, {66, "B"}, {67, "C"}, {68, "D"}, {69, "E"}, {70, "F"},
    {71, "G"}, {72, "H"}, {73, "I"}, {74, "J"}, {75, "K"}, {76, "L"},
    {77, "M"}, {78, "N"}, {79, "O"}, {80, "P"}, {81, "Q"}, {82, "R"},
    {83, "S"}, {84, "T"}, {85, "U"}, {86, "V"}, {87, "W"}, {88, "X"},
    {89, "Y"}, {90, "Z"}, {91, "["}, {92, "\\"}, {93, "]"}, {94, "^"},
    {95, "_"}, {96, "`"}, {97, "a"}, {98, "b"}, {99, "c"}, {100, "d"},
    {101, "e"}, {102, "f"}, {103, "g"}, {104, "h"}, {105, "i"}, {106, "j"},
    {107, "k"}, {108, "l"}, {109, "m"}, {110, "n"}, {111, "o"}, {112, "p"},
    {113, "q"}, {114, "r"}, {115, "s"}, {116, "t"}, {117, "u"}, {118, "v"},
    {119, "w"}, {120, "x"}, {121, "y"}, {122, "z"}, {123, "{"}, {124, "|"},
    {125, "}"}, {126, "~"}, {127, "DEL"},
};
static Object fn_ascii_chr(const Vec<Object>& args, ELix* elix){
    // (Ascii.chr obj)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Ascii.chr");
    pred = (args[0].is_integer());
    ELix::validate_type(
        pred, "(chr obj)", "expect `obj' to be an integer."
    );
    auto n = args[0].as_integer();
    pred = (n >= 0 && n < 128);
    ELix::validate_value(
        pred, "`(chr obj)'", "expect `obj' in the range [0, 128)"
    );
    
    auto ans = ASCII[n];
    if(ans.length()!=1){ ans = "?????"; }

    return Object(String{ans});
}

static Object fn_ascii_isascii(const Vec<Object>& args, ELix* elix){
    // (isascii? obj)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "isascii?");
    auto obj = args[0];
    ELix::validate_type(
        obj.is_string(), "`(isascii? obj)'",
        "expect `obj' to be a string."
    );
    auto xstr = obj.as_string().text;
    pred = (xstr.length()==1);
    ELix::validate_value(
        pred, "`(isascii? obj)'", "expect `obj' is a String with len==1"
    );
    bool ans{false};
    for(const auto& [_, val]: ASCII){
        if(val==xstr){
            ans = true;
            break;
        }
    }

    return Object(ans);
}


// -*-
static Object fn_ascii_ord(const Vec<Object>& args, ELix* elix){
    // (ord obj)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "ord");
    auto obj = args[0];
    ELix::validate_type(
        obj.is_string(), "`(ord obj)'",
        "expect `obj' to be a string."
    );
    auto xstr = obj.as_string().text;
    pred = (xstr.length()==1);
    ELix::validate_value(
        pred, "`(ord obj)'", "expect `obj' is a String with len==1"
    );
    i64 ans{};
    for(const auto& [key, val]: ASCII){
        if(val==xstr){
            ans = key;
            break;
        }
    }

    return Object(ans);
}

// -*-
static Object fn_panic(const Vec<Object>& args, ELix* elix){
    // (panic ...)
    for(auto i=0; i < args.size(); i++){
        if(i > 0){ std::cerr << " "; }
        std::cerr << args[i].str();
    }
    std::cerr << std::endl;
    std::exit(1);
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
    ELix* m_elix;

    // -*-
    // (format "Some fancy text with {var} embedded")
    std::string format(const Object& obj, ELix* elix) const{
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

static Object fn_format(const Vec<Object>& args, ELix* elix){
    // (format xstr)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "format");
    pred = (args[0].is_string());
    ELix::validate_type(
        pred, "`(format obj)'", "expect `obj' must be a string."
    );
    auto src = args[0].as_string().text;
    Formatter formatter(src);
    auto ans = formatter.format();

    return Object(String{ans});
}

// -*-
static Object fn_help(const Vec<Object>& args, ELix* elix){
    // (help obj)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "help");
    pred = args[0].is_symbol();
    ELix::validate_type(
        pred, "`(help obj)'", "expect `obj' to be a symbol"
    );
    auto sym = args[0].as_symbol().str();
    auto entry = ELix::docstrings.find(sym);
    if(entry==ELix::docstrings.end()){
        std::stringstream ss;
        ss << "unknown symbol " << std::quoted(sym) << ".";
        throw ELixError(ELixError::RuntimeError, ss.str());
    }
    std::stringstream ss;
    ss << "ELix help for symbol " << std::quoted(sym) << "\n\n";
    for(auto i=0; i < entry->second.size(); i++){
        ss << "[" << (i+1) << "]\n";
        ss << entry->second[i] << "\n-------\n" << std::endl;
    }

    return Object(String{ss.str()});
}

// -*-
static Object fn_eval(const Vec<Object>& args, ELix* elix){
    // (eval obj)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "eval");
    pred = args[0].is_string();
    ELix::validate_type(
        pred, "`(eval obj)'", "expect `obj' to be a String object."
    );

    auto src = args[0].as_string().text;
    Tokenizer tokenizer(src);
    Parser parser(tokenizer);
    auto exprs = parser.parse();
    Object result{};
    for(auto&& expr: exprs){
        result = expr->eval(elix);
    }

    return result;
}

// -*-
static Object fn_getenv(const Vec<Object>& args, ELix* elix){
    // (getenv obj)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "getenv");
    pred = args[0].is_string();
    ELix::validate_type(
        pred, "`(getenv obj)'", "expect `obj' to be a String object."
    );
    auto var = args[0].as_string().text;
    auto ans = std::getenv(var.c_str());
    Vec<Object> result{};
    if(ans != nullptr){
        auto self = String{std::string(ans)};
        auto vec = self.split(String{":"});
        for(const auto& path: vec){
            result.push_back(Object(path));
        }
    }

    return Object(Array{result});
}

// -*-
static Object fn_setenv(const Vec<Object>& args, ELix* elix){
    // (setenv var val flag)
    auto pred = (args.size()==3);
    ELix::validate_argc(pred, "setenv");
    pred = (args[0].is_string() && args[1].is_string() && args[0].is_bool());
    ELix::validate_type(
        pred, "`(setenv var val flag)'",
        "expect `var' and `val' to be Strings and `flag' a Boolean."
    );

    auto var = args[0].as_string().text.c_str();
    auto val = args[1].as_string().text.c_str();
    auto flag = (args[2].as_bool() ? 1 : 0);
    auto rc = ::setenv(var, val, flag);
    auto ans = (rc==0 ? true : false);

    return Object(ans);
}

static Object fn_quit(const Vec<Object>& args, ELix* elix){
    // (quit)
    auto pred = args.empty();
    ELix::validate_argc(pred, "quit");
    std::exit(EXIT_SUCCESS);

    return Object();
}

static Object fn_exit(const Vec<Object>& args, ELix* elix){
    // (exit obj)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "exit");
    pred = args[0].as_integer();
    ELix::validate_type(
        pred, "`(exit obj)'", "expect `obj' to be an integer used as an exit code."
    );
    auto ec = args[0].as_integer();
    std::exit(ec);

    return Object();
}

// -*-
static Object fn_bye(const Vec<Object>& args, ELix* elix){
    // (bye obj)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "bye");
    pred = args[0].is_string();
    ELix::validate_type(
        pred, "`(bye obj)'", "expect `obj' to be a string used an exit message."
    );
    auto msg = args[0].as_string().text;
    std::cerr << msg << std::endl;
    std::exit(EXIT_SUCCESS);

    return Object();
}

// -*-
static Object fn_clamp(const Vec<Object>& args, ELix* elix){
    // (clamp val vmin vmax)
    auto pred = (args.size()==3);
    ELix::validate_argc(pred, "clamp");
    pred = (args[0].is_number() && args[1].is_number() && args[2].is_number());
    ELix::validate_type(
        pred, "`(clamp val vmin vmax)'",
        "expect `val', `vmin', and `vmax' to be numbers"
    );
    pred = (args[0].is_integer() && args[1].is_integer() && args[2].is_integer());
    Object result{};
    if(pred){
        auto val = args[0].as_integer();
        auto vmin = args[1].as_integer();
        auto vmax = args[2].as_integer();
        if(vmin > vmax){ std::swap(vmin, vmax); }
        auto ans = std::clamp(val, vmin, vmax);
        result = Object(Number(ans));
    }else{
        auto val = args[0].as_float();
        auto vmin = args[1].as_float();
        auto vmax = args[2].as_float();
        if(vmin > vmax){ std::swap(vmin, vmax); }
        auto ans = std::clamp(val, vmin, vmax);
        result = Object(Number(ans));
    }

    return result;
}

// -*-
static Object fn_gcd(const Vec<Object>& args, ELix* elix){
    // (gcd m n)
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "gcd");
    pred = (args[0].is_integer() && args[1].is_integer());
    ELix::validate_type(
        pred, "`(gcd m n)'", "expect `m', and `n' to integers"
    );
    auto m = args[0].as_integer();
    auto n = args[1].as_integer();
    auto ans = std::gcd(m, n);

    return Object(Number(ans));
}

static Object fn_lcm(const Vec<Object>& args, ELix* elix){
    // (lcm m n)
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "lcm");
    pred = (args[0].is_integer() && args[1].is_integer());
    ELix::validate_type(
        pred, "`(lcm m n)'", "expect `m', and `n' to integers"
    );
    auto m = args[0].as_integer();
    auto n = args[1].as_integer();
    auto ans = std::lcm(m, n);

    return Object(Number(ans));
}

// -*-
void ELix::initialize_misc(void){
    add_builtin("clone", fn_clone, 1, 1);               // (clone obj)
    add_builtin("typeof", fn_typeof, 1, 1);             // (typeof obj)
    add_builtin("isintance?", fn_isinstance, 2, 2);     // (isinstance? obj ty)
    add_builtin("symbols", fn_symbols, 0, 0);           // (symbols)
    add_builtin("Macro.expand", fn_macro_expand, 2, -1);// (Macro.expand obj args)
    add_builtin("chr", fn_ascii_chr, 1, 1);             // (chr obj)
    add_builtin("isascii?", fn_ascii_isascii, 1, 1);    // (isascii? obj)
    add_builtin("ord", fn_ascii_ord, 1, 1);             // (ord obj)
    add_builtin("panic", fn_panic, -1, -1);             // (panic ...)
    add_builtin("format", fn_format, 1, 1);             // (format obj)
    add_builtin("help", fn_help, 1, 1);                 // (help obj)
    add_builtin("eval", fn_eval, 1, 1);                 // (eval obj)
    add_builtin("getenv", fn_getenv, 1, 1);             // (getenv obj)
    add_builtin("setenv", fn_setenv, 3, 3);             // (setenv var val flag)
    add_builtin("quit", fn_quit, 0, 0);                 // (quit)
    add_builtin("exit", fn_exit, 1, 1);                 // (exit obj)
    add_builtin("bye", fn_bye, 1, 1);                   // (bye obj)
    add_builtin("clamp", fn_clamp, 3, 3);               // (clamp val vmin, vmax)
    add_builtin("gcd", fn_gcd, 2, 2);                   // (gcd m n)
    add_builtin("lcm", fn_lcm, 2, 2);                   // (lcm m n)
}

// -*---------------*-
// -*- String APIs -*-
// -*---------------*-
static Object fn_str_find(const Vec<Object>& args, ELix* elix){
    // (String.find haystack needle)
    // (String.find haystack needle from)
    auto pred = (args.size()==2 || args.size()==3);
    ELix::validate_argc(pred, "String.find");
    auto arg1 = args[0];
    auto arg2 = args[1];
    auto arg3 = (args.size()==3 ? args[2] : Object(Number(i64(0))));
    pred = (arg1.is_string() && arg2.is_string() && arg3.is_integer());
    ELix::validate_type(
        pred, "`(String.find haystack needle [from])'",
        "expect `haystack' and `needle' to be strings.\n"
        "If `from' is given, it must be an integer >= 0"
    );
    auto haystack = arg1.as_string();
    auto needle = arg2.as_string();
    auto from = arg3.as_integer();
    auto ans = haystack.find(needle, from);

    return Object(Number(ans));
}

// -*-
static Object fn_str_reverse(const Vec<Object>& args, ELix* elix){
    // (String.reverse obj)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "String.reverse");
    pred = args[0].is_string();
    ELix::validate_type(
        pred, "`(String.reverse arg)'", "expect `arg' to be a string."
    );
    auto result = args[0].as_string();
    result.reverse();

    return Object(result);
}

// -*-
static Object fn_str_concat(const Vec<Object>& args, ELix* elix){
    // (String.concat arg1 arg2)
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "String.concat");
    pred = (args[0].is_string() && args[1].is_string());
    ELix::validate_type(
        pred, "`(String.concat x y)'", "expect `x' and `y' to be strings."
    );
    auto self = args[0].as_string();
    self.concat(args[1].as_string());

    return Object(self);
}

// -*-
static Object fn_str_len(const Vec<Object>& args, ELix* elix){
    // (String.len obj)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "String.len");
    pred = (args[0].is_string());
    ELix::validate_type(
        pred, "`(String.len arg)'", "expect `arg' to be a string."
    );
    auto self = args[0].as_string();
    auto size = self.len();

    return Object(Number(size));
}

// -*-
static Object fn_str_eq(const Vec<Object>& args, ELix* elix){
    // (String.equal x y)
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "String.equal");
    pred = (args[0].is_string() && args[1].is_string());
    ELix::validate_type(
        pred, "`(String.equal arg1 arg2)'", "expect `arg1' and `arg2' to be strings."
    );
    auto x = args[0].as_string().text;
    auto y = args[1].as_string().text;

    auto ans = (x==y);
    return Object(ans);
}

// -*-
static Object fn_str_ne(const Vec<Object>& args, ELix* elix){
    // (String.not_equal obj)
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "String.not_equal");
    pred = (args[0].is_string() && args[1].is_string());
    ELix::validate_type(
        pred, "`(String.not_equal arg1 arg2)'", "expect `arg1' and `arg2' to be strings."
    );
    auto x = args[0].as_string().text;
    auto y = args[1].as_string().text;

    auto ans = (x!=y);
    return Object(ans);
}

static Object fn_str_split(const Vec<Object>& args, ELix* elix){
    // (String.split text [delim])
    auto pred = (args.size()==1 || args.size()==2);
    ELix::validate_argc(pred, "String.split");
    pred = (args[0].is_string() && args[1].is_string());
    ELix::validate_type(
        pred, "`(String.split text [delim])'",
        "expect `text' and the optional `from' to be strings."
    );
    auto self = args[0].as_string();
    auto delim = (args.size()==2 ? args[1].as_string() : String{" "});

    auto ans = self.split(delim);
    Vec<Object> result{};
    for(const auto& item: ans){
        result.push_back(Object(item));
    }
    return Object(Array{result});
}

// -*-
static Object fn_str_startswith(const Vec<Object>& args, ELix* elix){
    // (String.startswith text prefix)
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "String.startswith");
    pred = (args[0].is_string() && args[1].is_string());
    ELix::validate_type(
        pred, "`(String.startswith text prefix)'",
        "expect `text' and `prefix' to be strings."
    );
    auto self = args[0].as_string();
    auto prefix = args[1].as_string();

    auto ans = self.startswith(prefix);
    return Object(ans);
}

static Object fn_str_endswith(const Vec<Object>& args, ELix* elix){
    // (String.endswith text suffix)
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "String.endswith");
    pred = (args[0].is_string() && args[1].is_string());
    ELix::validate_type(
        pred, "`(String.endswith text suffix)'",
        "expect `text' and `suffix' to be strings."
    );
    auto self = args[0].as_string();
    auto suffix = args[1].as_string();

    auto ans = self.endswith(suffix);
    return Object(ans);
}

static Object fn_str_ltrim(const Vec<Object>& args, ELix* elix){
    // (String.ltrim text)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "String.ltrim");
    ELix::validate_type(
        args[0].is_string(), "`(String.ltrim text)'",
        "expect `text' to be a strings."
    );

    auto text = args[0].as_string();
    text.ltrim();

    return Object(text);
}

// -*-
static Object fn_str_rtrim(const Vec<Object>& args, ELix* elix){
    // (String.rtrim text)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "String.rtrim");
    ELix::validate_type(
        args[0].is_string(), "`(String.rtrim text)'",
        "expect `text' to be a strings."
    );

    auto text = args[0].as_string();
    text.rtrim();

    return Object(text);
}

static Object fn_str_trim(const Vec<Object>& args, ELix* elix){
    // (String.trim text)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "String.trim");
    ELix::validate_type(
        args[0].is_string(), "`(String.trim text)'",
        "expect `text' to be a strings."
    );

    auto text = args[0].as_string();
    text.trim();

    return Object(text);
}

// -*-
static Object fn_str_lower(const Vec<Object>& args, ELix* elix){
    // (String.lower text)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "String.lower");
    ELix::validate_type(
        args[0].is_string(), "`(String.lower text)'",
        "expect `text' to be a strings."
    );

    auto text = args[0].as_string();
    text.lower();

    return Object(text);
}

// -*-
static Object fn_str_upper(const Vec<Object>& args, ELix* elix){
    // (String.upper text)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "String.upper");
    ELix::validate_type(
        args[0].is_string(), "`(String.upper text)'",
        "expect `text' to be a strings."
    );

    auto text = args[0].as_string();
    text.upper();

    return Object(text);
}

// -*-
static Object fn_str_capitalize(const Vec<Object>& args, ELix* elix){
    // (String.capitalize text)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "String.capitalize");
    ELix::validate_type(
        args[0].is_string(), "`(String.capitalize text)'",
        "expect `text' to be a strings."
    );

    auto text = args[0].as_string();
    text.capitalize();

    return Object(text);
}

// -*-
static Object fn_str_title(const Vec<Object>& args, ELix* elix){
    // (String.title text)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "String.title");
    ELix::validate_type(
        args[0].is_string(), "`(String.title text)'",
        "expect `text' to be a string."
    );

    auto text = args[0].as_string();
    text.title();

    return Object(text);
}

// -*-
static Object fn_str_find_all(const Vec<Object>& args, ELix* elix){
    // (String.find_all text needle)
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "String.find_all");
    pred = (args[0].is_string() && args[1].is_string());
    ELix::validate_type(
        pred, "`(String.find_all text needle)'",
        "expect `text' and `needle' to be strings."
    );

    auto text = args[0].as_string();
    auto needle = args[1].as_string();
    auto ans = text.find_all(needle);
    Vec<Object> result{};
    for(const auto& item: ans){
        result.push_back(Object(Number(item)));
    }

    return Object(Array{result});
}

// -*-
static Object fn_str_replace(const Vec<Object>& args, ELix* elix){
    // (String.replace text old neo)
    auto pred = (args.size()==3);
    ELix::validate_argc(pred, "String.replace");
    pred = (args[0].is_string() && args[1].is_string() && args[2].is_string());
    ELix::validate_type(
        pred, "`(String.replace text old neo)'",
        "expect `text', `old' and `neo' to be strings."
    );

    auto text = args[0].as_string();
    auto old = args[1].as_string();
    auto neo = args[2].as_string();
    text.replace(old, neo);

    return Object(text);
}

// -*-
static Object fn_str_replace_all(const Vec<Object>& args, ELix* elix){
    // (String.replace_all text old neo)
    auto pred = (args.size()==3);
    ELix::validate_argc(pred, "String.replace_all");
    pred = (args[0].is_string() && args[1].is_string() && args[2].is_string());
    ELix::validate_type(
        pred, "`(String.replace_all text old neo)'",
        "expect `text', `old' and `neo' to be strings."
    );

    auto text = args[0].as_string();
    auto old = args[1].as_string();
    auto neo = args[2].as_string();
    text.replace_all(old, neo);

    return Object(text);
}

// -*-
static Object fn_str_substr(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_at(const Vec<Object>& args, ELix* elix){
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
static Object fn_symbol_eq(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_symbol_ne(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_symbol_name(const Vec<Object>& args, ELix* elix){
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
static Object fn_pair_key(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_pair_value(const Vec<Object>& args, ELix* elix){
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
static Object fn_array_find(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_reverse(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_concat(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_len(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_slice(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_insert(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_get(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_set(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_splice(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_any(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_all(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_sort(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_reduce(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_push(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_array_pop(const Vec<Object>& args, ELix* elix){
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
static Object fn_list_find(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_reverse(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_concat(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_len(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_head(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_tail(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_first(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_last(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_push(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_pop(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_insert(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_list_remove(const Vec<Object>& args, ELix* elix){
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
static Object fn_dict_find(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_concat(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_len(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_get(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_set(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_update(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_keys(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_values(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_popitem(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_clear(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_remove(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_dict_items(const Vec<Object>& args, ELix* elix){
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
static Object fn_hashset_find(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_concat(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_len(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_add(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_union(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_intersection(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_symmetric_difference(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_clear(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_remove(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_pop(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_isdisjoint(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_issubset(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_hashset_issuperset(const Vec<Object>& args, ELix* elix){
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