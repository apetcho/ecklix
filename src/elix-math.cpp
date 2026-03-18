#include "elix.hpp"
#include<cmath>
#include<random>

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

// -*-
static Object fn_math_exp2(const Vec<Object>& args, ELix* elix){
    // (Math.exp2 num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.exp2");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.exp2 num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().exp2();

    return Object(result);
}

// -*-
static Object fn_math_expm1(const Vec<Object>& args, ELix* elix){
    // (Math.expm1 num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.expm1");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.expm1 num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().expm1();

    return Object(result);
}

// -*-
static Object fn_math_pow(const Vec<Object>& args, ELix* elix){
    // (Math.pow base expo)
    auto pred = (args.size()==2);
    ELix::validate_argc(pred, "Math.pow");
    pred = args[0].is_number() && args[1].is_number();
    ELix::validate_type(
        pred, "`(Math.pow base expo)'",
        "expect arguments `base' and `expo' to be numbers."
    );
    auto base = args[0].as_number();
    auto expo = args[1].as_number();
    auto result = base.pow(expo);

    return Object(result);
}

// -*-
static Object fn_math_sqrt(const Vec<Object>& args, ELix* elix){
    // (Math.sqrt num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.sqrt");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.sqrt num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().sqrt();

    return Object(result);
}

// -*-
static Object fn_math_cbrt(const Vec<Object>& args, ELix* elix){
    // (Math.cbrt num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.cbrt");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.cbrt num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().cbrt();

    return Object(result);
}

// -*-
static Object fn_math_erf(const Vec<Object>& args, ELix* elix){
    // (Math.erf num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.erf");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.erf num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().erf();

    return Object(result);
}

// -*-
static Object fn_math_erfc(const Vec<Object>& args, ELix* elix){
    // (Math.erfc num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.erfc");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.erfc num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().erfc();

    return Object(result);
}

// -*-
static Object fn_math_tgamma(const Vec<Object>& args, ELix* elix){
    // (Math.tgamma num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.tgamma");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.tgamma num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().tgamma();

    return Object(result);
}

static Object fn_math_lgamma(const Vec<Object>& args, ELix* elix){
    // (Math.lgamma num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.lgamma");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.lgamma num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().lgamma();

    return Object(result);
}

// -*-
static Object fn_math_isinf(const Vec<Object>& args, ELix* elix){
    // (Math.isinf? num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.isinf?");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.isinf? num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().isinf();

    return Object(result);
}

// -*-
static Object fn_math_isfinite(const Vec<Object>& args, ELix* elix){
    // (Math.isfinite? num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.isfinite?");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.isfinite? num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().isfinite();

    return Object(result);
}

// -*-
static Object fn_math_isnan(const Vec<Object>& args, ELix* elix){
    // (Math.isnan? num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.isnan?");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.isnan? num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().isnan();

    return Object(result);
}

// -*-
static Object fn_math_log(const Vec<Object>& args, ELix* elix){
    // (Math.log num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.log");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.log num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().log();

    return Object(result);
}

// -*-
static Object fn_math_log2(const Vec<Object>& args, ELix* elix){
    // (Math.log2 num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.log2");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.log2 num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().log2();

    return Object(result);
}

// -*-
static Object fn_math_log10(const Vec<Object>& args, ELix* elix){
    // (Math.log10 num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.log10");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.log10 num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().log10();

    return Object(result);
}

static Object fn_math_log1p(const Vec<Object>& args, ELix* elix){
    // (Math.log1p num)
    auto pred = (args.size()==1);
    ELix::validate_argc(pred, "Math.log1p");
    pred = args[0].is_number();
    ELix::validate_type(pred, "`(Math.log1p num)'", "expect argument `num' to be a number.");
    auto result = args[0].as_number().log1p();

    return Object(result);
}

// -*-
class Randomizer{
public:
    f64 nextFloat();
    f64 nextFloat(f64 vmax);
    f64 nextFloat(f64 vmin, f64 vmax);
    i64 nextInt(i64 vmax);
    i64 nextInt(i64 vmin, i64 vmax);

private:
    void setup(i64 vmin, i64 vmax);
    void setup(f64 vmin, f64 vmax);
};

// -*-
static Randomizer randomizer{};

// -*-
static Object fn_random_random(const Vec<Object>& args, ELix* elix){
    // (Random.random)
    // (Random.random N)
    // (Random.random vmax N)
    // (Random.random vmin vmax N)
    auto pred = (args.size() <= 3);
    ELix::validate_argc(pred, "Random.random");

    Object result{};
    if(args.size()==0){
        auto num = randomizer.nextFloat(0.0, 1.0);
        result = Object(Number(num));
    }else if(args.size()==1){
        pred = args[0].is_integer();
        ELix::validate_type(
            pred, "`(Random.random N)'", "expect `N' to be an integer."
        );
        auto N = args[0].as_integer();
        pred = (N > 0);
        ELix::validate_value(
            pred, "`(Random.random N)'", "expect `N' to be an integer such that N > 0."
        );
        Vec<Object> vec(N);
        for(i64 i=0; i < N; i++){
            auto num = Number(randomizer.nextFloat(0.0, 1.0));
            vec.push_back(Object(num));
        }
        result = Object(Array{vec});
    }else if(args.size()==2){
        pred = args[0].is_number() && args[1].is_integer();
        ELix::validate_type(
            pred, "`(Random.random vmax N)'",
            "expect `vmax' as a number and `N' to be an integer."
        );
        auto vmax = args[0].as_float();
        auto N = args[1].as_integer();
        pred = (N > 0 && vmax > 0.0);
        ELix::validate_value(
            pred, "`(Random.random vmax N)'",
            "expect `vmax' as a number, `N' an integer such that N > 0 and vmax > 0.0."
        );
        Vec<Object> vec(N);
        for(i64 i=0; i < N; i++){
            auto num = Number(randomizer.nextFloat(0.0, vmax));
            vec.push_back(Object(num));
        }
        result = Object(Array{vec});
    }else{
        pred = (args[0].is_number() && args[1].is_number() && args[2].is_integer());
        ELix::validate_type(
            pred, "`(Random.random vmin vmax N)'",
            "expect `vmin' and `vmax' as number and `N' to be an integer."
        );
        auto vmin = args[0].as_float();
        auto vmax = args[1].as_float();
        auto N = args[2].as_integer();
        pred = (N > 0 && vmax > vmin);
        ELix::validate_value(
            pred, "`(Random.random vmax N)'",
            "expect `vmin' and `vmax' as numbers, `N' an integer such that N > 0 and vmax > vmin."
        );
        Vec<Object> vec(N);
        for(i64 i=0; i < N; i++){
            auto num = Number(randomizer.nextFloat(vmin, vmax));
            vec.push_back(Object(num));
        }
        result = Object(Array{vec});
    }

    return result;
}

// -*-
static Object fn_random_nextFloat(const Vec<Object>& args, ELix* elix){
    // (Random.nextFloat)
    // (Random.nextFloat vmax)
    // (Random.nextFloat vmin vmax)
    auto pred = (args.size() <= 2);
    ELix::validate_argc(pred, "Random.nextFloat");

    Object result{};
    if(args.size()==0){
        auto num = randomizer.nextFloat();
        result = Object(Number(num));
    }else if(args.size()==1){
        pred = args[0].is_number();
        ELix::validate_type(
            pred, "`(Random.nextFloat vmax)'", "expect `vmax' to be a number."
        );
        auto vmax = args[0].as_float();
        result = Object(Number(randomizer.nextFloat(vmax)));
    }else{
        pred = (args[0].is_number() && args[1].is_number());
        ELix::validate_type(
            pred, "`(Random.nextFloat vmin vmax)'",
            "expect `vmin' and `vmax' as numbers."
        );
        auto vmin = args[0].as_float();
        auto vmax = args[1].as_float();
        pred = (vmin < vmax);
        ELix::validate_value(
            pred, "`(Random.nextFloat vmin vmax)'",
            "expect `vmin' and `vmax' as numbers such that vmin < vmax."
        );
        auto num = Number(randomizer.nextFloat(vmin, vmax));
        result = Object(num);
    }

    return result;
}

// -*-
static Object fn_random_nextInteger(const Vec<Object>& args, ELix* elix){
    //! @todo
    return Object();
}


// -*-
void ELix::initialize_math(void){
    ELix::add_builtin("Math.PI", Object(Number(3.141592653589793)));    // Math.PI
    ELix::add_builtin("Math.E", Object(Number(2.718281828459045)));     // Math.E
    ELix::add_builtin("Math.abs", fn_math_abs, 1, 1);                   // (Math.abs x)
    ELix::add_builtin("Math.ceil", fn_math_ceil, 1, 1);                 // (Math.ceil x)
    ELix::add_builtin("Math.round", fn_math_round, 1, 1);               // (Math.round x)
    ELix::add_builtin("Math.floor", fn_math_floor, 1, 1);               // (Math.floor x)
    ELix::add_builtin("Math.truncate", fn_math_truncate, 1, 1);         // (Math.truncate x)
    ELix::add_builtin("Math.sin", fn_math_sin, 1, 1);                   // (Math.sin x)
    ELix::add_builtin("Math.cos", fn_math_cos, 1, 1);                   // (Math.cos x)
    ELix::add_builtin("Math.tan", fn_math_tan, 1, 1);                   // (Math.tan x)
    ELix::add_builtin("Math.asin", fn_math_asin, 1, 1);                 // (Math.asin x)
    ELix::add_builtin("Math.acos", fn_math_acos, 1, 1);                 // (Math.acos x)
    ELix::add_builtin("Math.atan", fn_math_atan, 1, 1);                 // (Math.atan x)
    ELix::add_builtin("Math.atan2", fn_math_atan2, 1, 1);               // (Math.atan2 x)
    ELix::add_builtin("Math.sinh", fn_math_sinh, 1, 1);                 // (Math.sinh x)
    ELix::add_builtin("Math.cosh", fn_math_cosh, 1, 1);                 // (Math.cosh x)
    ELix::add_builtin("Math.tanh", fn_math_tanh, 1, 1);                 // (Math.tanh x)
    ELix::add_builtin("Math.asinh", fn_math_asinh, 1, 1);               // (Math.asinh x)
    ELix::add_builtin("Math.acosh", fn_math_acosh, 1, 1);               // (Math.acosh x)
    ELix::add_builtin("Math.atanh", fn_math_atanh, 1, 1);               // (Math.atanh x)
    ELix::add_builtin("Math.exp", fn_math_exp, 1, 1);                   // (Math.exp x)
    ELix::add_builtin("Math.exp2", fn_math_exp2, 1, 1);                 // (Math.exp2 x)
    ELix::add_builtin("Math.expm1", fn_math_expm1, 1, 1);               // (Math.expm1 x)
    ELix::add_builtin("Math.pow", fn_math_pow, 2, 2);                   // (Math.pow base expo)
    ELix::add_builtin("Math.sqrt", fn_math_sqrt, 1, 1);                 // (Math.sqrt x)
    ELix::add_builtin("Math.cbrt", fn_math_cbrt, 1, 1);                 // (Math.cbrt x)
    ELix::add_builtin("Math.erf", fn_math_erf, 1, 1);                   // (Msth.erf x)
    ELix::add_builtin("Math.erfc", fn_math_erfc, 1, 1);                 // (Math.erfc x)
    ELix::add_builtin("Math.tgamma", fn_math_tgamma, 1, 1);             // (Math.tgamma x)
    ELix::add_builtin("Math.lgamma", fn_math_lgamma, 1, 1);             // (Math.lgamma x)
    ELix::add_builtin("Math.isinf?", fn_math_isinf, 1, 1);              // (Math.isinf? x)
    ELix::add_builtin("Math.isfinite?", fn_math_isfinite, 1, 1);        // (Math.isfinite? x)
    ELix::add_builtin("Math.isnan?", fn_math_isnan, 1, 1);              // (Math.isnan? x)
    ELix::add_builtin("Math.log", fn_math_log, 1, 1);                   // (Math.log x)
    ELix::add_builtin("Math.log2", fn_math_log2, 1, 1);                 // (Math.log2 x)
    ELix::add_builtin("Math.log10", fn_math_log10, 1, 1);               // (Math.log10 x)
    ELix::add_builtin("Math.log1p", fn_math_log1p, 1, 1);               // (Math.log1p x)
    ELix::add_builtin("Random.random", fn_random_random, 0, 3);         // (Random.random [vmin vmax count])
    ELix::add_builtin("Random.nextFloat", fn_random_nextFloat, 0, 2);   // (Random.nextFloat [vmin vmax])
    ELix::add_builtin("Random.nextInteger", fn_random_nextInteger, 0, 2);//(Random.nextInteger [vmin vmax])
}

// -*--------------------------------------------------------------------------*-
}//-*- end::namespace::ekasoft::elx                                           -*-
// -*--------------------------------------------------------------------------*-