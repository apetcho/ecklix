use crate::buffer::{Buffer, BufferMode, Position, WordDirection};
use crate::minibufer::Minibuffer;
use crate::window::{Window, WindowLayout};
use crate::syntax::SyntaxHighlighter;
use crate::shell::Shell;
use crate::elix::ELixRuntime;
use crate::plugins::PluginManager;

use anyhow::Result;
use crossterm::{
    event::{self, Event, KeyCode, KeyEvent, KeyModifiers},
    execute,
    terminal::{
        self, EnterAlternateScreen, LeaveAlternateScreen,
        set_alternate_screen_new, disable_raw_mode, enable_raw_mode
    },
    Command,
};

use std::collections::HashMap;
use std::io::{self, Stdout};
use std::sync::{Arc, RwLock};


pub struct Editor {
    buffers: Vec<Buffer>,
    active_buffer_idx: usize,
    windows: Vec<Window>,
    active_window_idx: usize,
    minibufer: Minibuffer,
    syntax_highlighter: SyntaxHighlighter,
    shell: Shell,
    elix_runtime: ELixRuntime,
    plugin_manager: PluginManager,
    keymap: HashMap<String, Box<dyn Fn(&mut Editor)>>,
    running: bool,
    stdout: Stdout,
}
