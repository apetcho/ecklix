use anyhow::{Context, Result};
use serde::{Deserialize, Serialize};
use std::collections::HashMap;
use std::fs;
use std::path::{Path, PathBuf};
use std::sync::{Arc, Mutex};

#[derive(Debug, Clone, PartialEq, Serialize, Deserialize)]
pub struct PlugingMetadata {
    pub name: String,
    pub version: String,
    pub description: Option<String>,
    pub author: Option<String>,
    pub license: Option<String>,
    pub dependencies: Vec<String>,
    pub enabled: bool,
    pub load_order: i32,
    pub config: HashMap<String, String>,
}

impl Default for PlugingMetadata {
    fn default() -> Self {
        Self {
            name: String::new(),
            version: "0.0.0".to_string(),
            description: None,
            author: None,
            license: None,
            dependencies: Vec::new(),
            enabled: true,
            load_order: 0,
            config: HashMap::new(),
        }
    }
}

#[derive(Debug, Clone)]
pub struct Plugin {
    metadata: PlugingMetadata,
    source_path: PathBuf,
    state: PluginState,
    hooks: HashMap<String, Box<dyn Fn(&mut PluginManager) -> Result<()>>>,
    commands: HashMap<String, Box<dyn Fn(&mut PluginManager, &[String]) -> Result<Value>>>,
}

#[derive(Debug, Clone, PartialEq)]
pub enum PluginState {
    NotLoaded,
    Loading,
    Loaded,
    Error(String),
    Disabled,
}

#[derive(Debug, Clone)]
pub struct PluginManager {
    plugins: HashMap<String, Plugin>,
    loaded_plugins: Vec<String>,
    plugin_dir: PathBuf,
    config: PluginManagerConfig,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct PluginManagerConfig {
    pub auto_load: bool,
    pub load_order_mode: LoadOrderMode,
    pub enable_warning: bool,
    pub verbose_loading: bool,
}

impl Default for PluginManagerConfig {
    fn default() -> Self {
        Self {
            auto_load: true,
            load_order_mode: LoadOrderMode::Dependency,
            enable_warning: true,
            verbose_loading: false,
        }
    }
}

pub type Value = serde_json::Value;

impl Plugin {
    // pub fn new(...)
    // pub fn metadata(...)
    // pub fn metadata_mut(...)
    // pub fn source_path(...)
    // pub fn state(...)
    // pub fn set_state(...)
    // pub fn is_loaded(...)
    // pub fn is_enabled(...)
    // pub fn register_hook(...)
    // pub fn register_command(...)
    // pub fn hooks(...)
    // pub fn commands(...)
    // pub fn get_config(...)
    // pub fn set_config(...)
}