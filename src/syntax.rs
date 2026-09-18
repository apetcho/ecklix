use tree_sitter::{Parser, Language};
use std::fmt;


#[derive(Debug, Clone)]
pub struct SyntaxHighlighter {
    parser: Parser,
    languages: std::collections::HashMap<String, Language>,
}