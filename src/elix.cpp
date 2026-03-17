#include "elix.hpp"
#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<type_traits>
#include<typeindex>
#include<typeinfo>
#include<limits>

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
    this->m_imported.insert(Module{path, this});
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
    this->m_imported.insert(Module{path, this});
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
void ELix::validate_expr(Expression& expr, const Vec<std::string>& exclude, const std::string& where){
    auto xs = dynamic_cast<ListExpr*>(expr.get());
    if(xs == nullptr){ return; }
    if(xs->items.size()==0){ return; }
    auto sym = dynamic_cast<SymbolExpr*>(xs->items[0].get());
    if(sym==nullptr){
        std::stringstream ss;
        ss << "Malformed list-expression. First element must be a symbol.";
        throw ELixError(ELixError::SyntaxError, ss.str());
    }
    for(const auto& name: exclude){
        if(sym->name.str()==name){
            std::stringstream ss;
            ss << name << " is not allowed outside " << where << "-expression.";
            throw ELixError(ELixError::SyntaxError, ss.str());
        }
    }
}

// -*-
Object ELix::eval(ListExpr& expr){
    // special-form or function call
    // expect the first element in the list to be a symbol
        
    auto head = dynamic_cast<SymbolExpr*>(expr.items[0].get());
    if(head==nullptr){
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
    auto word = head->name.str();
    Object fun;
    Object result{};
    if(!ELix::is_reserved_word(word)){
        result = this->handle_list(expr.items);
    }else{
        if((word!="while" && word=="for")){
            for(auto& x: args){
                this->validate_expr(x, Vec<std::string>{"break", "continue"}, "while");
                this->validate_expr(x, Vec<std::string>{"break", "continue"}, "for");
            }
        }
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
        else if(word=="continue"){ result = this->handle_continue(args); }
        else if(word=="break"){ result = this->handle_break(args); }
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
void ELix::validate_argc(bool pred, const std::string& prefix){
    if(!pred){
        std::stringstream ss;
        ss << "Illegal " << std::quoted(prefix) << " expression: invalid number of arguments.";
        throw ELixError(ELixError::SyntaxError, ss.str());
    }
}

// -*-
void ELix::validate_type(bool pred, const std::string& prefix, const std::string& emsg){
    if(!pred){
        std::stringstream ss;
        ss << std::quoted(prefix) << " " << emsg;
        throw ELixError(ELixError::ValueError, ss.str());
    }
}

// -*-
void ELix::validate_value(bool pred, const std::string& prefix, const std::string& emsg){
    if(!pred){
        std::stringstream ss;
        ss << std::quoted(prefix) << " " << emsg;
        throw ELixError(ELixError::ValueError, ss.str());
    }
}

// -*-
void ELix::validate_name(const std::string& name){
    if(ELix::is_reserved_word(name)){
        std::stringstream ss;
        ss << std::quoted(name) << " is a builtin reserved word. It cannot be used to ";
        ss << "name a variable.";
        throw ELixError(ELixError::ValueError, ss.str());
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
    /*
        (if test body)
    */
    auto pred = (exprs.size()==2 || exprs.size()==3);
    this->check_argc(pred, "if");

    auto test = exprs[0]->eval(this);
    if(!test.is_bool()){
        std::stringstream ss;
        ss << "Malformed `if' expression. Expect the test expression to\n";
        ss << "evaluate to a bool value.";
        throw ELixError(ELixError::SyntaxError, ss.str());
    }

    if(test.as_bool()){
        [[maybe_unused]] auto _ = exprs[1]->eval(this);
    }

    if(exprs.size()==3){
        [[maybe_unused]] auto _ = exprs[2]->eval(this);
    }

    return Object();
}

// -*-
Object ELix::handle_let(Vec<Expression> exprs){
    /*
        (let ((var-1 val-1)
            (var-2 val-2)
            ...
            (var-N val-N))
            body)
    */
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
    /*
        (var (key-1 val-1)
            (key-2 val-2)
            (key-3 val-3)
            ...
            (key-N val-N))
    */
    auto pred = (exprs.size() >= 2 && exprs.size() % 2 == 0);
    this->check_argc(pred, "var");
    for(auto i=0; i < exprs.size(); i += 2){
        auto key = dynamic_cast<SymbolExpr*>(exprs[i].get());
        if(key==nullptr){
            std::stringstream ss;
            ss << "Invalid `var` exprssion. Expect a variable name but got ";
            ss << std::quoted(exprs[i]->eval(this).type().str()) << " object type.";
            throw ELixError(ELixError::SyntaxError, ss.str());
        }
        auto var = key->name.str();
        if(ELix::is_reserved_word(var)){
            std::stringstream ss;
            ss << std::quoted(var) << " is a builtin reserved word. It cannot be used to ";
            ss << "name a variable.";
            throw ELixError(ELixError::ValueError, ss.str());
        }
        auto val = exprs[i+1]->eval(this);
        this->m_runtime->define(var, val);
    }

    return Object();
}

// -*-
Object ELix::handle_cond(Vec<Expression> exprs){
    /*
        (cond
            (test-1 expr-1)
            (test-2 expr-2)
            ...
            (test-N expr-N))
    */
    auto pred = (exprs.size() >= 2);
    this->check_argc(pred, "cond");
    bool failed{true};
    for(auto i=0; i < exprs.size(); i++){
        auto branch = dynamic_cast<ListExpr*>(exprs[i].get());
        if(branch==nullptr){
            std::stringstream ss;
            ss << "Invalid `cond` exprssion. Expect the argument to be a list of list-expression.";
            throw ELixError(ELixError::SyntaxError, ss.str());
        }
        if(branch->items.size()!=2){
            std::stringstream ss;
            ss << "Invalid `cond` branch exprssion.\n";
            ss << "Expect each branch to be a list of two expressions.\n";
            throw ELixError(ELixError::SyntaxError, ss.str());
        }
        auto test = branch->items[0]->eval(this);
        if(!test.is_bool()){
            std::stringstream ss;
            ss << "Invalid `cond` branch exprssion.\n";
            ss << "Expect the first element each branch evaluate to boolean.\n";
            ss << "Got " << test.type().str() << " object type.";
            throw ELixError(ELixError::SyntaxError, ss.str());
        }
        if(test.as_bool()){
            failed = false;
            [[maybe_unused]] auto _ = branch->items[1]->eval(this);
        }
    }

    if(failed){
        std::stringstream ss;
        ss << "Malformed `cond' expression. Expression the test condition of\n";
        ss << "at least one branch to evalue to true, but got none.";
        throw ELixError(ELixError::SyntaxError, ss.str());
    }

    return Object();
}

// -*-
Object ELix::handle_while(Vec<Expression> exprs){
    // (while test body)
    auto pred = (exprs.size() >= 1);
    this->check_argc(pred, "while");
    constexpr auto MY_MAX_ITERATIONS = std::numeric_limits<i32>::max();
    auto test = exprs[0]->eval(this);
    if(!test.is_bool()){
        std::stringstream ss;
        ss << "Malformed `while' expression. Expression the test condition of\n";
        ss << "at least one branch to evalue to true, but got none.";
        throw ELixError(ELixError::SyntaxError, ss.str());
    }

    u32 iteration = 0;
    while(test.as_bool()){
        if(iteration > MY_MAX_ITERATIONS){
            std::stringstream ss;
            ss << "Too long running while loop. Maximum iteration exceeded.";
            throw ELixError(ELixError::RuntimeError, ss.str());
        }
        try{
            for(auto i=1; i < exprs.size(); i++){
                [[maybe_unused]] auto _ = exprs[i]->eval(this);
            }
            test = exprs[0]->eval(this);
            ++iteration;
        }catch(const BreakSignal&){ break; }
        catch(const ContinueSignal& ){ continue; }
    }

    return Object();
}

// -*-
Object ELix::handle_for(Vec<Expression> exprs){
    // (for (x iterable) body)
    auto pred = (exprs.size() >= 1);
    this->check_argc(pred, "for");
    auto args = dynamic_cast<ListExpr*>(exprs[0].get());
    if(args==nullptr){
        std::stringstream ss;
        ss << "Malformed `for' expression. Expected the first argument\n";
        ss << "to be a list expression.";
        throw ELixError(ELixError::SyntaxError, ss.str());
    }
    if(args->items.size()!=2){
        std::stringstream ss;
        ss << "Malformed `for' expression. Expected the first argument\n";
        ss << "to be a list expression of two elements.";
        throw ELixError(ELixError::SyntaxError, ss.str());
    }

    auto var_ = dynamic_cast<SymbolExpr*>(args->items[0].get());
    if(var_==nullptr){
        std::stringstream ss;
        ss << "Malformed `for' expression. Expected the first argument\n";
        ss << "to be a list expression with the first element to be a symbol.";
        throw ELixError(ELixError::SyntaxError, ss.str());
    }

    auto var = var_->name.str();
    if(ELix::is_reserved_word(var)){
        std::stringstream ss;
        ss << std::quoted(var) << " is a builtin reserved word. It cannot be used to ";
        ss << "name a variable.";
        throw ELixError(ELixError::ValueError, ss.str());
    }

    auto iterable = exprs[1]->eval(this);
    if(!iterable.is_iterable()){
        std::stringstream ss;
        ss << "Malformed `for' expression. Expected the first argument\n";
        ss << "to be a list expression with the second element to be an iterable object\n";
        ss << "i.e a String, a List, an Array, a Set, or a Dict object.";
        throw ELixError(ELixError::SyntaxError, ss.str());
    }

    auto ctx = std::make_shared<Env>(this->m_runtime);
    auto saved = this->m_runtime;
    this->m_runtime = ctx;
    this->m_runtime->define(var, Object());
    constexpr auto MY_MAX_ITERATIONS = std::numeric_limits<i32>::max();
    u32 iteration = 0;

    auto checkIteration = [&iteration, MY_MAX_ITERATIONS]() mutable {
        if(iteration > MY_MAX_ITERATIONS){
            std::stringstream ss;
            ss << "Too long running while loop. Maximum iteration exceeded.";
            throw ELixError(ELixError::RuntimeError, ss.str());
        }
    };
    if(exprs.size()==1){
        for(;;){ // empty-body
            checkIteration();
            ++iteration;
        }           
    }
    if(iterable.is_string()){
        auto xstr = iterable.as_string().str();
        for(auto c: xstr){
            checkIteration();
            this->m_runtime->define(var, Object(String{std::string(1, c)}));
            try{
                for(auto i=1; i < exprs.size(); i++){
                    [[maybe_unused]] auto _ = exprs[i]->eval(this);
                }
                ++iteration;
            }catch(const BreakSignal& ){ break; }
            catch(const ContinueSignal& ){ continue; }
        }
    }else if(iterable.is_array()){
        auto items = iterable.as_array().items;
        for(auto item: items){
            checkIteration();
            this->m_runtime->define(var, item);
            try{
                for(auto i=1; i < exprs.size(); i++){
                    [[maybe_unused]] auto _ = exprs[i]->eval(this);
                }
                ++iteration;
            }catch(const BreakSignal& ){ break; }
            catch(const ContinueSignal& ){ continue; }
        }
    }else if(iterable.is_list()){
        auto items = iterable.as_list().items;
        for(auto item: items){
            checkIteration();
            this->m_runtime->define(var, item);
            try{
                for(auto i=1; i < exprs.size(); i++){
                    [[maybe_unused]] auto _ = exprs[i]->eval(this);
                }
                ++iteration;
            }catch(const BreakSignal& ){ break; }
            catch(const ContinueSignal& ){ continue; }
            
        }
    }else if(iterable.is_set()){
        auto items = iterable.as_set().hset;
        for(auto item: items){
            checkIteration();
            this->m_runtime->define(var, item);
            try{
                for(auto i=1; i < exprs.size(); i++){
                    [[maybe_unused]] auto _ = exprs[i]->eval(this);
                }
                ++iteration;
            }catch(const BreakSignal& ){ break; }
            catch(const ContinueSignal& ){ continue; }
        }
    }else {
        auto items = iterable.as_dict().items();
        for(auto item: items){
            checkIteration();
            this->m_runtime->define(var, Object(item));
            try{
                for(auto i=1; i < exprs.size(); i++){
                    [[maybe_unused]] auto _ = exprs[i]->eval(this);
                }
                ++iteration;
            }catch(const BreakSignal& ){ break; }
            catch(const ContinueSignal& ){ continue; }
        }
    }

    return Object();
}

// -*-
Object ELix::handle_fun(Vec<Expression> exprs){
    // (fun name params body)
    auto pred = (exprs.size() >= 2);
    this->check_argc(pred, "fun");
    auto name = dynamic_cast<SymbolExpr*>(exprs[0].get());
    if(name==nullptr){
        std::stringstream ss;
        ss << "Malformed function definition. The name my be a symbol, ";
        auto self = exprs[0]->eval(this);
        ss << "but we got " << std::quoted(self.type().str());
        throw ELixError(ELixError::SyntaxError, ss.str());
    }
    auto fname = name->name.str();
    this->validate_name(fname);

    auto xs = dynamic_cast<ListExpr*>(exprs[1].get());
    if(xs==nullptr){
        std::stringstream ss;
        ss << "Malformed function definition. The second argument in a function definition\n";
        ss << "must be a list. ";
        auto self = exprs[1]->eval(this);
        ss << "but we got " << std::quoted(self.type().str());
        throw ELixError(ELixError::SyntaxError, ss.str());
    }
    Vec<Symbol> params{};
    for(auto& item: xs->items){
        auto self = dynamic_cast<SymbolExpr*>(item.get());
        if(self==nullptr){
            std::stringstream ss;
            ss << "Found " << std::quoted(item->eval(this).type().str()) << " object type ";
            ss << "in function parameters list.";
            throw ELixError(ELixError::SyntaxError, ss.str());
        }
        this->validate_name(self->name.str());
        params.push_back(self->name);
    }
    Vec<Expression> body{};
    if(exprs.size() > 2){
        for(auto i=2; i < exprs.size(); i++){
            body.push_back(std::move(exprs[i]));
        }
    }

    Lambda lambda{};
    lambda.params = std::move(params);
    lambda.body = std::move(body);
    lambda.ctx = std::make_shared<Env>(this->m_runtime);
    lambda.named = true;
    this->m_runtime->define(fname, Object(lambda));

    return std::move(lambda);
}

// -*-
Object ELix::handle_macro(Vec<Expression> exprs){
    // (macro name params body)
    auto pred = (exprs.size() >= 2);
    this->check_argc(pred, "macro");
    
    auto name = dynamic_cast<SymbolExpr*>(exprs[0].get());
    if(name==nullptr){
        std::stringstream ss;
        ss << "Malformed macro definition. The name my be a symbol, ";
        auto self = exprs[0]->eval(this);
        ss << "but we got " << std::quoted(self.type().str());
        throw ELixError(ELixError::SyntaxError, ss.str());
    }
    auto fname = name->name.str();
    this->validate_name(fname);
    auto xs = dynamic_cast<ListExpr*>(exprs[1].get());
    if(xs==nullptr){
        std::stringstream ss;
        ss << "Malformed macro definition. The second argument in a function definition\n";
        ss << "must be a list. ";
        auto self = exprs[1]->eval(this);
        ss << "but we got " << std::quoted(self.type().str());
        throw ELixError(ELixError::SyntaxError, ss.str());
    }
    Vec<Symbol> params{};
    for(auto& item: xs->items){
        auto self = dynamic_cast<SymbolExpr*>(item.get());
        if(self==nullptr){
            std::stringstream ss;
            ss << "Found " << std::quoted(item->eval(this).type().str()) << " object type ";
            ss << "in macro parameters list.";
            throw ELixError(ELixError::SyntaxError, ss.str());
        }

        this->validate_name(self->name.str());
        params.push_back(self->name);
    }
    Vec<Expression> body{};
    if(exprs.size() > 2){
        for(auto i=2; i < exprs.size(); i++){
            body.push_back(std::move(exprs[i]));
        }
    }

    Macro macro{};
    macro.params = std::move(params);
    macro.body = std::move(body);
    macro.ctx = std::make_shared<Env>(this->m_runtime);
    this->m_runtime->define(fname, Object(macro));

    return std::move(macro);
}

// -*-
Object ELix::handle_lambda(Vec<Expression> exprs){
    // (lambda params body)
    auto pred = (exprs.size() >= 1);
    this->check_argc(pred, "lambda");
        
    auto xs = dynamic_cast<ListExpr*>(exprs[0].get());
    if(xs==nullptr){
        std::stringstream ss;
        ss << "Malformed lambda-expression. The second argument in a function definition\n";
        ss << "must be a list. ";
        auto self = exprs[1]->eval(this);
        ss << "but we got " << std::quoted(self.type().str());
        throw ELixError(ELixError::SyntaxError, ss.str());
    }
    Vec<Symbol> params{};
    for(auto& item: xs->items){
        auto self = dynamic_cast<SymbolExpr*>(item.get());
        if(self==nullptr){
            std::stringstream ss;
            ss << "Found " << std::quoted(item->eval(this).type().str()) << " object type ";
            ss << "in lambda-expression parameters list.";
            throw ELixError(ELixError::SyntaxError, ss.str());
        }
        this->validate_name(self->name.str());
        params.push_back(self->name);
    }
    Vec<Expression> body{};
    if(exprs.size() > 2){
        for(auto i=2; i < exprs.size(); i++){
            body.push_back(std::move(exprs[i]));
        }
    }

    Lambda lambda{};
    lambda.params = std::move(params);
    lambda.body = std::move(body);
    lambda.ctx = std::make_shared<Env>(this->m_runtime);
    lambda.named = false;

    return std::move(lambda);
}

// -*-
Object ELix::handle_and(Vec<Expression> exprs){
    // (and expr1 expr2)
    auto pred = (exprs.size()==2);
    this->check_argc(pred, "and");
    auto x = exprs[0]->eval(this);
    auto y = exprs[1]->eval(this);
    if(!x.is_bool() && !y.is_bool()){
        std::stringstream ss;
        ss << "Malformed `and' expression. The two arguments of the `and'\n";
        ss << "operator must evaluate to a boolean.";
        throw ELixError(ELixError::SyntaxError, ss.str());
    }
    if(x.as_bool() && y.as_bool()){
        return Object(true);
    }
    return Object(false);
}

// -*-
Object ELix::handle_or(Vec<Expression> exprs){
    // (or expr1 expr2)
    auto pred = (exprs.size()==2);
    this->check_argc(pred, "or");
    auto x = exprs[0]->eval(this);
    auto y = exprs[1]->eval(this);
    if(!x.is_bool() && !y.is_bool()){
        std::stringstream ss;
        ss << "Malformed `or' expression. The two arguments of the `or'\n";
        ss << "operator must evaluate to a boolean.";
        throw ELixError(ELixError::SyntaxError, ss.str());
    }
    if(x.as_bool() || y.as_bool()){
        return Object(true);
    }
    return Object(false);
}

// -*-
Object ELix::handle_not(Vec<Expression> exprs){
    // (not expr)
    auto pred = (exprs.size()==1);
    this->check_argc(pred, "not");
    auto arg = exprs[0]->eval(this);
    if(!arg.is_bool()){
        std::stringstream ss;
        ss << "Malformed `not' expression. The argument of the `not'\n";
        ss << "operator must evaluate to a boolean.";
        throw ELixError(ELixError::SyntaxError, ss.str());
    }
    auto val = !arg.as_bool();
    return Object(val);
}

// -*-
Object ELix::handle_continue(Vec<Expression> exprs){
    // (continue)
    throw ContinueSignal{};
}

// -*-
Object ELix::handle_break(Vec<Expression> exprs){
    // (break)
    throw BreakSignal{};
}

// -*-
Object ELix::handle_list(Vec<Expression> exprs){
    // function call
    auto word = dynamic_cast<SymbolExpr*>(exprs[0].get());
    auto name = word->name.str();
    auto fun = exprs[0]->eval(this);
    // This is a function call
    // Get the associated callable object
    // Check that `fun' is actually a callable
    if(!fun.is_callable()){
        std::stringstream ss;
        ss << "" << std::quoted(name) << " is not a callable object.";
        throw ELixError(ELixError::SyntaxError, ss.str());
    }
    Object result{};
    Vec<Object> argv{};
    if(exprs.size() > 1){
        for(auto i=1; i < exprs.size(); i++){
            argv.push_back(exprs[i]->eval(this));
        }
        if(fun.is_macro()){
            auto macro = fun.as_macro();
            result = macro(argv, this);
        }else if(fun.is_lambda() || fun.is_function()){
            auto lambda = fun.as_lambda();
            result = lambda(argv, this);
        }else if(fun.is_func()){
            auto func = fun.as_func();
            result = func(argv, this);
        }
    }

    return result;
}

// -*-
Expression ELix::handle_quote(Vec<Expression> exprs){
    auto pred = (exprs.size()==1);
    this->check_argc(pred, "quote");
    return std::move(exprs[0]);
}

// -*-
Expression ELix::build_expression(const Object& self){
    // Expression expr = nullptr;
    if(self.is_symbol()){
        auto expr = std::make_unique<SymbolExpr>();
        expr->name = self.as_symbol();
        return std::move(expr);
    }else if(ELix::is_literal(self)){
        auto expr = std::make_unique<LiteralExpr>();
        expr->obj = self;
        return std::move(expr);
    }else if(self.is_list()){
        auto expr = std::make_unique<ListExpr>();
        expr->items = {};
        for(auto item: self.as_list().items){
            expr->items.push_back(std::move(this->build_expression(item)));
        }
        return std::move(expr);
    }else if(self.is_array()){
        auto expr = std::make_unique<ArrayExpr>();
        expr->items = {};
        for(auto item: self.as_list().items){
            expr->items.push_back(std::move(this->build_expression(item)));
        }
        return std::move(expr);
    }else if(self.is_dict()){
        auto expr = std::make_unique<DictExpr>();
        expr->items = {};
        for(auto item: self.as_list().items){
            expr->items.push_back(std::move(this->build_expression(item)));
        }
        return std::move(expr);
    }else if(self.is_set()){
        auto expr = std::make_unique<SetExpr>();
        expr->items = {};
        for(auto item: self.as_list().items){
            expr->items.push_back(std::move(this->build_expression(item)));
        }
        return std::move(expr);
    }
    auto expr = std::make_unique<PairExpr>();
    auto pair = self.as_pair();
    expr->key = this->build_expression(pair.key);
    expr->val = this->build_expression(pair.val);

    return std::move(expr);
}

// -*-
Expression ELix::handle_quasiquote(Vec<Expression> exprs){
    auto pred = (exprs.size()==1);
    this->check_argc(pred, "quote");
    return std::move(exprs[0]);
}

// -*-
Expression ELix::handle_unquote(Vec<Expression> exprs){
    auto pred = (exprs.size()==1);
    this->check_argc(pred, "unquote");
    return std::move(exprs[0]);
}

// -*-
Expression ELix::handle_unquote_splicing(Vec<Expression> exprs){
    auto pred = (exprs.size()==1);
    this->check_argc(pred, "unquote-splicing");
    auto xs = dynamic_cast<ListExpr*>(exprs[0].get());
    if(xs==nullptr){
        std::stringstream ss;
        ss << "Invalid argument type to `unquote-splicing' i.e ";
        ss << std::quoted(",@") << ".\nExpect argument to be list object.";
        throw ELixError(ELixError::SyntaxError, ss.str());
    }
    Vec<Expression> argv{};
    for(auto&& expr: exprs){
        auto self = dynamic_cast<ListExpr*>(expr.get());
        if(self != nullptr){
            auto items = self->items;
            if(items.size() != 0){
                auto sym = dynamic_cast<SymbolExpr*>(items[0].get());
                if(sym!=nullptr){
                    if(sym->name.str()=="unquote"){
                        if(items.size()!=2){
                            std::stringstream ss;
                            ss << "the argument to unquote expression inside the ";
                            ss << "unquote-splicing expression must be a list object.";
                            throw ELixError(ELixError::SyntaxError, ss.str());
                        }
                        auto args = dynamic_cast<ListExpr*>(items[1].get());
                        if(args==nullptr){
                            std::stringstream ss;
                            ss << "the argument to unquote expression inside the ";
                            ss << "unquote-splicing expression must be a list object.";
                            throw ELixError(ELixError::SyntaxError, ss.str());
                        }
                        for(auto& arg: args->items){
                            argv.push_back(this->build_expression(arg->eval(this))); 
                        }
                    }else{
                        Vec<Object> vec{};
                        for(auto& arg: items){
                            vec.push_back(arg->eval(this)); 
                        }
                        argv.push_back(this->build_expression(Object(List{vec})));
                    }
                }else{
                    std::stringstream ss;
                    ss << "Malformed list-expression found inside an ";
                    ss << "unquote-splicing expression.";
                    throw ELixError(ELixError::SyntaxError, ss.str());
                }
            }else{
                Vec<Object> vec{Object(Symbol{"list"})};
                argv.push_back(std::move(this->build_expression(Object(List{vec}))));
            }
        }else{
            argv.push_back(std::move(expr));
        }
    }

    auto result = std::make_unique<ListExpr>();
    result->items = std::move(argv);

    return std::move(result);
}

// -*-------------------------------*-
// -*- ModuleLoader implementation -*-
// -*-------------------------------*-
Module::Module(const Symbol& name, ELix* elix){
    this->configure(name);
    this->setup();
}

Module::Module(const std::string& filename, ELix* elix){
    this->configure(filename);
    this->setup();
}

Module::Module(const fs::path& filepath, ELix* elix){
    this->configure(filepath);
    this->setup();
}


Module::Module(const Module& mod) noexcept
: m_name{mod.m_name}
, m_filename{mod.m_filename}
, m_fullpath{mod.m_fullpath}
, m_cache{mod.m_cache}
, m_elix{mod.m_elix}
{}

Module::Module(Module&& mod) noexcept
: m_name{std::move(mod.m_name)}
, m_filename{std::move(mod.m_filename)}
, m_fullpath{std::move(mod.m_fullpath)}
, m_cache{std::move(mod.m_cache)}
, m_elix{mod.m_elix}
{}

Module& Module::operator=(const Module& mod) noexcept{
    if(this != &mod){
        this->m_name = mod.m_name;
        this->m_filename = mod.m_filename;
        this->m_fullpath = mod.m_fullpath;
        this->m_cache = mod.m_cache;
        this->m_elix = mod.m_elix;
    }

    return *this;
}

Module& Module::operator=(Module&& mod) noexcept{
    if(this != &mod){
        this->m_name = std::move(mod.m_name);
        this->m_filename = std::move(mod.m_filename);
        this->m_fullpath = std::move(mod.m_fullpath);
        this->m_cache = std::move(mod.m_cache);
        this->m_elix = mod.m_elix;
    }

    return *this;
}

void Module::load(Context& ctx) const{
    for(const auto& [key, val]: this->m_cache){
        ctx->define(key, val);
    }
}

void Module::add(const std::string& name, const Object& val){
    this->m_cache[name] = val;
}

const Object& Module::get(const std::string& name) const{
    auto entry = this->m_cache.find(name);
    if(entry == this->m_cache.end()){
        std::stringstream ss;
        ss << "symbol " << std::quoted(name) << " not found in module ";
        if(this->filename().length()==0 || this->fullpath().string().length()==0){
            ss << std::quoted(this->m_name);
        }else{
            ss << std::quoted(this->m_filename);
        }
        throw ELixError(ELixError::KeyError, ss.str());
    }
    return entry->second;
}

// -*-
void Module::setup(void){
    auto src = ELix::readfile(this->m_fullpath);
    auto myruntime = this->m_elix->m_runtime;
    this->m_elix->m_runtime = std::make_shared<Env>(this->m_elix->m_runtime);
    Tokenizer tokenizer{src};
    Parser parser{tokenizer};
    auto exprs = parser.parse();
    for(auto&& expr: exprs){
         [[maybe_unused]] auto _ = expr->eval(this->m_elix);
    }
    // fill the cache
    for(const auto& [key, val]: this->m_elix->m_runtime->bindings()){
        this->m_cache[key] = val;
    }
    this->m_elix->m_runtime = myruntime;
}

// -*-
void Module::configure(const Symbol& name){
    this->m_name = name.str();
    this->m_filename = "";
    this->m_fullpath = ""; 
}

// -*-
void Module::configure(const std::string& filename){
    auto path = fs::path(filename);
    auto ext = path.extension();
    if(ext!=ELix::scriptExt){
        std::stringstream ss;
        ss << "invalid file extension. ELix script file extension must be ";
        ss << std::quoted(ELix::scriptExt) << ".";
        throw ELixError(ELixError::ValueError, ss.str());
    }
    this->m_filename = filename;
    this->m_fullpath = fs::absolute(path);
    auto fname = path.filename().string();
    auto len = fname.length() - std::string(".elx").length();
    this->m_name = fname.substr(0, len);
}

// -*-
void Module::configure(const fs::path& filepath){
    auto ext = filepath.extension();
    if(ext!=ELix::scriptExt){
        std::stringstream ss;
        ss << "invalid file extension. ELix script file extension must be ";
        ss << std::quoted(ELix::scriptExt) << ".";
        throw ELixError(ELixError::ValueError, ss.str());
    }
    this->m_filename = filepath.string();
    this->m_fullpath = fs::absolute(filepath);
    auto fname = filepath.filename().string();
    auto len = fname.length() - std::string(".elx").length();
    this->m_name = fname.substr(0, len);
}

size_t ModuleHash::operator()(const Module& arg) const{
    auto text = (
        arg.name() + arg.filename() + arg.fullpath().string()
    );
    return std::hash<std::string>{}(text);
}

bool ModuleEqual::operator()(const Module& lhs, const Module& rhs) const{
    auto x = (lhs.name()+lhs.filename()+lhs.fullpath().string());
    auto y = (rhs.name()+rhs.filename()+rhs.fullpath().string());
    return (x==y);
}

// -*--------------------------------------------------------------------------*-
}//-*- end::namespace::ekasoft::elx                                           -*-
// -*--------------------------------------------------------------------------*-