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