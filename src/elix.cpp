#include "elix.hpp"
#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<type_traits>
#include<typeindex>
#include<typeinfo>

// -*--------------------------------------------------------------------------*-
// -*- begin::namespace::ekasoft::elx                                         -*-
// -*--------------------------------------------------------------------------*-
namespace ekasoft::elx{
// -

// -*-----------------------*-
// -*- Static data members -*-
// -*-----------------------*-
std::string ELix::scriptExt;
std::map<std::string, Vec<std::string>> ELix::docstrings;

Symbol ELix::Quote;
Symbol ELix::Unquote;
Symbol ELix::Quasiquote;
Symbol ELix::UnquoteSplicing;
    
Context ELix::prelude;
ELix::ModuleSet ELix::BuiltinModules;

// -*------------------*-
// -*- Static methods -*-
// -*------------------*-
void ELix::setup(void){
    ELix::scriptExt = ".elx";
    ELix::docstrings = {};
    ELix::Quote = Symbol{"quote"};                      // '
    ELix::Unquote = Symbol{"unquote"};                  // ,
    ELix::Quasiquote = Symbol{"quasiquote"};            // `
    ELix::UnquoteSplicing = Symbol{"unquote-splicing"}; // ,@
    ELix::prelude = nullptr;
    ELix::BuiltinModules = {};

    ELix::setup_prelude();
    ELix::init_builtin_modules();
}

// static void setup_prelude(void);
// -*-
void ELix::setup_prelude(void){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
    /*
    ELix::initialize_constructors();
    ELix::initialize_predicates();
    ELix::initialize_operators();
    ELix::initialize_functional();
    ELix::initialize_basic_io();
    ELix::initialize_misc();
    ELix::initialize_string();
    ELix::initialize_symbol();
    ELix::initialize_pair();
    ELix::initialize_array();
    ELix::initialize_list();
    ELix::initialize_dict();
    ELix::initialize_set();
    ELix::initialize_math();
    */
}

// -*-
bool ELix::is_reserved_word(const std::string& word){
    std::set<std::string> keywords = {
#define ELIX_DEF(_, text) text,
        ELIX_RESERVED_WORDS()
#undef ELIX_DEF
    };

    auto key = keywords.find(word);
    if(key==keywords.end()){
        return false;
    }
    return true;
}

// -*-
bool ELix::is_literal(const Object& obj){
    auto check = (
        obj.is_symbol() || obj.is_number() || obj.is_bool() ||
        obj.is_string() || obj.is_nil()
    );
    return (check ? true : false);
}

// -*-
void ELix::repl(const Vec<Object>& args){
    //! @todo add optional banner message
    //! @todo process all the args
    [[maybe_unused]] auto _ = args;
    ELix::setup();
    ELix elix;
    elix.m_runtime = std::make_shared<Env>(ELix::prelude);
    std::string src{};
    while(true){
        try{
            src = ELix::input();
            Tokenizer tokenizer{src};
            Parser parser{tokenizer};
            auto exprs = parser.parse();
            Object result{};
            for(auto&& expr: exprs){
                result = elix.eval(std::move(expr));
            }
            std::cout << result.str() << std::endl;
        }catch(const ELixError& err){
            std::cerr << err.describe() << std::endl;
        }catch(std::exception& err){
            std::cerr << err.what() << std::endl;
        }catch(...){
            std::cerr << "Fatal: unknown error occurred." << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
}

// -*-
void ELix::run(const fs::path& scriptpath, const Vec<Object>& args){
    ELix::setup();
    ELix elix;
    elix.m_runtime = std::make_shared<Env>(ELix::prelude);
    elix.m_runtime->define("@ARGV", Object(Array{args}));
    std::string src{};
    try{
        src = ELix::readfile(scriptpath);
        Tokenizer tokenizer{src};
        Parser parser{tokenizer};
        auto exprs = parser.parse();
        for(auto&& expr: exprs){
            [[maybe_unused]] auto result = elix.eval(std::move(expr));
        }
    }catch(const ELixError& err){
        std::cerr << err.describe() << std::endl;
    }catch(std::exception& err){
        std::cerr << err.what() << std::endl;
    }catch(...){
        std::cerr << "Fatal: unknown error occurred." << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

// -*-
std::string ELix::readfile(const fs::path& filename){
    if(!fs::exists(filename)){
        std::stringstream ss;
        ss << "file " << std::quoted(filename.string()) << " not found.";
        throw ELixError(ELixError::RuntimeError, ss.str());
    }
    std::ifstream fin(filename, std::ios::binary);
    if(!fin.is_open()){
        std::stringstream ss;
        ss << "unable to open file " << std::quoted(filename.string());
        throw ELixError(ELixError::RuntimeError, ss.str());
    }
    auto fsize = fs::file_size(filename);
    std::string result(fsize, '\0');
    fin.read(&result[0], fsize);
    if(fin.is_open()){
        fin.close();
    }
    return result;
}

// -*-
std::string ELix::readfile(const std::string& filename){
    return ELix::readfile(fs::path(filename));
}

// -*-
std::string ELix::readfile(const char* filename){
    return ELix::readfile(std::string(filename));
}

// -*-
std::string ELix::input(void){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

void ELix::add_builtin_module(const Module& mymodule){
    ELix::BuiltinModules.insert(mymodule);
}

// -*-
void ELix::init_builtin_modules(void){
    //! @todo
    /*
    - json
    - yaml
    - toml
    - csv
    - regex
    - system
    - datetime
    - process
    - thread
    - file
    - path
    - http
    - containers
    - net
    - curses
    - wx
    - hbird ***
    - zmq
    - 
    */
}

// -------------------------------------------
// -*- ELix : the interpreter internal API -*-
// -------------------------------------------
void ELix::load(const Symbol& name){
    // - load a builtin module
    // already loaded (i.e imported)
    for(const auto& mymod: this->m_imported){
        if(mymod.name()==name.str()){
            return;
        }
    }
    // not yet imported
    // find-by-name
    for(auto& mymod: ELix::BuiltinModules){
        if(mymod.name()==name.str()){
            this->m_imported.insert(mymod); // add to cache
            mymod.load(this->m_runtime);
            return;
        }
    }

    // module not found
    std::stringstream ss;
    ss << "module " << std::quoted(name.str()) << " not found.";
    throw ELixError(ELixError::RuntimeError, ss.str());
}

// -*-
void ELix::load(const fs::path& path){
    // user-defined script
    for(const auto& mymod: this->m_imported){
        // path is absolute or relative
        auto filename = path.string();
        if(path==mymod.fullpath() || filename==mymod.filename()){
            //! alread loaded. Do nothing
            return;
        }
    }
    // not yet loaded
    auto src = ELix::readfile(path);
    Tokenizer tokenizer{src};
    Parser parser{tokenizer};
    auto exprs = parser.parse();
    for(auto&& expr: exprs){
        [[maybe_unused]] auto result = this->eval(std::move(expr));
    }
    // add to cache
    this->m_imported.insert(Module{path});
}

// -*-
void ELix::load(const std::string& path){
    // user-defined script
    for(const auto& mymod: this->m_imported){
        // path is absolute or relative
        if(path==mymod.filename()){
            //! alread loaded. Do nothing
            return;
        }
    }
    // not yet loaded
    auto src = ELix::readfile(path);
    Tokenizer tokenizer{src};
    Parser parser{tokenizer};
    auto exprs = parser.parse();
    for(auto&& expr: exprs){
        [[maybe_unused]] auto result = this->eval(std::move(expr));
    }
    // add to cache
    this->m_imported.insert(Module{path});
}

// -*-
Object ELix::eval(Expression expr){
    std::map<std::type_index, std::string> typesmap;
    typesmap[std::type_index(typeid(ListExpr))] = "ListExpr";
    typesmap[std::type_index(typeid(ArrayExpr))] = "ArrayExpr";
    typesmap[std::type_index(typeid(DictExpr))] = "DictExpr";
    typesmap[std::type_index(typeid(SetExpr))] = "SetExpr";
    typesmap[std::type_index(typeid(LiteralExpr))] = "LiteralExpr";
    typesmap[std::type_index(typeid(SymbolExpr))] = "SymbolExpr";
    typesmap[std::type_index(typeid(PairExpr))] = "PairExpr";

    if(typesmap[std::type_index(typeid(expr))]=="SymbolExpr"){
        auto self = dynamic_cast<SymbolExpr*>(expr.get());
        return this->eval(*self);
    }else if(typesmap[std::type_index(typeid(expr))]=="LiteralExpr"){
        auto self = dynamic_cast<LiteralExpr*>(expr.get());
        return this->eval(*self);
    }else if(typesmap[std::type_index(typeid(expr))]=="ArrayExpr"){
        auto self = dynamic_cast<ArrayExpr*>(expr.get());
        return this->eval(*self);
    }else if(typesmap[std::type_index(typeid(expr))]=="PairExpr"){
        auto self = dynamic_cast<PairExpr*>(expr.get());
        return this->eval(*self);
    }else if(typesmap[std::type_index(typeid(expr))]=="SetExpr"){
        auto self = dynamic_cast<SetExpr*>(expr.get());
        return this->eval(*self);
    }else if(typesmap[std::type_index(typeid(expr))]=="DictExpr"){
        auto self = dynamic_cast<DictExpr*>(expr.get());
        return this->eval(*self);
    }else if(typesmap[std::type_index(typeid(expr))]=="ListExpr"){
        auto self = dynamic_cast<ListExpr*>(expr.get());
        return this->eval(*self);   
    }

    std::stringstream ss;
    ss << "Malformed expression " << expr->repr();
    throw ELixError(ELixError::SyntaxError, ss.str());
}

// -*-
Object ELix::eval(LiteralExpr& expr){
    return expr.obj;
}

// -*-
Object ELix::eval(SymbolExpr& expr){
    if(expr.name.str()=="nil"){ return Object(); }
    else if(expr.name.str()=="true"){ return Object(true); }
    else if(expr.name.str()=="false"){ return Object(false); }

    // user-defined variable or builtin constants
    return this->m_runtime->get(expr.name.str());
}

// -*-
Object ELix::eval(ListExpr& expr){
    // special-form or function call
    // expect the first element in the list to be a symbol
        
    auto head = expr.items[0]->eval(this);
    if(!head.is_symbol()){
        std::stringstream ss;
        ss << "Incorrect list-expression. Expect the first element to be a symbol.\n";
        ss << "A list-expression is a special-form or a function all.\n";
        ss << "Example:\n";
        ss << "   (lambda () (println \"Hello World!\"))\n";
        ss << "   (myfunction args)\n";
        throw ELixError(ELixError::SyntaxError, ss.str());
    }
    Vec<Expression> args{};
    for(auto i=1; i < expr.items.size(); i++){
        args.push_back(std::move(expr.items[i]));
    }
    auto word = head.as_symbol().str();
    Object fun;
    Object result{};
    if(!ELix::is_reserved_word(word)){
        // This is a function call
        // Get the associated callable object
        fun = this->m_runtime->get(word);
        // Check that `fun' is actually a callable
        if(!fun.is_callable()){
            std::stringstream ss;
            ss << "" << std::quoted(word) << " is not a callable object.";
            throw ELixError(ELixError::SyntaxError, ss.str());
        }
        Vec<Object> argv{};
        for(auto&& arg: args){
            argv.push_back(arg->eval(this));
        }
        if(fun.is_macro()){
            auto macro = fun.as_macro();
            result = macro(argv);
        }else if(fun.is_lambda() || fun.is_function()){
            auto lambda = fun.as_lambda();
            result = lambda(argv);
        }else if(fun.is_func()){
            auto func = fun.as_func();
            result = func(argv);
        }
    }else{
        if(word=="import"){ result = this->handle_import(args); }
        else if(word=="progn"){ result = this->handle_progn(args); }
        else if(word=="if"){ result = this->handle_if(args); }
        else if(word=="cond"){ result = this->handle_cond(args); }
        else if(word=="var"){ result = this->handle_var(args); }
        else if(word=="let"){ result = this->handle_let(args); }
        else if(word=="while"){ result = this->handle_while(args); }
        else if(word=="for"){ result = this->handle_for(args); }
        else if(word=="lambda"){ result = this->handle_lambda(args); }
        else if(word=="fun"){ result = this->handle_fun(args); }
        else if(word=="macro"){ result = this->handle_macro(args); }
        else if(word=="and"){ result = this->handle_and(args); }
        else if(word=="or"){ result = this->handle_or(args); }
        else if(word=="not"){ result = this->handle_not(args); }
        else if(word=="quote"){ result = this->handle_quote(args)->eval(this); }
        else if(word=="quasiquote"){ result = this->handle_quasiquote(args)->eval(this); }
        else if(word=="unquote"){ result = this->handle_unquote(args)->eval(this); }
        else if(word=="unquote-splicing"){ result = this->handle_unquote_splicing(args)->eval(this); }
        else{
            std::stringstream ss;
            ss << "unknown special form " << std::quoted(word);
            throw ELixError(ELixError::SyntaxError, ss.str());
        }
    }

    return result;
}

// -*-
Object ELix::eval(ArrayExpr& expr){
    Vec<Object> result{};
    for(auto&& item: expr.items){
        result.push_back(expr.eval(this));
    }

    return Object(Array{result});
}

// -*-
Object ELix::eval(DictExpr& expr){
    HashMap hmap{};
    for(auto&& item: expr.items){
        auto entry = item->eval(this);
        if(!entry.is_pair()){
            std::stringstream ss;
            ss << "Invalid Dict entry. Excpect each entry to a Pair object";
            throw ELixError(ELixError::SyntaxError, ss.str());
        }
        auto pair = entry.as_pair();
        hmap[pair.key] = pair.val;
    }
    return Object(Dict{hmap});
}

// -*-
Object ELix::eval(SetExpr& expr){
    HashSet hset{};
    for(auto&& key: expr.items){
        hset.insert(Object(key->eval(this)));
    }
    
    return Object(Set{hset});
}

// -*-
Object ELix::eval(PairExpr& expr){
    auto key = expr.key->eval(this);
    auto val = expr.val->eval(this);
    return Object(Pair{key, val});
}

// -*-
void ELix::print(const Object& obj){
    std::cout << obj.str();
}

// -*-
void ELix::println(const Object& obj){
    std::cout << obj.str() << std::endl;
}

// -*-
void ELix::check_argc(bool pred, const std::string& prefix){
    if(!pred){
        std::stringstream ss;
        ss << "Illegal " << std::quoted(prefix) << " expression: invalid number of arguments.";
        throw ELixError(ELixError::SyntaxError, ss.str());
    }
}

// -*-
Object ELix::handle_import(Vec<Expression> exprs){
    this->check_argc(exprs.size()==1, "import");
    auto self = exprs[0]->eval(this);
    if(!(self.is_symbol() || self.is_string())){
        std::stringstream ss;
        ss << "import: invalid argument type. Expect String or Symbol type.";
        throw ELixError(ELixError::TypeError, ss.str());
    }
    if(self.is_symbol()){
        this->load(self.as_symbol());
    }else{
        this->load(self.as_string().str());
    }

    return Object();
}

// -*-
Object ELix::handle_progn(Vec<Expression> exprs){
    for(auto&& expr: exprs){
        [[maybe_unused]] auto _ = expr->eval(this);
    }

    return Object();
}

// -*-
Object ELix::handle_if(Vec<Expression> exprs){
    auto pred = (exprs.size()==2 || exprs.size()==3);
    this->check_argc(pred, "if");

    if(exprs[0]->eval(this).as_bool()){
        return exprs[1]->eval(this);
    }

    if(exprs.size()==3){
        return exprs[2]->eval(this);
    }

    return Object();
}

// -*-
Object ELix::handle_let(Vec<Expression> exprs){
    auto pred = (exprs.size() > 1);
    this->check_argc(pred, "let");
    auto pairs_ = exprs[0]->eval(this);
    if(!pairs_.is_list()){
        std::stringstream ss;
        ss << "Malformed `let' expression. The first arguments must be a list of ";
        ss << "\npairs. Got " << std::quoted(pairs_.type().str()) << " object type.";
        throw ELixError(ELixError::SyntaxError, ss.str());
    }
    auto pairs = pairs_.as_list();
    // check whether each element is actually a pair
    for(const auto& item: pairs.items){
        if(!item.is_pair()){
            std::stringstream ss;
            ss << "Malformed `let' expression. The first arguments must be a list of ";
            ss << "\npairs. Got " << std::quoted(item.type().str()) << " object type as elements\n";
            ss << "of the list or pair.";
            throw ELixError(ELixError::SyntaxError, ss.str());
        }
        if(!item.as_pair().key.is_symbol()){
            std::stringstream ss;
            ss << "Malformed `let' expression. The first element of each pair must be ";
            ss << "a symbol.\nGot " << std::quoted(item.type().str()) << " object type.";
            throw ELixError(ELixError::SyntaxError, ss.str());
        }
    }

    auto ctx = std::make_shared<Env>(this->m_runtime);
    for(const auto& item: pairs.items){
        auto pair = item.as_pair();
        ctx->define(pair.key.as_symbol().str(), pair.val);
    }

    auto body = Vec<Expression>{};
    for(auto i=1; i < exprs.size(); i++){
        body.push_back(std::move(exprs[i]));
    }

    auto saved = this->m_runtime;
    this->m_runtime = ctx;
    for(auto& expr: body){
        [[maybe_unused]] auto _ = expr->eval(this);
    }

    this->m_runtime = saved;

    return Object();
}

// -*-
Object ELix::handle_var(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::handle_cond(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::handle_while(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::handle_for(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::handle_fun(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::handle_macro(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::handle_lambda(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::handle_and(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::handle_or(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::handle_not(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::handle_list(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Expression ELix::handle_quote(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Expression ELix::handle_quasiquote(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Expression ELix::handle_unquote(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Expression ELix::handle_unquote_splicing(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-------------------------------*-
// -*- ModuleLoader implementation -*-
// -*-------------------------------*-
Module::Module(const std::string& name){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

Module::Module(const fs::path& filepath){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

Module::Module(const std::string& name, const std::string& filepath){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

Module::Module(const std::string& name, const fs::path& filepath){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

Module::Module(const Module& mod) noexcept{
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

Module::Module(Module&& mod) noexcept{
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

Module& Module::operator=(const Module& mod) noexcept{
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

Module& Module::operator=(Module&& mod) noexcept{
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

void Module::load(Context& ctx) const{
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

void Module::add(const std::string& name, const Object& val){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

const Object& Module::get(const std::string& name) const{
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
void Module::setup(void){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

size_t ModuleHash::operator()(const Module& arg) const{
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

bool ModuleEqual::operator()(const Module& lhs, const Module& rhs) const{
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*--------------------------------------------------------------------------*-
}//-*- end::namespace::ekasoft::elx                                           -*-
// -*--------------------------------------------------------------------------*-