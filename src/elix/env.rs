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
    pub fn new_root() -> Self {
        Self {
            parent: None,
            bindings: HashMap::new(),
            id: 0,
        }
    }
    
    // pub fn new_child(parent: Arc<Mutex<Env>>) -> Self
    // pub fn bind(...)
    // pub fn get(...)
    // pub fn get_all(...)
    // pub fn has(...)
}

impl Default for Env {
    fn default() -> Self {
        Self::root()
    }
}

impl PartialEq for Env {
    fn eq(&self, other: &Self) -> bool {
        self.id == other.id
    }
}