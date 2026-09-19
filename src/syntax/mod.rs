mod highlighter;
mod colorscheme;
mod language;
mod tree_sitter_wrapper;

pub use highlighter::*;
pub use colorscheme::*;
pub use language::*;
pub use tree_sitter_wrapper::*;

use std::collections::HashMap;
use std::fs;
use std::path::Path;

#[derive(Debug, Clone)]
pub struct SyntaxHighlighter {
    colorscheme: ColorScheme,
    language_detector: LanguageDetector,
    parsers: HashMap<String, Box<dyn SyntaxParser>>,
    file_extensions: HashMap<String, String>,
}

impl SyntaxHighlighter {
    // pub fn new(...)
    // pub fn set_colorscheme(...)
    // pub fn get_colorscheme(...)
    // pub fn register_parser(...)
    // pub fn get_parser(...)
    // pub fn register_extension(...)
    // pub fn detect_language(...)
    // pub fn highlight_file(...)
    // pub fn highlight_text(...)
    // pub fn get_style_for_token(...)
}

pub trait SyntaxParser: Send + Sync {
    fn parse(&self, text: &str) -> Result<Vec<HighlightedToken>, SyntaxError>;
    fn language(&self) -> &str;
}


#[derive(Debug, Clone)]
pub struct HighlightedToken {
    pub range: std::ops::Range<usize>,
    pub style: Style,
    pub token_type: String,
}


#[derive(Debug, Clone)]
pub struct HighlightLine {
    pub spans: Vec<HighlightSpan>,
    pub line_number: usize,
}