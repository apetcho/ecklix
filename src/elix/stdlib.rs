use super::types::*;
use super::EvalError;

// -
pub fn load_standard_library(env: &mut Env){
    // Basic erithmetic
    env.bind("+", Value::Symbol("+".to_string()));
    env.bind("-", Value::Symbol("-".to_string()));
    env.bind("*", Value::Symbol("*".to_string()));
    env.bind("/", Value::Symbol("/".to_string()));
    env.bind("%", Value::Symbol("%".to_string()));

    // Comparison
    env.bind("=", Value::Symbol("=".to_string()));
    env.bind("!=", Value::Symbol("!=".to_string()));
    env.bind(">", Value::Symbol(">".to_string()));
    env.bind("<", Value::Symbol("<".to_string()));
    env.bind(">=", Value::Symbol(">=".to_string()));
    env.bind("<=", Value::Symbol("<=".to_string()));

    // Logical
    env.bind("and", Value::Symbol("and".to_string()));
    env.bind("or", Value::Symbol("or".to_string()));
    env.bind("not", Value::Symbol("not".to_string()));

    // Type checking
    env.bind("nil-p", Value::Symbol("nil-p".to_string()));
    env.bind("bool-p", Value::Symbol("bool-p".to_string()));
    env.bind("integer-p", Value::Symbol("integer-p".to_string()));
    env.bind("float-p", Value::Symbol("float-p".to_string()));
    env.bind("number-p", Value::Symbol("number-p".to_string()));
    env.bind("char-p", Value::Symbol("char-p".to_string()));
    env.bind("string-p", Value::Symbol("string-p".to_string()));
    env.bind("keyword-p", Value::Symbol("keyword-p".to_string()));
    env.bind("list-p", Value::Symbol("list-p".to_string()));
    env.bind("array-p", Value::Symbol("array-p".to_string()));
    env.bind("hashmap-p", Value::Symbol("hashmap-p".to_string()));
    env.bind("hashset-p", Value::Symbol("hashset-p".to_string()));
    env.bind("function-p", Value::Symbol("function-p".to_string()));
    env.bind("macro-p", Value::Symbol("macro-p".to_string()));
    env.bind("class-p", Value::Symbol("class-p".to_string()));

    // Type conversion
    env.bind("integer", Value::Symbol("integer".to_string()));
    env.bind("float", Value::Symbol("float".to_string()));
    env.bind("string", Value::Symbol("string".to_string()));
    env.bind("bool", Value::Symbol("bool".to_string()));

    // String operations
    env.bind("len", Value::Symbol("len".to_string()));
    env.bind("upper", Value::Symbol("upper".to_string()));
    env.bind("lower", Value::Symbol("lower".to_string()));
    env.bind("trim", Value::Symbol("trim".to_string()));
    env.bind("concat", Value::Symbol("concat".to_string()));
    env.bind("split", Value::Symbol("split".to_string()));
    env.bind("join", Value::Symbol("join".to_string()));
    env.bind("contains", Value::Symbol("contains".to_string()));
    env.bind("starts-with", Value::Symbol("starts-with".to_string()));
    env.bind("ends-with", Value::Symbol("ends-with".to_string()));

    // List operations
    env.bind("list", Value::Symbol("list".to_string()));
    env.bind("head", Value::Symbol("head".to_string()));
    env.bind("tail", Value::Symbol("tail".to_string()));
    env.bind("first", Value::Symbol("first".to_string()));
    env.bind("last", Value::Symbol("last".to_string()));
    env.bind("rest", Value::Symbol("rest".to_string()));
    env.bind("append", Value::Symbol("append".to_string()));
    env.bind("reverse", Value::Symbol("reverse".to_string()));
    env.bind("map", Value::Symbol("map".to_string()));
    env.bind("filter", Value::Symbol("filter".to_string()));
    env.bind("reduce", Value::Symbol("reduce".to_string()));
    env.bind("flatten", Value::Symbol("flatten".to_string()));
    env.bind("unique", Value::Symbol("unique".to_string()));

    // Array operations
    env.bind("array", Value::Symbol("array".to_string()));
    env.bind("array-push", Value::Symbol("array-push".to_string()));
    env.bind("array-pop", Value::Symbol("array-pop".to_string()));
    env.bind("array-push-front", Value::Symbol("array-push-front".to_string()));
    env.bind("array-pop-front", Value::Symbol("array-pop-front".to_string()));
    env.bind("array-slice", Value::Symbol("array-slice".to_string()));

    // Hashmap operations
    env.bind("hashmap", Value::Symbol("hashmap".to_string()));
    env.bind("hashmap-set", Value::Symbol("hashmap-set".to_string()));
    env.bind("hashmap-get", Value::Symbol("hashmap-get".to_string()));
    env.bind("hashmap-delete", Value::Symbol("hashmap-delete".to_string()));
    env.bind("hashmap-keys", Value::Symbol("hashmap-keys".to_string()));
    env.bind("hashmap-values", Value::Symbol("hashmap-values".to_string()));
    env.bind("hashmap-contains", Value::Symbol("hashmap-contains".to_string()));

    // Hashset operations
    env.bind("hashset", Value::Symbol("hashset".to_string()));
    env.bind("hashset-add", Value::Symbol("hashset-add".to_string()));
    env.bind("hashset-remove", Value::Symbol("hashset-remove".to_string()));
    env.bind("hashset-contains", Value::Symbol("hashset-contains".to_string()));
    env.bind("hashset-union", Value::Symbol("hashset-union".to_string()));
    env.bind("hashset-intersection", Value::Symbol("hashset-intersection".to_string()));
    env.bind("hashset-difference", Value::Symbol("hashset-difference".to_string()));

    // I/O
    env.bind("print", Value::Symbol("print".to_string()));
    env.bind("println", Value::Symbol("println".to_string()));
    env.bind("read", Value::Symbol("read".to_string()));
    env.bind("readln", Value::Symbol("readln".to_string()));
    env.bind("file-read", Value::Symbol("file-read".to_string()));
    env.bind("file-write", Value::Symbol("file-write".to_string()));
    env.bind("file-exists", Value::Symbol("file-exists".to_string()));

    // System
    env.bind("system", Value::Symbol("system".to_string()));
    env.bind("exit", Value::Symbol("exit".to_string()));
    env.bind("time", Value::Symbol("time".to_string()));
    env.bind("random", Value::Symbol("random".to_string()));

    // Debug
    env.bind("type", Value::Symbol("type".to_string()));
    env.bind("inspect", Value::Symbol("inspect".to_string()));
    env.bind("dump", Value::Symbol("dump".to_string()));
}

// Helper functions for standar library
// pub fn string_to_int(...)
// pub fn float_to_int(...)
// pub fn int_to_float(...)
// pub fn is_even(...)
// pub fn is_odd(...)
// pub fn is_positive(...)
// pub fn is_negative(...)
// pub fn is_zero(...)