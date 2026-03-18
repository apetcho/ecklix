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
    // (Math.ceil num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.ceil");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.ceil num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().ceil();

    return Object(result);
}

// -*-
static Object fn_math_round(const Vec<Object>& args, ELix* elix){
    // (Math.round num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.round");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.round num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().round();

    return Object(result);
}

// -*-
static Object fn_math_floor(const Vec<Object>& args, ELix* elix){
    // (Math.floor num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.floor");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.floor num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().floor();

    return Object(result);
}

// -*-
static Object fn_math_truncate(const Vec<Object>& args, ELix* elix){
    // (Math.truncate num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.truncate");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.truncate num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().truncate();

    return Object(result);
}

// -*-
static Object fn_math_sin(const Vec<Object>& args, ELix* elix){
    // (Math.sin num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.sin");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.sin num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().sin();

    return Object(result);
}

// -*-
static Object fn_math_cos(const Vec<Object>& args, ELix* elix){
    // (Math.cos num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.cos");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.cos num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().cos();

    return Object(result);
}

// -*-
static Object fn_math_tan(const Vec<Object>& args, ELix* elix){
    // (Math.tan num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.tan");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.tan num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().tan();

    return Object(result);
}

// -*-
static Object fn_math_asin(const Vec<Object>& args, ELix* elix){
    // (Math.asin num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.asin");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.asin num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().asin();

    return Object(result);
}

// -*-
static Object fn_math_acos(const Vec<Object>& args, ELix* elix){
    // (Math.acos num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.acos");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.acos num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().acos();

    return Object(result);
}

// -*-
static Object fn_math_atan(const Vec<Object>& args, ELix* elix){
    // (Math.atan num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.atan");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.atan num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().atan();

    return Object(result);
}

// -*-
static Object fn_math_atan2(const Vec<Object>& args, ELix* elix){
    // (Math.atan2 xnum ynum)
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "Math.atan2");
    pred = args[0].is_number() && args[1].is_number();
    ELix::validate_type(pred, "`(Math.atan2 x y)'", "expect arguments `x' and `y' to be numbers.");
    auto xnum = args[0].as_number();
    auto ynum = args[0].as_number();
    auto result = xnum.atan2(ynum);

    return Object(result);
}

// -*-
static Object fn_math_sinh(const Vec<Object>& args, ELix* elix){
    // (Math.sinh num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.sinh");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.sinh num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().sinh();

    return Object(result);
}

// -*-
static Object fn_math_cosh(const Vec<Object>& args, ELix* elix){
    // (Math.cosh num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.cosh");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.cosh num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().cosh();

    return Object(result);
}

// -*-
static Object fn_math_tanh(const Vec<Object>& args, ELix* elix){
    // (Math.tanh num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.tanh");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.tanh num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().tanh();

    return Object(result);
}

// -*-
static Object fn_math_asinh(const Vec<Object>& args, ELix* elix){
    // (Math.asinh num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.asinh");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.asinh num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().asinh();

    return Object(result);
}

// -*-
static Object fn_math_acosh(const Vec<Object>& args, ELix* elix){
    // (Math.acosh num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.acosh");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.acosh num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().acosh();

    return Object(result);
}

// -*-
static Object fn_math_atanh(const Vec<Object>& args, ELix* elix){
    // (Math.atanh num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.atanh");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.atanh num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().atanh();

    return Object(result);
}

// -*-
static Object fn_math_exp(const Vec<Object>& args, ELix* elix){
    // (Math.exp num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.exp");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.exp num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().exp();

    return Object(result);
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