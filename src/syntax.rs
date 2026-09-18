use tree_sitter::{Parser, Language};
use std::fmt;


#[derive(Debug, Clone)]
pub struct SyntaxHighlighter {
    parser: Parser,
    languages: std::collections::HashMap<String, Language>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum SyntaxMode {
    Rust,
    Lisp,
    Plain,
    Auto,
}

impl SyntaxHighlighter {
    // pub fn new(...)
    // pub fn highlight(...)
    // fn highlight_rust(...)
    // fn highlight_node(...)
    // fn highlight_lisp(...)
    // fn highlight_plain(...)
    // fn highlight_auto(...) 
}