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

impl Minibuffer {
    // pub fn new(...)
    // pub fn set_prompt(...)
    // pub fn set_active(...)
    // pub fn is_active(...)
    // pub fn input(...)
    // pub fn input_mut(...)
    // pub fn set_message(...)
    // pub fn message(...)
    // pub fn set_history(...)
    // pub fn add_to_history(...)
    // pub fn set_completion_candidates(...)
    // pub fn next_completion(...)
    // pub fn prev_completion(...)
    // pub fn apply_completion(...)
    // pub fn show_completion(...)
    // pub fn render(...)
}