#include "elix.hpp"
#include<algorithm>
#include<sstream>
#include<iomanip>
#include<cctype>
#include<numeric>

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
    // (< x y ...)
    auto pred = (args.size() >= 2);
    ELix::validate_argc(pred, "<");
    auto acc = args[0];
    
    for(size_t i=1; i < args.size(); i++){
        acc = (acc < args[i] ? acc : args[i]);
    }
    return Object(acc);
}

static Object fn_le(const Vec<Object>& args, ELix* elix){
    // (<= x y ...)
    auto pred = (args.size() >= 2);
    ELix::validate_argc(pred, "<=");
    auto acc = args[0];
    for(size_t i=1; i < args.size(); i++){
        acc = (acc <= args[i] ? acc : args[i]);
    }
    return Object(acc);
}

static Object fn_gt(const Vec<Object>& args, ELix* elix){
    // (> x ...)
    auto pred = (args.size() >= 2);
    ELix::validate_argc(pred, ">");
    auto acc = args[0];
    for(size_t i=1; i < args.size(); i++){
        acc = (acc > args[i] ? acc : args[i]);
    }
    return Object(acc);
}

static Object fn_ge(const Vec<Object>& args, ELix* elix){
    // (>= x ...)
    auto pred = (args.size() >= 2);
    ELix::validate_argc(pred, ">=");
    auto acc = args[0];
    for(size_t i=1; i < args.size(); i++){
        acc = (acc >= args[i] ? acc : args[i]);
    }
    return Object(acc);
}

static Object fn_eq(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_ne(const Vec<Object>& args, ELix* elix){
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
static Object fn_map(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_filter(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_zip(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_enumerate(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_len(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_push(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_pop(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_clear(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_concat(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_reverse(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_find(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_min(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_max(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_range(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_linspace(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_repeat(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_partial(const Vec<Object>& args, ELix* elix){
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
static Object fn_println(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_eprintln(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_print(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_eprint(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_input(const Vec<Object>& args, ELix* elix){
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
static Object fn_clone(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_typeof(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_isinstance(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_symbols(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_macro_expand(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_ascii_chr(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_ascii_ord(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_panic(const Vec<Object>& args, ELix* elix){
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
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_help(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_eval(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_getenv(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_setenv(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_quit(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_exit(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_bye(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_clamp(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_gcd(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_lcm(const Vec<Object>& args, ELix* elix){
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
static Object fn_clamp(const Vec<Object>& args){}
static Object fn_gcd(const Vec<Object>& args){}
static Object fn_lcm(const Vec<Object>& args){}
    */
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*---------------*-
// -*- String APIs -*-
// -*---------------*-
static Object fn_str_find(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_reverse(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_concat(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_len(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_eq(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_ne(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_split(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_startswith(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_endswith(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_ltrim(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_rtrim(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_trim(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_lower(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_upper(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_capitalize(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_title(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_find_all(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_replace(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_str_replace_all(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

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