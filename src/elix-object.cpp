#include "elix.hpp"
#include<sstream>
#include<iomanip>
#include<cmath>

// -*--------------------------------------------------------------------------*-
// -*- begin::namespace::ekasoft::elx                                         -*-
// -*--------------------------------------------------------------------------*-
namespace ekasoft::elx{
// -
// -*-----------------------------------------*-
// -*- begin::namespace::ekasoft::elx::utils -*-
// -*-----------------------------------------*-
namespace utils{
// -
std::size_t Hash::operator()(const Object& key) const{
    if(!key.is_hashable()){
        std::stringstream ss;
        ss << "object of type " << std::quoted(key.type().str());
        ss << " is not hashable";
        throw ELixError(ELixError::TypeError, ss.str());
    }

    return std::hash<std::string>{}(key.str());
}

// -*-
bool Equal::operator()(const Object& lhs, const Object& rhs) const{
    auto xstr = lhs.str();
    auto ystr = rhs.str();
    return (xstr==ystr);
}

// -*-----------------------------------------*-
}//-*- end::namespace::ekasoft::elx::utils   -*-
// -*-----------------------------------------*-

// --------------
// -*- Number -*-
// --------------
Number::Number(): m_value{i64(0)} {}

Number::Number(i64 num): m_value{num} {}

Number::Number(f64 num): m_value{num} {}

Number::Number(const Number& num) noexcept
: m_value{num.m_value}
{}

Number::Number(Number&& num) noexcept
: m_value{std::move(num.m_value)}
{}

Number& Number::operator=(const Number& num) noexcept{
    if(this != &num){
        this->m_value = num.m_value;
    }
    return *this;
}

Number& Number::operator=(Number&& num) noexcept{
    if(this != &num){
        this->m_value = std::move(num.m_value);
    }
    return *this;
}

std::string Number::str(void) const{
    std::stringstream ss;
    auto ptr = std::get_if<i64>(&this->m_value);
    if(ptr!=nullptr){ ss << *ptr; }
    else{ ss << std::get<f64>(this->m_value); }

    return ss.str();
}

std::string Number::repr(void) const{
    return this->str();
}

// -*-
Number::operator bool() const{
    if(this->is_integer()){
        return (std::get<i64>(this->m_value)==0);
    }
    return (std::get<f64>(this->m_value)==0.0);
}

// -*-
Number::operator i64() const{
    if(this->is_integer()){
        return std::get<i64>(this->m_value);
    }
    return static_cast<i64>(std::get<f64>(this->m_value));
}

Number::operator f64() const{
    if(this->is_integer()){
        return static_cast<f64>(std::get<i64>(this->m_value));
    }
    return std::get<f64>(this->m_value);
}

// -*-
bool Number::as_bool(void){
    if(this->is_integer()){
        i64 num{};
        this->value(num);
        return (num=0);
    }
    f64 num{};
    this->value(num);
    return (num=0.0);
}

i64 Number::as_integer(void){
    if(this->is_integer()){
        return std::get<i64>(this->m_value);
    }
    f64 num{};
    this->value(num);
    return static_cast<i64>(num);
}

f64 Number::as_float(void){
    if(this->is_integer()){
        i64 num{};
        this->value(num);
        return static_cast<f64>(num);
    }
    return std::get<f64>(this->m_value);
}

bool Number::is_integer(void) const{
    return std::holds_alternative<i64>(this->m_value);
}

Number Number::clone(void) const{
    if(this->is_integer()){
        return Number(static_cast<i64>(*this));
    }
    return Number(static_cast<f64>(*this));
}

// -*-
Number Number::abs(void) const{
    if(this->is_integer()){
        return Number(std::abs(static_cast<i64>(*this)));
    }
    return Number(std::abs(static_cast<f64>(*this)));
}

// -*-
Number Number::ceil(void) const{
    return Number(std::ceil(static_cast<f64>(*this)));
}

// -*-
Number Number::floor(void) const{
    return Number(std::floor(static_cast<f64>(*this)));
}


// -*-
Number Number::round(void) const{
    return Number(std::round(static_cast<f64>(*this)));
}


// -*-
Number Number::truncate(void) const{
    return Number(std::trunc(static_cast<f64>(*this)));
}


// -*-
Number Number::sin(void) const{
    return Number(std::sin(static_cast<f64>(*this)));
}


// -*-
Number Number::cos(void) const{
    return Number(std::cos(static_cast<f64>(*this)));
}


// -*-
Number Number::tan(void) const{
    return Number(std::tan(static_cast<f64>(*this)));
}

// -*-
Number Number::asin(void) const{
    return Number(std::asin(static_cast<f64>(*this)));
}

// -*-
Number Number::acos(void) const{
    return Number(std::acos(static_cast<f64>(*this)));
}

// -*-
Number Number::atan(void) const{
    return Number(std::atan(static_cast<f64>(*this)));
}

// -*-
Number Number::atan2(const Number& rhs) const{
    auto y = static_cast<f64>(rhs);
    auto x = static_cast<f64>(*this);
    return Number(std::atan2(y, x));
}

// -*-
Number Number::sinh(void) const{
    return Number(std::sinh(static_cast<f64>(*this)));
}

// -*-
Number Number::cosh(void) const{
    return Number(std::cosh(static_cast<f64>(*this)));
}

// -*-
Number Number::tanh(void) const{
    return Number(std::tanh(static_cast<f64>(*this)));
}

// -*-
Number Number::asinh(void) const{
    return Number(std::asinh(static_cast<f64>(*this)));
}

// -*-
Number Number::acosh(void) const{
    return Number(std::acosh(static_cast<f64>(*this)));
}

// -*-
Number Number::atanh(void) const{
    return Number(std::atanh(static_cast<f64>(*this)));
}

// -*-
Number Number::exp(void) const{
    return Number(std::exp(static_cast<f64>(*this)));
}

// -*-
Number Number::expm1(void) const{
    return Number(std::expm1(static_cast<f64>(*this)));
}

// -*-
Number Number::exp2(void) const{
    return Number(std::exp2(static_cast<f64>(*this)));
}
/*
// -*-
class Number:: final{
public:

// -
Number Number::exp2(void) const{}
Number Number::log(void) const{}
Number Number::log2(void) const{}
Number Number::log10(void) const{}
Number Number::log1p(void) const{}
Number Number::erf(void) const{}
Number Number::erfc(void) const{}
Number Number::tgamma(void) const{}
Number Number::lgamma(void) const{}
Number Number::isnan(void) const{}
Number Number::isinf(void) const{}
Number Number::isfinite(void) const{}
bool Number::operator!(){}
Number& Number::operator-(){}
Number& Number::operator~(){}

Number operator+(const Number& lhs, const Number& rhs){}
Number operator-(const Number& lhs, const Number& rhs){}
Number operator*(const Number& lhs, const Number& rhs){}
Number operator/(const Number& lhs, const Number& rhs){}
Number operator%(const Number& lhs, const Number& rhs){}

Number operator&(const Number& lhs, const Number& rhs){}
Number operator|(const Number& lhs, const Number& rhs){}
Number operator^(const Number& lhs, const Number& rhs){}
Number operator<<(const Number& lhs, const Number& rhs){}
Number operator>>(const Number& lhs, const Number& rhs){}

bool operator&&(const Number& lhs, const Number& rhs){}
bool operator&&(const Number& lhs, const Number& rhs){}

bool operator==(const Number& lhs, const Number& rhs){}
bool operator!=(const Number& lhs, const Number& rhs){}
bool operator<(const Number& lhs, const Number& rhs){}
bool operator>(const Number& lhs, const Number& rhs){}
bool operator<=(const Number& lhs, const Number& rhs){}
bool operator>=(const Number& lhs, const Number& rhs){}

private:
    using Value = std::variant<i64, f64>;

    Value m_value;
};

// -*-
struct Symbol{
    std::string value;
std::string Symbol::str(void) const;
std::string Symbol::repr(void) const;
Symbol Symbol::clone(void) const;

bool operator==(const Symbol& lhs, const Symbol& rhs);
bool operator!=(const Symbol& lhs, const Symbol& rhs);
};

// -*-
struct Lambda{
    Vec<Symbol> params;
    Vec<Expression> body;
    Context ctx;
std::string Lambda::str(void) const;
std::string Lambda::repr(void) const;
Lambda Lambda::clone(void) const;
Object Lambda::operator()(const Vec<Object>& args);
};

// -*-
struct List{
    Vec<Object> items;
std::string List::str(void) const;
std::string List::repr(void) const;
List List::clone(void) const;
i64 List::find(const Object& args) const;
List& List::reverse(void);
List& List::concat(const List& args);
i64 List::len(void) const;
Object List::head(void) const;
List List::tail(void) const;
Object List::first(void) const;
Object List::last(void) const;
List& List::push(const Object& arg);
List& List::pop(void);
List& List::insert(i64 idx, const Object& obj);
Object List::remove(i64 idx);
};


// -*-
struct Array{
    Vec<Object> items;
std::string Array::str(void) const;
std::string Array::repr(void) const;
Array Array::clone(void) const;

i64 Array::find(const Object& arg, i64 from=0);
Array& Array::reverse(void);
Array& Array::concat(const Array& array);
i64 Array::len(void) const;
Array Array::slice(i64 i, i64 j) const;
Array& Array::insert(i64 idx,  const Object& arg);
Object Array::get(i64 idx) const;
Array& Array::set(i64 idx, const Object& arg);
Array& Array::splice(i64 idx, const Object& arg);
bool Array::any(const Object& predicate) const;
bool Array::all(const Object& predicate) const;
Object Array::reduce(const Object& fn, const Object& initVal) const;
String& Array::sort(const Object& fn);
};

// -*-
struct String{
    std::string text;

std::string String::str(void) const;
std::string String::repr(void) const;
String String::clone(void) const;

i64 String::find(const String& needle, i64 from=0) const;
String& String::reverse(void);
String& String::concat(const String& arg);
i64 String::len(void) const;

bool operator+(const String& lhs, const String& rhs);
bool operator==(const String& lhs, const String& rhs);
bool operator!=(const String& lhs, const String& rhs);

Vec<String> String::split(const String delim=String(" "));
String String::slice(i64 i, i64 j) const;
bool String::startswith(const String& rhs) const;
bool String::endswith(const String& rhs) const;
String& String::ltrim(void);
String& String::rtrim(void);
String& String::trim(void);
String& String::lower(void);
String& String::upper(void);
String& String::capitalize(void);
String& String::title(void);
Vec<i64> String::find_all(const String& needle);
String& String::replace(const String& needle, i64 from=0);
String& String::replace_all(void);
String String::chr(void) const;
i64 String::ord(void) const;
String String::substr(i64 i, i64 j) const;
};

// -*-
struct Pair{
    Object key;
    Object val;
std::string Pair::str(void) const;
std::string Pair::repr(void) const;
String Pair::clone(void) const;
};

// -*-
struct Dict{ // Dict
    HashMap hmap;

std::string Dict::str(void) const;
std::string Dict::repr(void) const;
Dict Dict::clone(void) const;

Object Dict::find(const Object& key) const;
Dict& Dict::concat(const Dict&& dict);
i64 Dict::len(void) const;
Object Dict::get(const Object& key) const;
Dict& Dict::set(const Object& key, const Object& val);
Dict& Dict::update(const Object& key, const Object& val);
Vec<Object> Dict::keys(void) const;
Vec<Object> Dict::values(void) const;
Object Dict::popitem(const Object& key) const;
Dict& Dict::clear(void);
Vec<Pair> Dict::items(const Vec<Object>& args);
};

// -*-
struct Set{
    HashSet hset;

std::string Set::str(void) const;
std::string Set::repr(void) const;
Set Set::clone(void) const;

bool Set::find(const Object& needle) const;
Set& Set::concat(const Set& args);
i64 Set::len(void) const;
Set& Set::add(const Object& key);
Set Set::set_union(const Set& rhs) const;
Set Set::intersection(const Set& rhs) const;
Set Set::symmetric_difference(const Set& rhs) const;
Set& Set::clear(void);
Set& Set::discard(const Object& key);
Object Set::pop(void);
bool Set::isdisjoint(const Set& rhs) const;
bool Set::issubset(const Set& rhs) const;
bool Set::issuperset(const Set& rhs) const;
};

struct Func{
    Fn fn;
std::string Func::str(void) const;
std::string Func::repr(void) const;
Func Func::clone(void) const;
Object Func::operator()(const Vec<Object>& args);
};

struct Macro{
    Vec<Symbol> params;
    Vec<Expression> body;
    Context ctx;
std::string Macro::str(void) const;
std::string Macro::repr(void) const;
Macro Macro::clone(void) const;
Expression Macro::expand(const Expression& expr, Context& ctx);
Object Macro::operator()(const Vec<Object>& args);
};

// -*-
class Object final{
public:
Object::Object();
Object::Object(bool b);
Object::Object(i64 num);
Object::Object(f64 num);
Object::Object(const Number& num);
Object::Object(const Symbol& sym);
Object::Object(const char* cstr);
Object::Object(const std::string& str);
Object::Object(const String& str);
Object::Object(const Array& array);
Object::Object(const List& xs);
Object::Object(const Dict& xmap);
Object::Object(const Set& xset);
Object::Object(const Pair& pair);
Object::Object(const Lambda& lambda);
Object::Object(const Macro& macro);
Object::Object(const Func& func);
Object::Object(const Object& obj) noexcept;
Object::Object(Object&& obj) noexcept;
Object& Object::operator=(const Object& obj) noexcept;
Object& Object::operator=(Object&& obj) noexcept;

~Object(){}

bool Object::is_nil(void) const;
bool Object::is_integer(void) const;
bool Object::is_float(void) const;
bool Object::is_number(void) const;
bool Object::is_symbol(void) const;
bool Object::is_string(void) const;
bool Object::is_array(void) const;
bool Object::is_list(void) const;
bool Object::is_macro(void) const;
bool Object::is_lambda(void) const;
bool Object::is_func(void) const;
bool Object::is_callable(void) const;
bool Object::is_pair(void) const;

i64 Object::as_integer(void) const;
f64 Object::as_float(void) const;
Symbol Object::as_symbol(void) const;
String Object::as_string(void) const;
Array Object::as_array(void) const;
Dict Object::as_dict(void) const;
List Object::as_list(void) const;
Set Object::as_set(void) const;
Lambda Object::as_lambda(void) const;
Macro Object::as_macro(void) const;
Func Object::as_func(void) const;
Pair Object::as_pair(void) const;

std::string Object::str(void) const;
std::string Object::repr(void) const;
Object Object::clone(void) const;

bool Object::is_hashable(void) const;

private:
    using Value = std::variant<
        Nil, bool, Number, String, Symbol, Func, Lambda, Macro,
        Pair, List, Array, Dict, Set
    >;

    Value m_value;
};

// -*-
Symbol ELixError::ValueError;
Symbol ELixError::TypeError;
Symbol ELixError::SyntaxError;
Symbol ELixError::RuntimeError;
Symbol ELixError::KeyError;
Symbol ELixError::IndexError;
class ELixError: public std::runtime_error {
public:
ELixError::ELixError();
ELixError::ELixError(const Symbol& sym);
ELixError::ELixError(const Symbol& sym, const std::string& msg);

std::string ELixError::describe(void) const;
private:
    Symbol m_kind;
};

// -----------
// -*- Env -*-
// -----------
class Env : public std::enable_shared_from_this<Env> {
public:
Env::Env(Context parent=nullptr);

void Env::define(const std::string& name, const Object& obj);
bool Env::update(const std::string& name, const Object& obj);
Object Env::get(const std::string& name);

private:
std::map<std::string, Object> m_bindins;
Context m_parent;
};

*/

// -*--------------------------------------------------------------------------*-
}//-*- end::namespace::ekasoft::elx                                           -*-
// -*--------------------------------------------------------------------------*-