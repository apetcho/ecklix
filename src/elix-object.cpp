#include "elix.hpp"
#include<type_traits>
#include<typeindex>
#include<typeinfo>
#include<stdexcept>
#include<algorithm>
#include<sstream>
#include<iomanip>
#include<variant>
#include<cstring>
#include<cerrno>
#include<cctype>
#include<random>
#include<cfenv>
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

bool Less::operator()(const Object& lhs, const Object& rhs) const{
    auto xstr = lhs.str();
    auto ystr = rhs.str();
    return (xstr<ystr);
}

// -*-----------------------------------------*-
}//-*- end::namespace::ekasoft::elx::utils   -*-
// -*-----------------------------------------*-

// -*-------------------------*-
// -*- Number Implementation -*-
// -*-------------------------*-
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
    auto num = std::atan(static_cast<f64>(*this));
    return Number(num);
}

// -*-
Number Number::atan2(const Number& rhs) const{
    auto y = static_cast<f64>(rhs);
    auto x = static_cast<f64>(*this);
    auto num = std::atan2(y, x);
    return Number(num);
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

// -*-
Number Number::log(void) const{
    return Number(std::log(static_cast<f64>(*this)));
}

// -*-
Number Number::log2(void) const{
    return Number(std::log2(static_cast<f64>(*this)));
}

// -*-
Number Number::log10(void) const{
    return Number(std::log10(static_cast<f64>(*this)));
}

// -*-
Number Number::log1p(void) const{
    return Number(std::log1p(static_cast<f64>(*this)));
}

// -*-
Number Number::erf(void) const{
    return Number(std::erf(static_cast<f64>(*this)));
}

// -*-
Number Number::erfc(void) const{
    return Number(std::erfc(static_cast<f64>(*this)));
}

// -*-
Number Number::tgamma(void) const{
    return Number(std::tgamma(static_cast<f64>(*this)));
}

// -*-
Number Number::lgamma(void) const{
    return Number(std::lgamma(static_cast<f64>(*this)));
}

// -*-
bool Number::isnan(void) const{
    return std::isnan(static_cast<f64>(*this));
}

// -*-
bool Number::isinf(void) const{
    return std::isinf(static_cast<f64>(*this));
}

// -*-
bool Number::isfinite(void) const{
    return std::isfinite(static_cast<f64>(*this));
}

Number& Number::operator!(){
    bool val{};
    this->value(val);
    this->m_value = (val ? i64(0) : i64(1));
    return *this;
}

Number& Number::operator-(){
    if(std::holds_alternative<i64>(this->m_value)){
        i64 num{};
        this->value(num);
        this->m_value = num;
    }else{
        f64 num{};
        this->value(num);
        this->m_value = num;
    }
    return *this;
}

Number& Number::operator~(){
    if(std::holds_alternative<i64>(this->m_value)){
        i64 num{};
        this->value(num);
        this->m_value = (~num);
    }else{
        std::stringstream ss;
        ss << "cannot apply '~' operator on floating-point value ";
        ss << this->str();
        throw ELixError(ELixError::SyntaxError, ss.str());
    }
    return *this;
}

Number operator+(const Number& lhs, const Number& rhs){
    if(lhs.is_integer() && rhs.is_integer()){
        auto x = static_cast<i64>(lhs);
        auto y = static_cast<i64>(rhs);
        x += y;
        return Number(x);
    }
    auto num = static_cast<f64>(lhs);
    num += static_cast<f64>(rhs);
    return Number(num);
}

Number operator-(const Number& lhs, const Number& rhs){
    if(lhs.is_integer() && rhs.is_integer()){
        auto x = static_cast<i64>(lhs);
        auto y = static_cast<i64>(rhs);
        x -= y;
        return Number(x);
    }
    auto num = static_cast<f64>(lhs);
    num -= static_cast<f64>(rhs);
    return Number(num);
}

Number operator*(const Number& lhs, const Number& rhs){
    if(lhs.is_integer() && rhs.is_integer()){
        auto x = static_cast<i64>(lhs);
        auto y = static_cast<i64>(rhs);
        x *= y;
        return Number(x);
    }
    auto num = static_cast<f64>(lhs);
    num *= static_cast<f64>(rhs);
    return Number(num);
}

Number operator/(const Number& lhs, const Number& rhs){
    if(lhs.is_integer() && rhs.is_integer()){
        auto x = static_cast<i64>(lhs);
        auto y = static_cast<i64>(rhs);
        if(y==0){
            throw ELixError(ELixError::ValueError, "division by zero");
        }
        return Number((x/y));
    }
    auto x = static_cast<f64>(lhs);
    auto y = static_cast<f64>(rhs);
    if(y==0.0){
        throw ELixError(ELixError::ValueError, "division by zero");
    }
    return Number((x/y));
}

Number operator%(const Number& lhs, const Number& rhs){
    if(lhs.is_integer() && rhs.is_integer()){
        auto x = static_cast<i64>(lhs);
        auto y = static_cast<i64>(rhs);
        if(y==0){
            throw ELixError(ELixError::ValueError, "division by zero");
        }
        return Number((x%y));
    }
    auto x = static_cast<f64>(lhs);
    auto y = static_cast<f64>(rhs);
    if(y==0.0){
        throw ELixError(ELixError::ValueError, "division by zero");
    }
    return Number(std::fmod(x, y));
}

Number operator&(const Number& lhs, const Number& rhs){
    if(lhs.is_integer() && rhs.is_integer()){
        auto x = static_cast<i64>(lhs);
        auto y = static_cast<i64>(rhs);
        return Number((x & y));
    }
    std::stringstream ss;
    ss << "incompatible type for operator `&'. Arguments types are ";
    ss << (lhs.is_integer()? "Integer" : "Float") << " and ";
    ss << (rhs.is_integer()? "Integer" : "Float");
    throw ELixError(ELixError::TypeError, ss.str());
}

Number operator|(const Number& lhs, const Number& rhs){
    if(lhs.is_integer() && rhs.is_integer()){
        auto x = static_cast<i64>(lhs);
        auto y = static_cast<i64>(rhs);
        return Number((x | y));
    }
    std::stringstream ss;
    ss << "incompatible type for operator `|'. Arguments types are ";
    ss << (lhs.is_integer()? "Integer" : "Float") << " and ";
    ss << (rhs.is_integer()? "Integer" : "Float");
    throw ELixError(ELixError::TypeError, ss.str());
}

Number operator^(const Number& lhs, const Number& rhs){
    if(lhs.is_integer() && rhs.is_integer()){
        auto x = static_cast<i64>(lhs);
        auto y = static_cast<i64>(rhs);
        return Number((x ^ y));
    }
    std::stringstream ss;
    ss << "incompatible type for operator `^'. Arguments types are ";
    ss << (lhs.is_integer()? "Integer" : "Float") << " and ";
    ss << (rhs.is_integer()? "Integer" : "Float");
    throw ELixError(ELixError::TypeError, ss.str());
}

Number operator<<(const Number& lhs, const Number& rhs){
    if(lhs.is_integer() && rhs.is_integer()){
        auto x = static_cast<i64>(lhs);
        auto y = static_cast<i64>(rhs);
        return Number((x << y));
    }
    std::stringstream ss;
    ss << "incompatible type for operator `<<'. Arguments types are ";
    ss << (lhs.is_integer()? "Integer" : "Float") << " and ";
    ss << (rhs.is_integer()? "Integer" : "Float");
    throw ELixError(ELixError::TypeError, ss.str());
}

Number operator>>(const Number& lhs, const Number& rhs){
    if(lhs.is_integer() && rhs.is_integer()){
        auto x = static_cast<i64>(lhs);
        auto y = static_cast<i64>(rhs);
        return Number((x >> y));
    }
    std::stringstream ss;
    ss << "incompatible type for operator `>>'. Arguments types are ";
    ss << (lhs.is_integer()? "Integer" : "Float") << " and ";
    ss << (rhs.is_integer()? "Integer" : "Float");
    throw ELixError(ELixError::TypeError, ss.str());
}

bool operator||(const Number& lhs, const Number& rhs){
    auto x = static_cast<bool>(lhs);
    auto y = static_cast<bool>(rhs);
    return (x || y);
}

bool operator&&(const Number& lhs, const Number& rhs){
    auto x = static_cast<bool>(lhs);
    auto y = static_cast<bool>(rhs);
    return (x && y);
}

bool operator==(const Number& lhs, const Number& rhs){
    if(lhs.is_integer() && rhs.is_integer()){
        auto x = static_cast<i64>(lhs);
        auto y = static_cast<i64>(rhs);
        return (x==y);
    }
    auto x = static_cast<f64>(lhs);
    auto y = static_cast<f64>(rhs);
    return (x==y);
}

bool operator!=(const Number& lhs, const Number& rhs){
    return !(lhs==rhs);
}

bool operator<(const Number& lhs, const Number& rhs){
    auto x = static_cast<f64>(lhs);
    auto y = static_cast<f64>(rhs);
    return (x < y);
}

bool operator>(const Number& lhs, const Number& rhs){
    return !(lhs<=rhs);
}

bool operator<=(const Number& lhs, const Number& rhs){
    return ((lhs < rhs) || (lhs == rhs));
}

bool operator>=(const Number& lhs, const Number& rhs){
    return !(lhs < rhs);
}

bool operator>=(const Number& lhs, const Number& rhs){
    return !(lhs < rhs);
}

// -*-------------------------*-
// -*- Symbol Implementation -*-
// -*-------------------------*-
std::string Symbol::str(void) const{
    return this->value;
}

std::string Symbol::repr(void) const{
    return this->str();
}

Symbol Symbol::clone(void) const{
    Symbol sym{};
    sym.value = this->value;
    return std::move(sym);
}

bool operator==(const Symbol& lhs, const Symbol& rhs){
    return (lhs.value==rhs.value);
}

bool operator!=(const Symbol& lhs, const Symbol& rhs){
    return !(lhs==rhs);
}

// -*-------------------------*-
// -*- Lambda Implementation -*-
// -*-------------------------*-
std::string Lambda::str(void) const{
    std::stringstream ss;
    if(this->named){ ss << "function at "; }
    else{ ss << "lambda  at "; }
    ss << std::hex << std::addressof(*this);
    return ss.str();
}

// -*-
std::string Lambda::repr(void) const{
    std::stringstream ss;
    if(this->named){ ss << "(fun "; }
    else{ ss << "(lambda "; }
    if(this->params.empty()){ ss << "()"; }
    else{
        ss << "(";
        for(size_t i=0; i < this->params.size(); i++){
            if(i>0){ ss << " "; }
            ss << this->params[i].str();
        }
        ss << ")";
    }
    for(const auto& expr: this->body){
        ss << expr->repr() << "\n";
    }
    ss << ")";
    return ss.str();
}

// -*-
Lambda Lambda::clone(void) const{
    Lambda result{};
    result.named = this->named;
    result.params = this->params;
    result.body = this->body;
    result.ctx = std::make_shared<Env>(*ctx);

    return std::move(result);
}

// -*-
Object Lambda::operator()(const Vec<Object>& args){
    if(this->params.size() != args.size()){
        std::stringstream ss;
        ss << "Incorrect number of arguments.\n";
        ss << "Expect " << this->params.size() << " but got " << args.size();
    }
    for(size_t i=0; i < args.size(); i++){
        auto key = this->params[i].str();
        auto val = args[i];
        this->ctx->define(key, val);
    }
    Object result{};
    auto visitor = ELix::visitor;
    auto runtime = ELix::runtime;

    auto self = std::make_unique<ELix>();
    ELix::runtime = this->ctx;
    ELix::visitor = self.get();

    for(auto&& expr: this->body){
        result = expr->eval(ELix::visitor);
    }
    ELix::visitor = visitor;
    ELix::runtime = runtime;
    return result;
}

// -*-----------------------*-
// -*- List Implementation -*-
// -*-----------------------*-
std::string List::str(void) const{
    if(this->items.empty()){
        return "()";
    }
    std::stringstream ss;
    ss << "(";
    for(size_t i=0; i < this->items.size(); i++){
        if(i > 0){ ss << " "; }
        ss << this->items[i].str();
    }
    ss << ")";
    return ss.str();
}

// -
std::string List::repr(void) const{
    if(this->items.empty()){
        return "()";
    }
    std::stringstream ss;
    ss << "(";
    for(size_t i=0; i < this->items.size(); i++){
        if(i > 0){ ss << " "; }
        ss << this->items[i].repr();
    }
    ss << ")";
    return ss.str();
}

List List::clone(void) const{
    List xs{};
    for(const auto& item: this->items){
        xs.items.push_back(Object(item));
    }

    return std::move(xs);
}

// -*-
i64 List::find(const Object& rhs, i64 from) const{
    if(from >= this->len()){
        throw ELixError(
            ELixError::IndexError, "index out or range while applying `List.find'"
        );
    }
    if(from==-1){
        for(i64 i=(this->len()-1); i >= 0; i--){
            if(this->items[i]==rhs){ return i; }
        }
    }else{
        if(from < 0){
            std::stringstream ss;
            ss << "Invalid value of the second argument to `List.find'.\n";
            ss << "Expect the value to be either -1 or an appropriate positive value";
            throw ELixError(ELixError::IndexError, ss.str());
        }
        for(i64 i=from; i < this->items.size(); i++){
            if(this->items[i]==rhs){ return i; }
        }
    }
    return -1;
}

// -*-
List& List::reverse(void){
    std::reverse(this->items.begin(), this->items.end());
    return *this;
}

// -*-
List& List::concat(const List& rhs){
    for(const auto& arg: rhs.items){
        this->items.emplace_back(arg);
    }
    return *this;
}

i64 List::len(void) const{
    return static_cast<i64>(this->items.size());
}

// -*-
Object List::head(void) const{
    if(this->items.empty()){
        throw ELixError(
            ELixError::ValueError,
            "Cannot apply `List.head' on an empty list."
        );
    }
    return Object(this->items[0]);
}

// -*-
List List::tail(void) const{
    List xs;
    xs.items = {};
    for(auto ptr=this->items.begin()+1; ptr != this->items.end(); ptr++){
        auto obj = *ptr;
        xs.items.emplace_back(obj);
    }
    return std::move(xs);
}

// -*-
Object List::first(void) const{
    if(this->items.empty()){
        throw ELixError(
            ELixError::ValueError, "Cannot apply `List.first' on an empty list"
        );
    }
    return Object(this->items[0]);
}

Object List::last(void) const{
    if(this->items.empty()){
        throw ELixError(
            ELixError::ValueError, "Cannot apply `List.last' on an empty list"
        );
    }
    auto idx = this->items.size() - 1;
    return Object(this->items[idx]);
}

List& List::push(const Object& arg){
    this->items.emplace_back(arg);
    return *this;
}

List& List::pop(void){
    if(this->items.empty()){
        throw ELixError(
            ELixError::ValueError, "Cannot apply `List.pop' on an empty list"
        );
    }
    this->items.pop_back();
    return *this;
}

List& List::insert(i64 idx, const Object& obj){
    bool check = (idx < 0 || idx >= this->items.size());
    if(check){
        throw ELixError(
            ELixError::IndexError, "index out or range while applying `List.insert'"
        );
    }
    auto ptr = this->items.begin() + idx;
    this->items.insert(ptr, obj);
    return *this;
}

// -*-
Object List::remove(i64 idx){
    bool check = (idx < 0 || idx >= this->items.size());
    if(check){
        throw ELixError(
            ELixError::IndexError, "index out or range while applying `List.remove'"
        );
    }
    auto ptr = this->items.begin() + idx;
    this->items.erase(ptr);
    return *this;
}

// -*-
List& List::clear(void){
    this->items.clear();
    return *this;
}

// -*------------------------*-
// -*- Array Implementation -*-
// -*------------------------*-
std::string Array::str(void) const{
    std::stringstream ss;
    if(this->items.empty()){ ss << "[]"; }
    else{
        ss << "[";
        for(size_t i=0; i < this->items.size(); i++){
            if(i > 0){ ss << " "; }
            ss << this->items[i].str();
        }
        ss << "]";
    }
    return ss.str();
}

// -
std::string Array::repr(void) const{
    std::stringstream ss;
    if(this->items.empty()){ ss << "[]"; }
    else{
        ss << "[";
        for(size_t i=0; i < this->items.size(); i++){
            if(i > 0){ ss << " "; }
            ss << this->items[i].repr();
        }
        ss << "]";
    }
    return ss.str();
}

// -*-
Array Array::clone(void) const{
    Array xs{};
    for(const auto& item: this->items){
        xs.items.emplace_back(item);
    }
    return std::move(xs);
}

// -*-
i64 Array::find(const Object& rhs, i64 from){
    if(from >= this->len()){
        throw ELixError(
            ELixError::IndexError, "index out or range while applying 'Array.find'"
        );
    }
    if(this->len()==0){ return -1; }
    if(from==-1){
        for(i64 i=(this->len()-1); i >= 0; i--){
            if(this->items[i]==rhs){ return i; }
        }
    }else{
        if(from < 0){
            std::stringstream ss;
            ss << "Invalid value of the second argument to `Array.find'.\n";
            ss << "Expect the value to be either -1 or an appropriate positive value";
            throw ELixError(ELixError::IndexError, ss.str());
        }
        for(i64 i=from; i < this->items.size(); i++){
            if(this->items[i]==rhs){ return i; }
        }
    }
    return -1;
}

// -*-
Array& Array::reverse(void){
    std::reverse(this->items.begin(), this->items.end());
    return *this;
}

// -*-
Array& Array::concat(const Array& array){
    for(const auto& item: array.items){
        this->items.emplace_back(item);
    }
    return *this;
}

// -*-
i64 Array::len(void) const{
    return static_cast<i64>(this->items.size());
}

// -*-
Array Array::slice(i64 i, i64 j) const{
    auto checkIndex = [this](i32 idx){
        if(idx < 0 || idx > this->items.size()){
            throw ELixError(
                ELixError::IndexError, "Index out of range while apply `Array.slice'"
            );
        }
    };
    if(i > j){
        throw ELixError(
            ELixError::SyntaxError,
            "Incorrect arguments order found while applying `Array.slice'"
        );
    }
    checkIndex(i);
    checkIndex(j);
    Array array{};
    array.items = {};
    if(i==j){ return std::move(array); }
    for(auto idx=i; idx < j; idx++){
        array.items.emplace_back(this->items[idx]);
    }
    return std::move(array);
}

// -*-
Array& Array::insert(i64 idx,  const Object& arg){
    bool check = (idx < 0 || idx >= this->items.size());
    if(check){
        throw ELixError(
            ELixError::IndexError, "index out or range while applying `Array.insert'"
        );
    }
    auto ptr = this->items.begin() + idx;
    this->items.insert(ptr, arg);
    return *this;    
}

Object Array::get(i64 idx) const{
    bool check = (idx < 0 || idx >= this->items.size());
    if(check){
        throw ELixError(
            ELixError::IndexError, "index out or range while applying `Array.get'"
        );
    }
    return Object(this->items[idx]);
}

Array& Array::set(i64 idx, const Object& arg){
    bool check = (idx < 0 || idx >= this->items.size());
    if(check){
        throw ELixError(
            ELixError::IndexError, "index out or range while applying `Array.set'"
        );
    }
    this->items[idx] = Object(arg);
    return *this;
}

Array& Array::splice(i64 idx, const Array& rhs){
    bool check = (idx < 0 || idx >= this->items.size());
    if(check){
        throw ELixError(
            ELixError::IndexError, "index out or range while applying `Array.splice'"
        );
    }
    auto ptr = this->items.begin() + idx;
    this->items.insert(ptr, rhs.items.cbegin(), rhs.items.cend());
    return *this;
}

// -*-
bool Array::any(const Object& predicate) const{
    auto check = (
        predicate.is_func() ||
        predicate.is_lambda() ||
        predicate.is_function()
    );
    if(!check){
        std::stringstream ss;
        ss << "Incorrect argument type to `Array.any'. Expect a callable object ";
        ss << "but got " << std::quoted(predicate.type().str());
        throw ELixError(ELixError::TypeError, ss.str());
    }
    bool ans{false};
    if(predicate.is_lambda() || predicate.is_function()){
        auto fun = predicate.as_lambda();
        for(const auto& arg: this->items){
            auto rv = fun(Vec<Object>{arg});
            if(!rv.is_bool()){
                std::stringstream ss;
                ss << "Incorrect argument type to `Array.any'. Expect a predicate";
                throw ELixError(ELixError::TypeError, ss.str());
            }
            if(rv.as_bool()){
                ans = true;
                break;
            }
        }
    }else{
        auto fun = predicate.as_func();
        for(const auto& arg: this->items){
            auto rv = fun(Vec<Object>{arg});
            if(!rv.is_bool()){
                std::stringstream ss;
                ss << "Incorrect argument type to `Array.any'. Expect a predicate";
                throw ELixError(ELixError::TypeError, ss.str());
            }
            if(rv.as_bool()){
                ans = true;
                break;
            }
        }
    }

    return ans;
}

// -*-
bool Array::all(const Object& predicate) const{
    auto check = (
        predicate.is_func() ||
        predicate.is_lambda() ||
        predicate.is_function()
    );
    if(!check){
        std::stringstream ss;
        ss << "Incorrect argument type to `Array.all'. Expect a callable object ";
        ss << "but got " << std::quoted(predicate.type().str());
        throw ELixError(ELixError::TypeError, ss.str());
    }
    bool ans{true};
    if(predicate.is_lambda() || predicate.is_function()){
        auto fun = predicate.as_lambda();
        for(const auto& arg: this->items){
            auto rv = fun(Vec<Object>{arg});
            if(!rv.is_bool()){
                std::stringstream ss;
                ss << "Incorrect argument type to `Array.all'. Expect a predicate";
                throw ELixError(ELixError::TypeError, ss.str());
            }
            if(!rv.as_bool()){
                ans = false;
                break;
            }
        }
    }else{
        auto fun = predicate.as_func();
        for(const auto& arg: this->items){
            auto rv = fun(Vec<Object>{arg});
            if(!rv.is_bool()){
                std::stringstream ss;
                ss << "Incorrect argument type to `Array.all'. Expect a predicate";
                throw ELixError(ELixError::TypeError, ss.str());
            }
            if(!rv.as_bool()){
                ans = false;
                break;
            }
        }
    }

    return ans;
}

// -*-
Object Array::reduce(const Object& predicate, const Object& initVal) const{
    auto check = (
        predicate.is_func() ||
        predicate.is_lambda() ||
        predicate.is_function()
    );
    if(!check){
        std::stringstream ss;
        ss << "Incorrect argument type to `Array.reduce'. Expect a callable object ";
        ss << "but got " << std::quoted(predicate.type().str());
        throw ELixError(ELixError::TypeError, ss.str());
    }
    Object acc{initVal};
    if(this->items.empty()){
        return Object(acc);
    }
    if(predicate.is_func()){
        auto func = predicate.as_func();
        for(const auto& val: this->items){
            acc = func(Vec<Object>{acc, val});
        }
    }else{
        auto func = predicate.as_lambda();
        for(const auto& val: this->items){
            acc = func(Vec<Object>{acc, val});
        }
    }

    return Object(acc);
}

// -*-
Array& Array::sort(const Object& predicate){
    if(predicate.is_nil()){
        std::sort(
            this->items.begin(), this->items.end(),
            [](const Object& x, const Object& y){ return (x < y);}
        );
        return *this;
    }
    if(this->len() < 2){
        return *this;
    }
    auto check = (
        predicate.is_func() ||
        predicate.is_lambda() ||
        predicate.is_function()
    );
    if(!check){
        std::stringstream ss;
        ss << "Incorrect argument type to `Array.sort'. Expect a callable object ";
        ss << "but got " << std::quoted(predicate.type().str());
        throw ELixError(ELixError::TypeError, ss.str());
    }
    if(predicate.is_func()){
        auto func = predicate.as_func();
        // check func is a predicate
        auto check = func(Vec<Object>(this->items.cbegin(), this->items.cbegin()+2));
        if(!check.is_bool()){
            std::stringstream ss;
            ss << "Incorrect argument type to `Array.sort'. Expect a predicate.";
            throw ELixError(ELixError::TypeError, ss.str());
        }
        std::sort(
            this->items.begin(), this->items.end(),
            [&func](const Object& x, const Object& y){
                return func(Vec<Object>{x, y});
            }
        );
        return *this;
    }else{
        auto func = predicate.as_lambda();
        // check func is a predicate
        auto check = func(Vec<Object>(this->items.cbegin(), this->items.cbegin()+2));
        if(!check.is_bool()){
            std::stringstream ss;
            ss << "Incorrect argument type to `Array.sort'. Expect a predicate.";
            throw ELixError(ELixError::TypeError, ss.str());
        }
        std::sort(
            this->items.begin(), this->items.end(),
            [&func](const Object& x, const Object& y){
                return func(Vec<Object>{x, y});
            }
        );
        return *this;
    }
}

// -*-
Array& Array::clear(void){
    this->items.clear();
    return *this;
}

// -*-
Array& Array::push(const Object& rhs){
    this->items.emplace_back(Object(rhs));
    return *this;
}

// -*-
Array& Array::pop(void){
    if(this->items.empty()){
        std::stringstream ss;
        ss << "Cannot apply `Array.pop' to an empty array.";
        throw ELixError(ELixError::ValueError, ss.str());
    }
    this->items.pop_back();
    return *this;
}

// -*-------------------------*-
// -*- String Implementation -*-
// -*-------------------------*-
std::string String::str(void) const{
    return this->text;
}

std::string String::repr(void) const{
    std::stringstream ss;
    ss << std::quoted(this->str());
    return ss.str();
}

String String::clone(void) const{
    String result{};
    result.text = std::string(this->text);
    return std::move(result);
}

// -*-
i64 String::find(const String& needle, i64 from) const{
    if(from >= this->len()){
        throw ELixError(
            ELixError::IndexError, "index out or range while applying 'String.find'"
        );
    }
    if(from==-1){
        auto pos = this->text.find_last_of(needle.text);
        if(pos==std::string::npos){ return -1; }
    }else{
        if(from < 0){
            std::stringstream ss;
            ss << "Invalid value of the second argument to `String.find'. ";
            ss << "Expect the value to be either -1 or an appropriate positive value";
            throw ELixError(ELixError::IndexError, ss.str());
        }
        auto pos = this->text.find(needle.text, from);
        if(pos==std::string::npos){ return -1; }
        return static_cast<i64>(pos);
    }
}

// -*-
String& String::reverse(void){
    std::reverse(this->text.begin(), this->text.end());
    return *this;
}

// -*-
String& String::concat(const String& arg){
    this->text += std::string(arg.text);
    return *this;
}

// -*-
i64 String::len(void) const{
    return static_cast<i64>(this->text.length());
}

// -*-
String operator+(const String& lhs, const String& rhs){
    String str{};
    str.text = (lhs.text + rhs.text);
    return std::move(str);
}

// -*-
bool operator==(const String& lhs, const String& rhs){
    return (lhs.text==rhs.text);
}

bool operator!=(const String& lhs, const String& rhs){
    return !(lhs==rhs);
}

bool operator<=(const String& lhs, const String& rhs){
    return ((lhs < rhs) || (lhs == rhs));
}

bool operator>=(const String& lhs, const String& rhs){
    return !(lhs <= rhs);
}

bool operator<(const String& lhs, const String& rhs){
    return !(lhs >= rhs);
}

bool operator>(const String& lhs, const String& rhs){
    return !(lhs <= rhs);
}

Vec<String> String::split(const String delim){
    Vec<String> result{};
    size_t pos{};
    while(true){
        auto idx = this->text.find(delim.text);
        if(idx == std::string::npos){ break; }
        if(idx != pos){
            String self{};
            self.text = this->text.substr(pos, idx - pos);
            pos = idx + delim.text.length();
            result.push_back(std::move(self));
        }
    }
    if(this->text.length() > pos){
        String self{};
        self.text = this->text.substr(pos);
        result.push_back(std::move(self));
    }
    return std::move(result);
}

String String::substr(i64 i, i64 j) const{
    auto checkIndex = [this](i32 idx){
        if(idx < 0 || idx > this->text.length()){
            throw ELixError(
                ELixError::IndexError, "Index out of range while apply `String.substr'"
            );
        }
    };
    if(i > j){
        throw ELixError(
            ELixError::SyntaxError,
            "Incorrect arguments order found while applying `String.substr'"
        );
    }
    checkIndex(i);
    checkIndex(j);
    String result{};
    auto txt = this->text.substr(i, j);
    result.text = std::string(txt);
    return std::move(result);
}

bool String::startswith(const String& rhs) const{
    if(this->len() < rhs.len()){ return false; }
    for(auto i=0; i < rhs.len(); i++){
        if(this->text[i]!=rhs.text[i]){ return false; }
    }
    return true;
}

bool String::endswith(const String& rhs) const{
    if(this->len() > rhs.len()){ return false; }
    auto start = this->len() - rhs.len();
    for(auto i=0; i < rhs.len(); i++){
        if(this->text[start+i]!=rhs.text[i]){ return false; }
    }
    return true;
}

String& String::ltrim(void){
    this->text.erase(
        this->text.begin(),
        std::find_if(
            this->text.begin(), this->text.end(),
            [](unsigned char c){ return (c!=' ');}
        )
    );
    return *this;
}

String& String::rtrim(void){
    this->text.erase(
        std::find_if(
            this->text.rbegin(), this->text.rend(),
            [](unsigned char c){ return (c!=' '); }
        ).base(),
        this->text.end()
    );
    return *this;
}

String& String::trim(void){
    this->ltrim().rtrim();
    return *this;
}

String& String::lower(void){
    std::transform(
        this->text.cbegin(), this->text.cend(),
        this->text.begin(),
        [](unsigned char c){ return std::tolower(c);}
    );
    return *this;
}

String& String::upper(void){
    std::transform(
        this->text.cbegin(), this->text.cend(),
        this->text.begin(),
        [](unsigned char c){ return std::toupper(c);}
    );
    return *this;
}

String& String::capitalize(void){
    for(auto& c: this->text){
        if(std::isalpha(c)){
            c = std::toupper(c);
            break;
        }
    }
    return *this;
}

String& String::title(void){
    this->capitalize();
    bool space{false};
    for(auto& c: this->text){
        if(std::isspace(c)){
            space = true;
            continue;
        }
        if(space==true && std::isalpha(c)){
            c = std::toupper(c);
            space = false;
            continue;
        }
    }
    return *this;
}

Vec<i64> String::find_all(const String& needle){
    if(this->len() < needle.len()){ return Vec<i64>{}; }
    auto sandbox = this->text;
    Vec<i64> result{};
    i64 idx = 0;
    while(true){
        auto pos = sandbox.find(needle.text, idx);
        if(pos==std::string::npos){ break; }
        result.push_back(static_cast<i64>(pos));
        idx = pos + needle.len();
    }
    return std::move(result);
}

String& String::replace(const String& old, const String& neo){
    auto pos = this->text.find(old.text);
    if(pos==std::string::npos){ return *this; }
    this->text.replace(pos, old.len(), neo.text);
    return *this;
}

String& String::replace_all(const String& old, const String& neo){
    std::replace(
        this->text.begin(), this->text.end(),
        old.text, neo.text
    );

    return *this;
}


i64 String::ord(void) const{
    if(this->len() != 1){
        std::stringstream ss;
        ss << "`ord` is only applicable on string with length==1";
        throw ELixError(ELixError::ValueError, ss.str());
    }
    auto c = this->text[0];
    return static_cast<i64>(c);
}

String String::at(i64 idx) const{
    if(idx < 0 || idx >= this->len()){
        std::stringstream ss;
        ss << "Invalid argument value found while applying `String.at'.";
        throw ELixError(ELixError::IndexError, ss.str());
    }
    String result{};
    result.text = std::string(1, this->text[idx]);
    return std::move(result);
}

// -*-----------------------*-
// -*- Pair Implementation -*-
// -*-----------------------*-
std::string Pair::str(void) const{
    std::stringstream ss;
    ss << "#(" << this->key.str() << " " << this->val.str() << "')";
    return ss.str();
}

std::string Pair::repr(void) const{
    std::stringstream ss;
    ss << "#(" << this->key.repr() << " " << this->val.repr() << "')";
    return ss.str();
}

Pair Pair::clone(void) const{
    Pair pair{};
    pair.key = Object(this->key);
    pair.val = Object(this->val);

    return std::move(pair);
}

// -*-----------------------*-
// -*- Dict Implementation -*-
// -*-----------------------*-
std::string Dict::str(void) const{
    if(this->len()==0){ return "{}"; }

    std::stringstream ss;
    ss << "{\n";
    for(const auto& [key, val]: this->hmap){
        Pair pair{};
        pair.key = Object(key);
        pair.val = Object(val);
        ss << pair.str() << " ";
    }
    ss << "\n}";
    return ss.str();
}

std::string Dict::repr(void) const{
    if(this->len()==0){ return "{}"; }

    std::stringstream ss;
    ss << "{\n";
    for(const auto& [key, val]: this->hmap){
        Pair pair{};
        pair.key = Object(key);
        pair.val = Object(val);
        ss << pair.repr() << " ";
    }
    ss << "\n}";
    return ss.str();
}

Dict Dict::clone(void) const{
    Dict dict{};
    dict.hmap = HashMap{this->hmap};
    return std::move(dict);
}

bool Dict::find(const Object& key) const{
    if(!key.is_hashable()){
        std::stringstream ss;
        ss << "key " << std::quoted(key.str()) << " is not hashable";
        throw ELixError(ELixError::KeyError, ss.str());
    }
    auto entry = this->hmap.find(key);
    if(entry == this->hmap.end()){
        return false;
    }
    return true;
}

Dict& Dict::concat(const Dict& dict){
    for(const auto& [key, val]: dict.hmap){
        this->hmap[key] = val;
    }
    return *this;
}

i64 Dict::len(void) const{
    return static_cast<i64>(this->hmap.size());
}

Object Dict::get(const Object& key) const{
    if(this->find(key)){
        auto entry = this->hmap.find(key);
        return Object(entry->second);
    }
    std::stringstream ss;
    ss << "key " << std::quoted(key.str()) << " not found.";
    throw ELixError(ELixError::KeyError, ss.str());
}

void Dict::put(const Object& key, const Object& val){
    this->hmap[key] = val;
}

Dict& Dict::set(const Object& key, const Object& val){
    if(!key.is_hashable()){
        std::stringstream ss;
        ss << "key " << std::quoted(key.str()) << " is not hashable";
        throw ELixError(ELixError::KeyError, ss.str());
    }
    this->hmap[key] = val;
    return *this;
}

Dict& Dict::update(const Object& key, const Object& val){
    if(this->find(key)){
        this->hmap[key] = val;
    }
    std::stringstream ss;
    ss << "Cannot update undefined key " << std::quoted(key.str());
    throw ELixError(ELixError::KeyError, ss.str());
}

Vec<Object> Dict::keys(void) const{
    Vec<Object> result{};
    for(const auto& [key, _]: this->hmap){
        result.push_back(Object(key));
    }
    return std::move(result);
}

Vec<Object> Dict::values(void) const{
    Vec<Object> result{};
    for(const auto& [_, val]: this->hmap){
        result.push_back(Object(val));
    }
    return std::move(result);
}

Pair Dict::popitem(const Object& key){
    if(this->find(key)){
        Pair pair{};
        auto entry = this->hmap.find(key);
        pair.key = Object(entry->first);
        pair.val = Object(entry->second);
        this->hmap.erase(entry);
        return std::move(pair);
    }
    std::stringstream ss;
    ss << "Cannot pop item with undefined key " << std::quoted(key.str());
    throw ELixError(ELixError::KeyError, ss.str());
}

Dict& Dict::clear(void){
    this->hmap.clear();
    return *this;
}

Dict& Dict::remove(const Object& key){
    if(this->find(key)){
        auto entry = this->hmap.find(key);
        this->hmap.erase(key);
    }
    return *this;
}

Vec<Pair> Dict::items(void) const {
    Vec<Pair> result{};
    for(const auto& [key, val]: this->hmap){
        Pair pair{};
        pair.key = Object(key);
        pair.val = Object(val);
        result.push_back(std::move(pair));
    };
}

// -*----------------------*-
// -*- Set Implementation -*-
// -*----------------------*-
std::string Set::str(void) const{
    if(this->len()==0){ return "#{}"; }
    std::stringstream ss;
    ss << "#{\n";
    for(const auto& key: this->hset){
        ss << key.str() << " ";
    }
    ss << "\n}";
    return ss.str();
}

std::string Set::repr(void) const{
    if(this->len()==0){ return "#{}"; }
    std::stringstream ss;
    ss << "#{\n";
    for(const auto& key: this->hset){
        ss << key.repr() << " ";
    }
    ss << "\n}";
    return ss.str();
}

Set Set::clone(void) const{
    Set result{};
    result.hset = HashSet{this->hset};
    return std::move(result);
}

bool Set::find(const Object& needle) const{
    auto entry = this->hset.find(needle);
    if(entry == this->hset.end()){
        return false;
    }
    return true;
}

Set& Set::concat(const Set& args){
    this->hset.insert(args.hset.begin(), args.hset.end());
    return *this;
}

i64 Set::len(void) const{
    return static_cast<i64>(this->hset.size());
}

Set& Set::add(const Object& key){
    this->hset.insert(key);
    return *this;
}

Set Set::set_union(const Set& rhs) const{
    auto result = this->clone();
    result.concat(rhs);
    return std::move(result);
}

Set Set::intersection(const Set& rhs) const{
    Set result{};
    result.hset = {};
    std::set_intersection(
        this->hset.begin(), this->hset.end(),
        rhs.hset.begin(), rhs.hset.end(),
        std::back_inserter(result.hset)
    );

    return std::move(result);
}

Set Set::symmetric_difference(const Set& rhs) const{
    Set result{};
    result.hset = {};
    std::set_symmetric_difference(
        this->hset.begin(), this->hset.end(),
        rhs.hset.begin(), rhs.hset.end(),
        std::back_inserter(result.hset)
    );

    return std::move(result);
}

Set Set::difference(const Set& rhs) const{
    Set result{};
    result.hset = {};
    std::set_difference(
        this->hset.begin(), this->hset.end(),
        rhs.hset.begin(), rhs.hset.end(),
        std::back_inserter(result.hset),
        [](const Object& x, const Object& y){
            return utils::Less()(x, y);
        }
    );

    return std::move(result);
}

Set& Set::clear(void){
    this->hset.clear();
    return *this;
}

Set& Set::remove(const Object& key){
    if(this->find(key)){
        auto entry = this->hset.find(key);
        this->hset.erase(entry);
    }
    return *this;
}

Object Set::pop(void){
    if(this->len()==0){
        std::stringstream ss;
        ss << "Cannot apply `Set.pop' on empty set.";
        throw ELixError(ELixError::ValueError, ss.str());
    }
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<i64> dist(0, this->len());
    auto idx = dist(rng);
    idx = idx==this->len() ? (idx-1) : idx;
    Vec<Object> vec(this->hset.begin(), this->hset.end());
    auto key = vec[idx];
    auto entry = this->hset.find(key);
    this->hset.erase(entry);

    return Object(key);
}

bool Set::isdisjoint(const Set& rhs) const{
    for(const auto& key: rhs.hset){
        if(this->find(key)){ return true; }
    }

    return false;
}

bool Set::issubset(const Set& rhs) const{
    if(rhs.len() < this->len()){ return false;}
    for(const auto& key: this->hset){
        if(!rhs.find(key)){ return false; }
    }
    return true;
}

bool Set::issuperset(const Set& rhs) const{
    if(rhs.len() < this->len()){ return false; }

    return rhs.issubset(*this);
}

// -*-----------------------*-
// -*- Func Implementation -*-
// -*-----------------------*-
std::string Func::str(void) const{
    std::stringstream ss;
    ss << "<Builtin function at " << std::hex << std::addressof(*this) << ">";
    return ss.str();
}

std::string Func::repr(void) const{
    return this->str();
}

Func Func::clone(void) const{
    Func func{};
    func.fn = this->fn;
    func.minArgc = this->minArgc;
    func.maxArgc = this->maxArgc;
    return func;
}

Object Func::operator()(const Vec<Object>& args){
    if(this->minArgc==-1 && this->maxArgc==-1){ // variadic case
        return this->fn(args);
    }
    auto argc = args.size();
    if(argc < this->minArgc || argc > this->maxArgc){
        std::stringstream ss;
        ss << "Invalid number of arguments. Expect at least " << this->minArgc;
        ss << " and at most " << this->maxArgc << " arguments but got " << argc;
        throw ELixError(ELixError::RuntimeError, ss.str());
    }

    return this->fn(args);
}

// -*------------------------*-
// -*- Macro Implementation -*-
// -*------------------------*-
std::string Macro::str(void) const{
    std::stringstream ss;
    ss << "<macro " << this->name.str() << " at ";
    ss << std::hex << std::addressof(*this) << ")";
    return ss.str();
}

std::string Macro::repr(void) const{
    std::stringstream ss;
    ss << "(macro " << this->name.str() << "(";
    for(auto i=0; i < params.size(); i++){
        if(i > 0){ ss << " "; }
        ss << params[i].str();
    }
    ss << ")\n";
    for(auto& expr: body){
        ss << " " << expr->repr() << "\n";
    }
    ss << "\n)";
    return ss.str();
}

Macro Macro::clone(void) const{
    Macro macro{};
    macro.name.value = this->name.value;
    macro.params = Vec<Symbol>{this->params};
    macro.ctx = std::make_shared<Env>(*this->ctx);

    return std::move(macro);
}

// -*-
Expression Macro::expand(const Vec<Object>& args,  ELix& elix){
    if(this->params.size()!=args.size()){
        std::stringstream ss;
        ss << "Invalid number or arguments encountered while expanding ";
        ss << "macro " << std::quoted(this->name.str()) << "\n";
        ss << "Expecting " << this->params.size() << " arguments but got ";
        ss << args.size();
        throw ELixError(ELixError::RuntimeError, ss.str());
    }
    Context ctx_ = this->ctx;
    this->ctx = std::make_shared<Env>(ctx_);
    for(auto i=0; i < this->params.size(); i++){
        auto key = this->params[i].str();
        this->ctx->define(key, args[i]);
    }

    std::map<std::type_index, std::string> typesmap;
    typesmap[std::type_index(typeid(ListExpr))] = "ListExpr";
    typesmap[std::type_index(typeid(ArrayExpr))] = "ArrayExpr";
    typesmap[std::type_index(typeid(DictExpr))] = "DictExpr";
    typesmap[std::type_index(typeid(SetExpr))] = "SetExpr";
    typesmap[std::type_index(typeid(LiteralExpr))] = "LiteralExpr";
    typesmap[std::type_index(typeid(SymbolExpr))] = "SymbolExpr";
    typesmap[std::type_index(typeid(PairExpr))] = "PairExpr";

    decltype(this->body) expanded{};
    auto argc = body.size();
    for(auto i=0; i < argc; i++){
        auto& expr = this->body[i];
        if(typesmap[std::type_index(typeid(expr))]=="SymbolExpr"){
            auto self = *dynamic_cast<SymbolExpr*>(expr.get());
            auto sym = self.name.str();
            if(ELix::is_reserved_word(sym)){
                if(sym=="quote"){
                    if(i+1 >= argc){
                        std::stringstream ss;
                        ss << "Malformed quote expression. Expect an argument but ";
                        ss << "got none.";
                        throw ELixError(ELixError::SyntaxError, ss.str());
                    }else{
                        auto& arg = this->body[(i+1)];
                        auto xs = Vec<Expression>{};
                        xs.push_back(std::move(this->body[(i+1)]));
                        i += 1;
                        expanded.push_back(std::move(elix.handle_quote(xs)));
                        continue;
                    }
                }
                else if(sym=="unquote"){
                    if(i+1 >= argc){
                        std::stringstream ss;
                        ss << "Malformed unquote expression. Expect an argument but ";
                        ss << "got none.";
                        throw ELixError(ELixError::SyntaxError, ss.str());
                    }else{
                        auto& arg = this->body[(i+1)];
                        auto xs = Vec<Expression>{};
                        xs.push_back(std::move(this->body[(i+1)]));
                        i += 1;
                        expanded.push_back(std::move(elix.handle_unquote(xs)));
                        continue;
                    }
                }
                else if(sym=="quasiquote"){
                    if(i+1 >= argc){
                        std::stringstream ss;
                        ss << "Malformed quasiquote expression. Expect an argument but ";
                        ss << "got none.";
                        throw ELixError(ELixError::SyntaxError, ss.str());
                    }else{
                        auto& arg = this->body[(i+1)];
                        auto xs = Vec<Expression>{};
                        xs.push_back(std::move(this->body[(i+1)]));
                        i += 1;
                        expanded.push_back(std::move(elix.handle_quasiquote(xs)));
                        continue;
                    }
                }
                else if(sym=="unquote-splicing"){
                    if(i+1 >= argc){
                        std::stringstream ss;
                        ss << "Malformed unquote-splicing expression. Expect an argument but ";
                        ss << "got none.";
                        throw ELixError(ELixError::SyntaxError, ss.str());
                    }else{
                        auto& arg = this->body[(i+1)];
                        auto xs = Vec<Expression>{};
                        xs.push_back(std::move(this->body[(i+1)]));
                        i += 1;
                        expanded.push_back(std::move(elix.handle_unquote_splicing(xs)));
                        continue;
                    }
                }else if(sym=="macro"){
                    std::stringstream ss;
                    ss << "Cannot define a macro inside a macro body. Expect an argument but ";
                    throw ELixError(ELixError::SyntaxError, ss.str());
                }
            }
        }else{
            expanded.push_back(std::move(expr));
        }
    }

    auto result = std::make_unique<ListExpr>();
    result->items = std::move(expanded);
    this->ctx = ctx_;
    return std::move(result);
}

// -*-
Object Macro::operator()(const Vec<Object>& args, ELix& elix){
    auto expaned = this->expand(args, elix);
    return expaned->eval(&elix);
}

// -*------------------------------*-
// -*- Core Object Implementation -*-
// -*------------------------------*-
Object::Object()
: m_type{Symbol{"Nil"}}
, m_value{Nil{}}
{}

// -*-
Object::Object(bool b)
: m_type{Symbol{"Bool"}}
, m_value{b}
{}

// -*-
Object::Object(i64 num)
: m_type{Symbol{"Integer"}}
, m_value{Number{num}}
{}

// -*-
Object::Object(f64 num)
: m_type{Symbol{"Float"}}
, m_value{Number{num}}
{}

// -*-
Object::Object(const Number& num)
: m_type{Symbol{"Float"}}
, m_value{num}
{
    if(num.is_integer()){
        this->m_type = Symbol{"Integer"};
    }
}

// -*-
Object::Object(const Symbol& sym)
: m_type{Symbol{"Symbol"}}
, m_value{sym}
{}

// -*-
Object::Object(const char* cstr)
: m_type{Symbol{"String"}}
{
    String mstr{};
    mstr.text = std::string(cstr);
    this->m_value = std::move(mstr);
}

// -*-
Object::Object(const std::string& str)
: m_type{Symbol{"String"}}
{
    String mstr{};
    mstr.text = str;
    this->m_value = std::move(mstr);
}

Object::Object(const String& str)
: m_type{Symbol{"String"}}
, m_value{str}
{}

// -*-
Object::Object(const Array& array)
: m_type{Symbol{"Array"}}
, m_value{array}
{}

// -*-
Object::Object(const List& xs)
: m_type{Symbol{"List"}}
, m_value{xs}
{}

// -*-
Object::Object(const Dict& mdict)
: m_type{Symbol{"Dict"}}
, m_value{mdict}
{}

// -*-
Object::Object(const Set& mset)
: m_type{Symbol{"Set"}}
, m_value{mset}
{}

// -*-
Object::Object(const Pair& mpair)
: m_type{Symbol{"Pair"}}
, m_value{mpair}
{}

// -*-
Object::Object(const Lambda& lambda)
: m_type{Symbol{"Lambda"}}
, m_value{lambda}
{
    if(lambda.named){
        this->m_type = Symbol{"Function"};
    }
}

// -*-
Object::Object(const Macro& macro)
: m_type{Symbol{"Macro"}}
, m_value{macro}
{}

// -*-
Object::Object(const Func& func)
: m_type{"Func"}
, m_value{func}
{}

// -*-
Object::Object(const Object& obj) noexcept
: m_type{obj.m_type}
, m_value{obj.m_value}
{}

// -*-
Object::Object(Object&& obj) noexcept
: m_type{std::move(obj.m_type)}
, m_value{std::move(obj.m_value)}
{}

// -*-
Object& Object::operator=(const Object& obj) noexcept{
    if(this != &obj){
        this->m_type = obj.m_type;
        this->m_value = obj.m_value;
    }
    return *this;
}

// -*-
Object& Object::operator=(Object&& obj) noexcept{
    if(this != &obj){
        this->m_type = std::move(obj.m_type);
        this->m_value = std::move(obj.m_value);
    }
    return *this;
}

// -*-
Object::~Object(){}

// -*-
bool Object::is_nil(void) const{
    return this->m_type==Symbol{"Nil"};
}

// -*-
bool Object::is_bool(void) const{
    return this->m_type==Symbol{"Bool"};
}

// -*-
bool Object::is_integer(void) const{
    return this->m_type==Symbol{"Integer"};
}

// -*-
bool Object::is_float(void) const{
    return this->m_type==Symbol{"Float"};
}

// -*-
bool Object::is_number(void) const{
    return (this->is_integer() || this->is_float());
}

// -*-
bool Object::is_symbol(void) const{
    return this->m_type==Symbol{"Symbol"};
}

// -*-
bool Object::is_string(void) const{
    return this->m_type==Symbol{"String"};
}

// -*-
bool Object::is_array(void) const{
    return this->m_type==Symbol{"Array"};
}

// -*-
bool Object::is_list(void) const{
    return this->m_type==Symbol{"List"};
}

// -*-
bool Object::is_dict(void) const{
    return this->m_type==Symbol{"Dict"};
}

// -*-
bool Object::is_set(void) const{
    return this->m_type==Symbol{"Set"};
}

// -*-
bool Object::is_macro(void) const{
    return this->m_type==Symbol{"Macro"};
}

bool Object::is_lambda(void) const{
    return this->m_type==Symbol{"Lambda"};
}

// -*-
bool Object::is_function(void) const{
    return this->m_type==Symbol{"Function"};
}

// -*-
bool Object::is_func(void) const{
    return this->m_type==Symbol{"Func"};
}

// -*-
bool Object::is_callable(void) const{
    return (
        this->is_lambda() ||
        this->is_function() ||
        this->is_func() ||
        this->is_macro()
    );
}

// -*-
bool Object::is_pair(void) const{
    return this->m_type==Symbol{"Pair"};
}

// -*-
bool Object::as_bool(void) const{
    bool result{};
    if(this->is_bool()){
        result = std::any_cast<bool>(this->m_value);
    }else if(this->is_number()){
        auto num = std::any_cast<Number>(this->m_value);
        result = static_cast<bool>(num);
    }else if(this->is_nil()){
        result = false;
    }else if(this->is_string()){
        auto xs = std::any_cast<String>(this->m_value);
        result = (xs.len()==0);
    }else if(this->is_array()){
        auto xs = std::any_cast<Array>(this->m_value);
        result = (xs.len()==0);
    }else if(this->is_list()){
        auto xs = std::any_cast<List>(this->m_value);
        result = (xs.len()==0);
    }else if(this->is_dict()){
        auto xs = std::any_cast<Dict>(this->m_value);
        result = (xs.len()==0);
    }else if(this->is_set()){
        auto xs = std::any_cast<Set>(this->m_value);
        result = (xs.len()==0);
    }else if(this->is_pair()){
        auto xs = std::any_cast<Pair>(this->m_value);
        result = (xs.key.is_nil() && xs.val.is_nil());
    }else{
        result = true;
    }

    return result;
}

// -*-
i64 Object::as_integer(void) const{
    i64 result{};
    if(this->is_number()){
        auto num = std::any_cast<Number>(this->m_value);
        result = static_cast<i64>(num);
    }else if(this->is_bool()){
        auto val = std::any_cast<bool>(this->m_value);
        result = static_cast<i64>(val ? 1 : 0);
    }else if(this->is_string()){
        auto val = std::any_cast<String>(this->m_value);
        size_t pos{};
        if(val.text.find('.')!=std::string::npos){
            f64 num{};
            try{
                num = std::stod(val.text, &pos);
            }catch(const std::invalid_argument& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(const std::out_of_range& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(...){
                throw ELixError(
                    ELixError::ValueError,
                    "unexpected error occurred while converting numeric string to number"
                );
            }
            if(pos==val.len()){
                result = static_cast<i64>(num);
            }else{
                std::stringstream ss;
                ss << "argument is not a numeric string";
                throw ELixError(ELixError::ValueError, ss.str());
            }
        }else if(val.text.find('e')!=std::string::npos || val.text.find('E')!=std::string::npos){
            f64 num{};
            try{
                num = std::stod(val.text, &pos);
            }catch(const std::invalid_argument& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(const std::out_of_range& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(...){
                throw ELixError(
                    ELixError::ValueError,
                    "unexpected error occurred while converting numeric string to number"
                );
            }
            if(pos==val.len()){
                result = static_cast<i64>(num);
            }else{
                std::stringstream ss;
                ss << "argument is not a numeric string";
                throw ELixError(ELixError::ValueError, ss.str());
            }
        }else if(val.text[0]=='0' && val.text[1]=='x'){
            i64 num{};
            try{
                num = std::stoll(val.text.substr(2), &pos, 16);
            }catch(const std::invalid_argument& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(const std::out_of_range& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(...){
                throw ELixError(
                    ELixError::ValueError,
                    "unexpected error occurred while converting numeric string to number"
                );
            }
            if(pos!=val.len()){
                std::stringstream ss;
                ss << "argument is not a numeric string";
                throw ELixError(ELixError::ValueError, ss.str());
            }
        }else if(val.text[0]=='0' && val.text[1]=='o'){
            i64 num{};
            try{
                num = std::stoll(val.text.substr(2), &pos, 8);
            }catch(const std::invalid_argument& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(const std::out_of_range& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(...){
                throw ELixError(
                    ELixError::ValueError,
                    "unexpected error occurred while converting numeric string to number"
                );
            }
            if(pos!=val.len()){
                std::stringstream ss;
                ss << "argument is not a numeric string";
                throw ELixError(ELixError::ValueError, ss.str());
            }
        }else if(val.text[0]=='0' && val.text[1]=='b'){
            i64 num{};
            try{
                num = std::stoll(val.text.substr(2), &pos, 2);
            }catch(const std::invalid_argument& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(const std::out_of_range& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(...){
                throw ELixError(
                    ELixError::ValueError,
                    "unexpected error occurred while converting numeric string to number"
                );
            }
            if(pos!=val.len()){
                std::stringstream ss;
                ss << "argument is not a numeric string";
                throw ELixError(ELixError::ValueError, ss.str());
            }
        }else{
            i64 num{};
            try{
                num = std::stoll(val.text, &pos);
            }catch(const std::invalid_argument& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(const std::out_of_range& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(...){
                throw ELixError(
                    ELixError::ValueError,
                    "unexpected error occurred while converting numeric string to number"
                );
            }
            if(pos!=val.len()){
                std::stringstream ss;
                ss << "argument is not a numeric string";
                throw ELixError(ELixError::ValueError, ss.str());
            }
        }
    }else{
        std::stringstream ss;
        ss << std::quoted(this->type().str()) << " cannot be cast to an integer";
        throw ELixError(ELixError::TypeError, ss.str());
    }
    return result;
}

// -*-
f64 Object::as_float(void) const{
    f64 result{};
    if(this->is_number()){
        auto num = std::any_cast<Number>(this->m_value);
        result = static_cast<f64>(num);
    }else if(this->is_bool()){
        auto val = std::any_cast<bool>(this->m_value);
        result = static_cast<f64>(val ? 1 : 0);
    }else if(this->is_string()){
        auto val = std::any_cast<String>(this->m_value);
        size_t pos{};
        if(val.text.find('.')!=std::string::npos){
            try{
                result = std::stod(val.text, &pos);
            }catch(const std::invalid_argument& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(const std::out_of_range& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(...){
                throw ELixError(
                    ELixError::ValueError,
                    "unexpected error occurred while converting numeric string to number"
                );
            }
            if(pos!=val.len()){
                std::stringstream ss;
                ss << "argument is not a numeric string";
                throw ELixError(ELixError::ValueError, ss.str());
            }
        }else if(val.text.find('e')!=std::string::npos || val.text.find('E')!=std::string::npos){
            try{
                result = std::stod(val.text, &pos);
            }catch(const std::invalid_argument& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(const std::out_of_range& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(...){
                throw ELixError(
                    ELixError::ValueError,
                    "unexpected error occurred while converting numeric string to number"
                );
            }
            if(pos!=val.len()){
                std::stringstream ss;
                ss << "argument is not a numeric string";
                throw ELixError(ELixError::ValueError, ss.str());
            }
        }else if(val.text[0]=='0' && val.text[1]=='x'){
            i64 num{};
            try{
                num = std::stoll(val.text.substr(2), &pos, 16);
            }catch(const std::invalid_argument& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(const std::out_of_range& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(...){
                throw ELixError(
                    ELixError::ValueError,
                    "unexpected error occurred while converting numeric string to number"
                );
            }
            if(pos==val.len()){
                result = static_cast<f64>(num);
            }else{
                std::stringstream ss;
                ss << "argument is not a numeric string";
                throw ELixError(ELixError::ValueError, ss.str());
            }
        }else if(val.text[0]=='0' && val.text[1]=='o'){
            i64 num{};
            try{
                num = std::stoll(val.text.substr(2), &pos, 8);
            }catch(const std::invalid_argument& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(const std::out_of_range& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(...){
                throw ELixError(
                    ELixError::ValueError,
                    "unexpected error occurred while converting numeric string to number"
                );
            }
            if(pos==val.len()){
                result = static_cast<f64>(num);
            }else{
                std::stringstream ss;
                ss << "argument is not a numeric string";
                throw ELixError(ELixError::ValueError, ss.str());
            }
        }else if(val.text[0]=='0' && val.text[1]=='b'){
            i64 num{};
            try{
                num = std::stoll(val.text.substr(2), &pos, 2);
            }catch(const std::invalid_argument& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(const std::out_of_range& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(...){
                throw ELixError(
                    ELixError::ValueError,
                    "unexpected error occurred while converting numeric string to number"
                );
            }
            if(pos==val.len()){
                result = static_cast<f64>(num);
            }else{
                std::stringstream ss;
                ss << "argument is not a numeric string";
                throw ELixError(ELixError::ValueError, ss.str());
            }
        }else{
            i64 num{};
            try{
                num = std::stoll(val.text, &pos);
            }catch(const std::invalid_argument& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(const std::out_of_range& err){
                throw ELixError(ELixError::ValueError, std::string(err.what()));
            }
            catch(...){
                throw ELixError(
                    ELixError::ValueError,
                    "unexpected error occurred while converting numeric string to number"
                );
            }
            if(pos==val.len()){
                result = static_cast<f64>(num);
            }else{
                std::stringstream ss;
                ss << "argument is not a numeric string";
                throw ELixError(ELixError::ValueError, ss.str());
            }
        }
    }else{
        std::stringstream ss;
        ss << std::quoted(this->type().str()) << " cannot be cast to a floating-point.";
        throw ELixError(ELixError::TypeError, ss.str());
    }
    return result;
}

// -*-
Symbol Object::as_symbol(void) const{
    Symbol result{};
    if(this->is_symbol()){
        result = std::any_cast<Symbol>(this->m_value);
    }else if(this->is_string()){
        auto mstr = std::any_cast<String>(this->m_value);
        result.value = mstr.text;
    }else{
        std::stringstream ss;
        ss << std::quoted(this->type().str()) << " cannot be cast to a symbol object.";
        throw ELixError(ELixError::TypeError, ss.str());
    }

    return result;
}

// -*-
String Object::as_string(void) const{
    String result{};
    result.text = this->str();
    return result;
}

// -*-
Array Object::as_array(void) const{
    Array result{};
    if(this->is_array()){
        result = std::any_cast<Array>(this->m_value);
    }else if(this->is_list()){
        auto xs = std::any_cast<List>(this->m_value);
        result.items = Vec<Object>(xs.items.cbegin(), xs.items.cend());
    }else if(this->is_pair()){
        auto xs = std::any_cast<Pair>(this->m_value);
        result.items = Vec<Object>{Object(xs.key), Object(xs.val)};
    }else if(this->is_dict()){
        auto xs = std::any_cast<Dict>(this->m_value);
        for(const auto& pair: xs.items()){
            result.push(Object(pair));
        }
    }else if(this->is_set()){
        auto xs = std::any_cast<Set>(this->m_value);
        for(const auto& key: xs.hset){
            result.push(Object(key));
        }
    }else if(this->is_string()){
        auto xs = std::any_cast<String>(this->m_value);
        for(const auto& c: xs.text){
            auto txt = std::string(1, c);
            auto mstr = String{txt};
            result.push(Object(mstr));
        }
    }else{
        std::stringstream ss;
        ss << std::quoted(this->type().str()) << " cannot be cast to an array object.";
        throw ELixError(ELixError::TypeError, ss.str());
    }

    return result;
}

// -*-
Dict Object::as_dict(void) const{
    Dict result{};
    if(this->is_dict()){
        result = std::any_cast<Dict>(this->m_value);
    }else if(this->is_list()){
        auto xs = std::any_cast<List>(this->m_value);
        for(const auto& item: xs.items){
            if(!item.is_pair()){
                std::stringstream ss;
                ss << "Error wile constructing a Dict object from a list.\n";
                ss << "Expect elements of the list to be pairs but found ";
                ss << std::quoted(item.type().str()) << " object.";
                throw ELixError(ELixError::TypeError, ss.str());
            }
            auto pair = item.as_pair();
            result.put(Object(pair.key), Object(pair.val));
        }
    }else if(this->is_array()){
        auto xs = std::any_cast<Array>(this->m_value);
        for(const auto& item: xs.items){
            if(!item.is_pair()){
                std::stringstream ss;
                ss << "Error wile constructing a Dict object from an array.\n";
                ss << "Expect elements of the array to be pairs but found ";
                ss << std::quoted(item.type().str()) << " object.";
                throw ELixError(ELixError::TypeError, ss.str());
            }
            auto pair = item.as_pair();
            result.put(Object(pair.key), Object(pair.val));
        }
    }else if(this->is_set()){
        auto xs = std::any_cast<Set>(this->m_value);
        for(const auto& key: xs.hset){
            if(!key.is_pair()){
                std::stringstream ss;
                ss << "Error wile constructing a Dict object from a set.\n";
                ss << "Expect elements of the set to be pairs of hashable objects\nbut found ";
                ss << std::quoted(key.type().str()) << " object.";
                throw ELixError(ELixError::TypeError, ss.str());
            }
            auto pair = key.as_pair();
            result.put(Object(pair.key), Object(pair.val));
        }
    }else{
        std::stringstream ss;
        ss << std::quoted(this->type().str()) << " cannot be cast to a dict object.";
        throw ELixError(ELixError::TypeError, ss.str());
    }

    return result;
}

// -*-
List Object::as_list(void) const{
    List result{};
    if(this->is_array()){
        result = std::any_cast<List>(this->m_value);
    }else if(this->is_list()){
        auto xs = std::any_cast<Array>(this->m_value);
        result.items = Vec<Object>(xs.items.cbegin(), xs.items.cend());
    }else if(this->is_pair()){
        auto xs = std::any_cast<Pair>(this->m_value);
        result.items = Vec<Object>{Object(xs.key), Object(xs.val)};
    }else if(this->is_dict()){
        auto xs = std::any_cast<Dict>(this->m_value);
        for(const auto& pair: xs.items()){
            result.push(Object(pair));
        }
    }else if(this->is_set()){
        auto xs = std::any_cast<Set>(this->m_value);
        for(const auto& key: xs.hset){
            result.push(Object(key));
        }
    }else if(this->is_string()){
        auto xs = std::any_cast<String>(this->m_value);
        for(const auto& c: xs.text){
            auto txt = std::string(1, c);
            auto mstr = String{txt};
            result.push(Object(mstr));
        }
    }else{
        std::stringstream ss;
        ss << std::quoted(this->type().str()) << " cannot be cast to an array object.";
        throw ELixError(ELixError::TypeError, ss.str());
    }

    return result;
}

// -*-
Set Object::as_set(void) const{
    Set result{};
    if(this->is_set()){
        result = std::any_cast<Set>(this->m_value);
    }else if(this->is_dict()){
        auto xs = std::any_cast<Dict>(this->m_value);
        for(const auto pair: xs.items()){
            result.add(Object(pair));
        }
    }else if(this->is_list()){
        auto xs = std::any_cast<List>(this->m_value);
        for(const auto& key: xs.items){
            result.add(key);
        }
    }else if(this->is_array()){
        auto xs = std::any_cast<Array>(this->m_value);
        for(const auto& key: xs.items){
            result.add(key);
        }
    }else if(this->is_string()){
        auto xs = std::any_cast<String>(this->m_value);
        for(const auto& c: xs.text){
            std::string txt(1, c);
            result.add(Object(txt));
        }
    }else{
        std::stringstream ss;
        ss << std::quoted(this->type().str()) << " cannot be cast to a set object.";
        throw ELixError(ELixError::TypeError, ss.str());
    }

    return result;
}

// -*-
Lambda Object::as_lambda(void) const{
    Lambda result{};
    if(this->is_lambda() || this->is_function()){
        result = std::any_cast<Lambda>(this->m_value);
        result.named = false;
    }else{
        std::stringstream ss;
        ss << std::quoted(this->type().str()) << " cannot be cast to a lambda object.";
        throw ELixError(ELixError::TypeError, ss.str());
    }
    return result;
}

/*
// -*-
class Object final{
public:


Macro Object::as_macro(void) const{}
Lambda Object::as_function(void) const{}
Func Object::as_func(void) const{}
Pair Object::as_pair(void) const{}

std::string Object::str(void) const{}
std::string Object::repr(void) const{}
Object Object::clone(void) const{}

bool Object::is_hashable(void) const{}
bool Object::is_iterable(void) const{}
Symbol Object::type(void) const{}

bool operator==(const Object& lhs, const Object& rhs){}
bool operator!=(const Object& lhs, const Object& rhs){}
bool operator<=(const Object& lhs, const Object& rhs){}
bool operator>=(const Object& lhs, const Object& rhs){}
bool operator<(const Object& lhs, const Object& rhs){}
bool operator>(const Object& lhs, const Object& rhs){}
Object operator+(const Object& lhs, const Object& rhs){}
Object operator-(const Object& lhs, const Object& rhs){}
Object operator*(const Object& lhs, const Object& rhs){}
Object operator/(const Object& lhs, const Object& rhs){}
Object operator%(const Object& lhs, const Object& rhs){}

Object operator<<(const Object& lhs, const Object& rhs){}
Object operator>>(const Object& lhs, const Object& rhs){}
Object operator|(const Object& lhs, const Object& rhs){}
Object operator&(const Object& lhs, const Object& rhs){}
Object operator^(const Object& lhs, const Object& rhs){}

Object operator||(const Object& lhs, const Object& rhs){}
Object operator&&(const Object& lhs, const Object& rhs){}

Object Object::logical_not(void) const{}
Object Object::negate(void) const{}
Object Object::bitwise_not(void) const{}

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