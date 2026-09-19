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
    pub auto_indent: bool,
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

#[derive(Debug, Clone)]
pub struct FontConfig {
    pub regular: String,
    pub bold: String,
    pub italic: String,
    pub size: f32,
    pub ligatures: bool,
}

impl Default for FontConfig {
    fn default() -> Self {
        Self {
            regular: "monospace".to_string(),
            bold: "monospace-bold".to_string(),
            italic: "monospace-italic".to_string(),
            size: 12.0,
            ligatures: true,
        }
    }
}

impl Default for EditorConfig {
    fn default() -> Self {
        Self {
            tab_size: 4,
            use_tabs: false,
            line_numbers: true,
            status_line: true,
            mode_line: true,
            minimap: true,
            minimap_width: 30,
            syntax_highlighting: true,
            auto_indent: true,
            wrap_lines: false,
            search_case_sensitive: false,
            search_whole_word: false,
            max_buffer_size: 1024 * 1024 * 16,  // 16MB
            undo_history_size: 1000,
            save_backup: true,
            backup_extension: ".bak".to_string(),
            terminal_width: 80,
            terminal_height: 24,
            color_scheme: "default".to_string(),
            fonts: FontConfig::default(),
            mouse_support: true,
            scroll_bar: true,
            scroll_bar_width: 10,
            buffer_tabs: true,
            buffer_tab_width: 15,
            show_whitespace: false,
            show_trailing_whitespace: false,
            show_line_endings: false,
        }
    }
}

pub struct InitContext {
    pub config: InitConfig,
    pub elix_runtime: ELixRuntime,
    pub plugin_manager: PluginManager,
    pub keymap: Keymap,
    pub syntax_highlighter: SyntaxHighlighter,
    pub shell: Shell,
    pub buffers: Vec<Buffer>,
    pub working_directory: PathBuf,
}