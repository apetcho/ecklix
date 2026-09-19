use super::types::*;
use std::collections::HashMap;
use std::sync::{Arc, Mutex};

#[derive(Debug, Clone)]
pub struct Env {
    parent: Option<Arc<Mutex<Env>>>,
    bindings: HashMap<String, Value>,
    id: usize,
}

impl Env {
    // pub fn root(...)
    // pub fn new(parent: Arc<Mutex<Env>>) -> Self
    // pub fn bind(...)
    // pub fn get(...)
    // pub fn get_all(...)
    // pub fn has(...)
}