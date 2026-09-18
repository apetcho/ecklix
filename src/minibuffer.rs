use std::collections::VecDeque;

#[derive(Debug, Clone)]
pub struct Minibuffer {
    prompt: String,
    input: String,
    history: VecDeque<String>,
    history_index: usize,
    active: bool,
    message: Option<String>,
    completion_candidates: Vec<String>,
    completion_index: usize,
}