use crate::syntax::Style;

use std::collections::HashMap;


#[derive(Debug, Clone)]
pub struct ColorScheme {
    pub name: String,
    pub token_styles: HashMap<String, Style>,
    pub default_style: Style,
    pub background_style: Style,
    pub foreground_style: Style,
    pub selection_style: Style,
    pub cursor_style: Style,
    pub error_style: Style,
    pub waring_style: Style,
    pub info_style: Style,
}