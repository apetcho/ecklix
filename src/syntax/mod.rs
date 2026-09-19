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