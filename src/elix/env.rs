use super::types::*;
use std::collections::HashMap;
use std::sync::{Arc, Mutex};

#[derive(Debug, Clone)]
pub struct Env {
    parent: Option<Arc<Mutex<Env>>>,
    bindings: HashMap<String, Value>,
    id: usize,
}