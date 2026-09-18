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


impl Editor {
    // pub fn new(...)
    // pub fn run(...)
    // pub fn init_keybindings(...)
    // pub fn poll_input(...)
    // fn handle_key(...)
    // fn render(...)
    // fn draw_mode_line(...)
    // fn draw_status_line(...)
    // fn insert_char(...)
    // fn enter_normal_mode(...)
    // fn enter_insert_mode(...)
    // fn enter_command_mode(...)
    // fn open_minibuffer(...)
    // fn save(...)
    // fn next_buffer(...)
    // fn prev_buffer(...)
    // fn quit(...)
    // fn cleanup(...)

    // Helper methods
    // fn buffer(...)
    // fn buffer_mut(...)
    // fn active_window(...)
}