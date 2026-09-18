use anyhow::Result;
use std::collections::HashMap;
use std::fs;
use std::path::{Path, PathBuf};
use std::env;

use crate::elix::{ELixRuntime, Environment, Value, EvalError};
use crate::plugins::PluginManager;
use crate::keymap::Keymap;
use crate::buffer::{Buffer};
use crate::syntax::{SyntaxMode, SyntaxHighlighter};
use crate::shell::Shell;


#[derive(Debug, Clone)]
pub struct InitConfig {
    pub editor_config: EditorConfig,
    pub keybindings: Keymap,
    pub plugins: Vec<String>,
    pub syntax_modes: HashMap<String, SyntaxMode>,
    pub initial_buffers: Vec<(String, String)>,
    pub custom_elix_forms: HashMap<String, Box<dyn Fn(&mut InitConfig)>>,
    pub package_dependencies: Vec<String>,
}

#[derive(Debug, Clone)]
pub struct EditorConfig {
    pub tab_size: usize,
    pub use_tabs: bool,
    pub line_numbers: bool,
    pub status_line: bool,
    pub mode_line: bool,
    pub minimap: bool,
    pub minimap_width: usize,
    pub syntax_highlighting: bool,
    pub auto_ident: bool,
    pub wrap_lines: bool,
    pub search_case_sensitive: bool,
    pub search_whole_word: bool,
    pub max_buffer_size: usize,
    pub undo_history_size: usize,
    pub save_backup: bool,
    pub backup_extension: String,
    pub terminal_width: usize,
    pub terminal_height: usize,
    pub color_scheme: String,
    pub fonts: FontConfig,
    pub mouse_support: bool,
    pub scroll_bar: bool,
    pub scroll_bar_width: usize,
    pub buffer_tabs: bool,
    pub buffer_tab_width: usize,
    pub show_whitespace: bool,
    pub show_trailing_whitespace: bool,
    pub show_line_endings: bool,
}