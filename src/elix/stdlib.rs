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
}