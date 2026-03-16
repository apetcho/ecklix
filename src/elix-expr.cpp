#include "elix.hpp"

// -*--------------------------------------------------------------------------*-
// -*- begin::namespace::ekasoft::elx                                         -*-
// -*--------------------------------------------------------------------------*-
namespace ekasoft::elx{
// -

// -*------------------------------*-
// -*- LiteralExpr implementation -*-
// -*------------------------------*-
Object LiteralExpr::eval(Visitor visitor){
    return visitor->eval(*this);
}

std::string LiteralExpr::str(void) const{
    return this->obj.str();
}

std::string LiteralExpr::repr(void) const{
    return this->obj.repr();
}

// -*-----------------------------*-
// -*- SymbolExpr implementation -*-
// -*-----------------------------*-
Object SymbolExpr::eval(Visitor visitor){
    return visitor->eval(*this);
}

std::string SymbolExpr::str(void) const{
    return this->name.str();
}

std::string SymbolExpr::repr(void) const{
    return this->name.repr();
}

// -*---------------------------*-
// -*- ListExpr implementation -*-
// -*---------------------------*-
Object ListExpr::eval(Visitor visitor){
    return visitor->eval(*this);
}

std::string ListExpr::str(void) const{
    Vec<Object> xs{};
    for(const auto& expr: this->items){
        xs.push_back(Object(String{expr->str()}));
    }

    auto result = Object(List{xs});
    return result.str();
}

std::string ListExpr::repr(void) const{
    Vec<Object> xs{};
    for(const auto& expr: this->items){
        xs.push_back(Object(String{expr->repr()}));
    }

    auto result = Object(List{xs});
    return result.repr();
}

// -*----------------------------*-
// -*- ArrayExpr implementation -*-
// -*----------------------------*-
Object ArrayExpr::eval(Visitor visitor){
    return visitor->eval(*this);
}

std::string ArrayExpr::str(void) const{
    Vec<Object> xs{};
    for(const auto& expr: this->items){
        xs.push_back(Object(String{expr->str()}));
    }

    auto result = Object(Array{xs});
    return result.str();
}

std::string ArrayExpr::repr(void) const{
    Vec<Object> xs{};
    for(const auto& expr: this->items){
        xs.push_back(Object(String{expr->repr()}));
    }

    auto result = Object(Array{xs});
    return result.str();
}

// -*---------------------------*-
// -*- PairExpr implementation -*-
// -*---------------------------*-
Object PairExpr::eval(Visitor visitor){
    return visitor->eval(*this);
}

std::string PairExpr::str(void) const{
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

std::string PairExpr::repr(void) const{
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*---------------------------*-
// -*- DictExpr implementation -*-
// -*---------------------------*-
Object DictExpr::eval(Visitor visitor){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

std::string DictExpr::str(void) const{
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

std::string DictExpr::repr(void) const{
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*---------------------------*-
// -*- SetExpr implementation -*-
// -*---------------------------*-
Object SetExpr::eval(Visitor visitor){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

std::string SetExpr::str(void) const{
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

std::string SetExpr::repr(void) const{
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*--------------------------------------------------------------------------*-
}//-*- end::namespace::ekasoft::elx                                           -*-
// -*--------------------------------------------------------------------------*-