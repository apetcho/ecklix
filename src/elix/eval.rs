use super::types::*;
use super::parser::*;
use super::env::*;
use super::stdlib;
use crate::elix::EvalError;
use std::collections::HashMap;
use std::sync::{Arc, RwLock};
use parking_lot::RwLock as ParkingRwLock;

pub type Environment = Arc<Env>;
pub type FunctionTable = Arc<ParkingRwLock<HashMap<String, Box<dyn Fn(&Environment, Vec<Value>) -> Result<Value, EvalError>>>>>;
pub type MacroTable = Arc<ParkingRwLock<HashMap<String, Box<dyn Fn(&Environment, Vec<Form>) -> Result<Form, EvalError>>>>>;

// pub fn evaluator(...)
// fn handle_define_function(...)
// fn handle_define_macro(...)
// fn handle_var(...)
// fn handle_let(...)
// fn handle_if(...)
// fn handle_cond(...)
// fn handle_match(...)
// fn match_pattern(...)
// fn handle_for(...)
// fn handle_while(...)
// fn handle_lambda(...)
// fn handle_class(...)
// fn handle_import(...)
// fn handle_export(...)
// fn handle_package(...)
// fn handle_and(...)
// fn handle_or(...)
// fn handle_not(...)
// fn handle_eval(...)
// fn handle_case(...)
// fn handle_try(...)
// fn handle_with(...)
// fn handle_progn(...)
// fn handle_block(...)
// fn handle_backquote(...)
// fn macro_expand(...)
// fn handle_call(...)
// fn binary_operation(...)
// fn unary_operation(...)

// Extension trait for Environment to support binding
trait EnvExt {
    fn bind(&self, name: &str, value: Value);
    fn get(&self, name: &str) -> Option<Value>;
}

// impl EnvExt for Env {}