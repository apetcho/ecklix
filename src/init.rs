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