#include "elix.hpp"
#include<cmath>


// -*--------------------------------------------------------------------------*-
// -*- begin::namespace::ekasoft::elx                                         -*-
// -*--------------------------------------------------------------------------*-
namespace ekasoft::elx{
// -

// -*-
static Object fn_math_abs(const Vec<Object>& args, ELix* elix){
    // (Math.abs num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.abs");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.abs num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().abs();

    return Object(result);
}

// -*-
static Object fn_math_ceil(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_round(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_floor(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_truncate(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_sin(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_cos(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_tan(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_asin(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_acos(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_atan(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_atan2(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_sinh(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_cosh(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_tanh(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_asinh(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_acosh(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_atanh(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_exp(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_exp2(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_expm1(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_pow(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_sqrt(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_cbrt(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_erf(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_erfc(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_tgamma(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_lgamma(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_isinf(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_isfinite(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

static Object fn_math_isnan(const Vec<Object>& args, ELix* elix){
    //! @todo
    throw ELixError(Symbol{"NotImplementedError"}, __func__);
}

// -*-
void ELix::initialize_math(void){
    //! @todo
    /*
static Object fn_math_abs(const Vec<Object>& args);
static Object fn_math_ceil(const Vec<Object>& args);
static Object fn_math_round(const Vec<Object>& args);
static Object fn_math_floor(const Vec<Object>& args);
static Object fn_math_truncate(const Vec<Object>& args);
static Object fn_math_sin(const Vec<Object>& args);
static Object fn_math_cos(const Vec<Object>& args);
static Object fn_math_tan(const Vec<Object>& args);
static Object fn_math_asin(const Vec<Object>& args);
static Object fn_math_acos(const Vec<Object>& args);
static Object fn_math_atan(const Vec<Object>& args);
static Object fn_math_atan2(const Vec<Object>& args);
static Object fn_math_sinh(const Vec<Object>& args);
static Object fn_math_cosh(const Vec<Object>& args);
static Object fn_math_tanh(const Vec<Object>& args);
static Object fn_math_asinh(const Vec<Object>& args);
static Object fn_math_acosh(const Vec<Object>& args);
static Object fn_math_atanh(const Vec<Object>& args);
static Object fn_math_exp(const Vec<Object>& args);
static Object fn_math_exp2(const Vec<Object>& args);
static Object fn_math_expm1(const Vec<Object>& args);
static Object fn_math_pow(const Vec<Object>& args);
static Object fn_math_sqrt(const Vec<Object>& args);
static Object fn_math_cbrt(const Vec<Object>& args);
static Object fn_math_erf(const Vec<Object>& args);
static Object fn_math_erfc(const Vec<Object>& args);
static Object fn_math_tgamma(const Vec<Object>& args);
static Object fn_math_lgamma(const Vec<Object>& args);
static Object fn_math_isinf(const Vec<Object>& args);
static Object fn_math_isfinite(const Vec<Object>& args);
static Object fn_math_isnan(const Vec<Object>& args);
    */
}

// -*--------------------------------------------------------------------------*-
}//-*- end::namespace::ekasoft::elx                                           -*-
// -*--------------------------------------------------------------------------*-