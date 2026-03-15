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
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
bool ELix::is_reserved_word(const std::string& word){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
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
Context ModuleLoader::load(const Symbol& name,  Context ctx){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Context ModuleLoader::load(const fs::path& scritpepath, Context ctx){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Context ModuleLoader::load(const std::string& scritpepath, Context ctx){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
Context ModuleLoader::load(const char* scritpepath, Context ctx){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
void ModuleLoader::setup(ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*--------------------------------------------------------------------------*-
}//-*- end::namespace::ekasoft::elx                                           -*-
// -*--------------------------------------------------------------------------*-