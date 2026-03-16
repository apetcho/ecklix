#include "elix.hpp"

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
    
ExprVisitor* ELix::visitor;
UniquePtr<Env> ELix::prelude;
Context ELix::runtime; 
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
    ELix::visitor = nullptr;
    ELix::prelude = nullptr;
    ELix::runtime = nullptr;
    ELix::BuiltinModules = {};

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

    ELix::init_builtin_modules();
}

// -*-
bool ELix::is_reserved_word(const std::string& word){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
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
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
void ELix::run(const fs::path& scriptpath, const Vec<Object>& args){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
std::string ELix::readfile(const fs::path& filename){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
std::string ELix::readfile(const std::string& filename){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
std::string ELix::readfile(const char* filename){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
std::string ELix::input(void){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

void ELix::add_module(const Module& mymodule){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
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
Context ELix::load(const Symbol& name){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Context ELix::load(const fs::path& script){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Context ELix::load(const std::string& script){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::eval(Expression expr){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::eval(LiteralExpr& expr){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::eval(SymbolExpr& expr){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::eval(ListExpr& expr){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::eval(ArrayExpr& expr){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::eval(DictExpr& expr){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::eval(SetExpr& expr){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
void ELix::print(const Object& obj){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::handle_import(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::handle_progn(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::handle_if(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Object ELix::handle_let(Vec<Expression> exprs){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
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

void Module::load(Context& ctx){
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