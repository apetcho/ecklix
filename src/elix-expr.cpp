#include "elix.hpp"
#include<sstream>
#include<iomanip>

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
    std::stringstream ss;
    ss << "(";
    for(auto i=0; i < this->items.size(); i++){
        if(i > 0){ ss << " "; }
        ss << this->items[i]->str();
    }
    ss << ")";
    return ss.str();
}

std::string ListExpr::repr(void) const{
    std::stringstream ss;
    ss << "\"(";
    for(auto i=0; i < this->items.size(); i++){
        if(i > 0){ ss << " "; }
        ss << this->items[i]->repr();
    }
    ss << ")\"";
    return ss.str();
}

// -*----------------------------*-
// -*- ArrayExpr implementation -*-
// -*----------------------------*-
Object ArrayExpr::eval(Visitor visitor){
    return visitor->eval(*this);
}

std::string ArrayExpr::str(void) const{
    std::stringstream ss;
    ss << "[";
    for(auto i=0; i < this->items.size(); i++){
        if(i > 0){ ss << " "; }
        ss << this->items[i]->str();
    }
    ss << "]";
    return ss.str();
}

std::string ArrayExpr::repr(void) const{
    std::stringstream ss;
    ss << "\"[";
    for(auto i=0; i < this->items.size(); i++){
        if(i > 0){ ss << " "; }
        ss << this->items[i]->repr();
    }
    ss << "]\"";
    return ss.str();
}

// -*---------------------------*-
// -*- PairExpr implementation -*-
// -*---------------------------*-
Object PairExpr::eval(Visitor visitor){
    return visitor->eval(*this);
}

std::string PairExpr::str(void) const{
    std::stringstream ss;
    ss << "#(" << this->key->str() << " " << this->val->str() << ")";
    return ss.str();
}

std::string PairExpr::repr(void) const{
    std::stringstream ss;
    ss <<"\"#(" << this->key->repr() << " " << this->val->repr() << ")\"";
    return ss.str();
}

// -*---------------------------*-
// -*- DictExpr implementation -*-
// -*---------------------------*-
Object DictExpr::eval(Visitor visitor){
    return visitor->eval(*this);
}

std::string DictExpr::str(void) const{
    std::stringstream ss;
    ss << "{";
    for(auto i=0; i < this->items.size(); i++){
        if(i > 0){ ss << " "; }
        ss << this->items[i]->str();
    }
    ss << "}";

    return ss.str();
}

std::string DictExpr::repr(void) const{
    std::stringstream ss;
    ss << "\"{";
    for(auto i=0; i < this->items.size(); i++){
        if(i > 0){ ss << " "; }
        ss << this->items[i]->repr();
    }
    ss << "}\"";
    return ss.str();
}

// -*---------------------------*-
// -*- SetExpr implementation -*-
// -*---------------------------*-
Object SetExpr::eval(Visitor visitor){
    return visitor->eval(*this);
}

std::string SetExpr::str(void) const{
    std::stringstream ss;
    ss << "#{";
    for(auto i=0; i < this->items.size(); i++){
        if(i > 0){ ss << " "; }
        ss << this->items[i]->str();
    }
    ss << "}";

    return ss.str();
}

std::string SetExpr::repr(void) const{
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*--------------------------------------------------------------------------*-
}//-*- end::namespace::ekasoft::elx                                           -*-
// -*--------------------------------------------------------------------------*-