
mod types;
mod parser;
mod eval;
mod stdlib;
pub mod package;

pub use types::*;
pub use parser::*;
pub use eval::*;
pub use env::*;
pub use stdlib::*;
pub use package::*;

use std::collections::HashMap;
use std::sync::{Arc, RwLock};

#[derive(Debug)]
pub struct ELixRuntime {
    env: Environment,
    packages: Arc<RwLock<HashMap<String, Package>>>,
    loaded_packages: Arc<RwLock<Vec<String>>>,
    function_table: Arc<RwLock<HashMap<String, Box<dyn Fn(&EvalEnv, Vec<Value>) -> Result<Value, EvalError>>>>>,
}