use anyhow::{Result, Context};
use std::collections::HashMap;
use std::io::{self, BufRead, Write};
use std::path::{Path, PathBuf};
use std::process::{Command, Stdio, Child, Output};
use std::sync::{Arc, Mutex};
use std::time::{Duration, Instant};
use std::env;

#[derive(Debug)]
pub struct ShellCommand {
    pub command: String,
    pub args: Vec<String>,
    pub working_dir: Option<String>,
    pub env_vars: HashMap<String, String>,
    pub timeout: Option<Duration>,
}

#[derive(Debug, Clone)]
pub struct ShellResult {
    pub stdout: String,
    pub stderr: String,
    pub exit_code: Option<i32>,
    pub success: bool,
    pub execution_time: Duration,
}


#[derive(Debug, Clone, Default)]
pub struct Shell {
    working_dir: PathBuf,
    environment: HashMap<String, String>,
    history: Vec<String>,
    history_index: usize,
    aliases: HashMap<String, String>,
    process: HashMap<String, Child>,
    process_counter: usize,
}