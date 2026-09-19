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

// -*-
impl PluginManager {
    // pub fn new(...)
    // pub fn new_with_config(...)
    // pub fn get_plugin_directory(...)
    // pub fn load_plugin(...)
    // pub fn load_all_plugins(...)
    // pub fn unload_plugin(...)
    // pub fn enable_plugin(...)
    // pub fn disable_plugin(...)
    // pub fn is_plugin_loaded(...)
    // pub fn get_plugin(...)
    // pub fn get_plugin_mut(...)
    // pub fn list_installed_plugins(...)
    // pub fn list_loaded_plugins(...)
    // pub fn list_enabled_plugins(...)
    // pub fn install_plugin(...)
    // pub fn uninstall_plugin(...)
    // pub fn get_config(...)
    // pub fn set_config(...)
    // pub fn configure_plugin(...)
    // fn find_plugin(...)
    // fn parse_plugin_metadata(...)
    // fn parse_elix_metadata(...)
    // fn extract_elix_value(...)
    // fn extract_elix_list(...)
    // fn parse_local_plugin_metadata(...)
    // fn find_all_plugins(...)
    // fn load_plugin_implementation(...)
    // fn register_plugin_hooks(...)
    // fn extract_plugin_name(...)
    // fn clone_plugin_to_dir(...)
    // fn get_plugin_path(...)
}

// pub fn create_plugin_template(...)

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;
    use tempfile::TempDir;

    #[test]
    fn test_plugin_manager_creation(){}

    #[test]
    fn test_plugin_metada_parsing(){}

    #[test]
    fn test_plugin_template_creation(){}
}