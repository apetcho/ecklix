use ropey::Rope;
use std::fmt;
use uuid::Uuid;

#[derive(Debug, Clone)]
pub struct Buffer {
    id: Uuid,
    name: String,
    rope: Rope,
    position: Position,
    modified: bool,
    mode: BufferMode,
    syntax_mode: Option<SyntaxMode>,
}


#[derive(Debug, clone, PartialEq, Eq)]
pub struct Position{
    pub line: usize,
    pub col: usize,
}

#[derive(Debug, Clone, PartialEq, Eq, Default)]
pub enum BufferMode {
    #[default]
    Normal,
    Insert,
    Visual,
    Replace,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum SyntaxMode{
    Rust,
    Lisp,
    Plain,
    Auto,
}

impl Buffer {
    // pub fn new(...)
    // pub fn id(..)
    // pub fn name(...)
    // pub fn set_name(...)
    // pub fn content(...)
    // pub fn position(...)
    // pub fn position_mut(...)
    // pub fn modified(...)
    // pub fn set_modified(...)
    // pub fn mode(...)
    // pub fn set_mode(...)
    // pub fn syntax_mode(...)
    // pub fn set_syntax_mode(...)
    // pub fn line_count(...)
    // pub fn char_len(...)
    // pub fn text_at(...)
    // pub fn insert_char(...)
    // pub fn delete_char(...)
    // pub fn insert_newline(...)
    // pub fn delete_newline(...)
    // pub fn move_position(...)
    // pub fn move_word(...)
    // pub fn delete_to_line_end(...)
    // pub fn delete_to_line_start(...)
    // pub fn move_to_line_start(...)
    // pub fn move_to_line_end(...)
    // pub fn move_to_line(...)
    // pub fn goto_positon(...)
}

/*
impl fmt::Display for Buffer {
    fn fmt(&self, f: mut fmt::Formatter<'_>) -> fmt::Result{}
}
*/

pub trait PositionExt {
    fn to_byte_index(&self, rope: &Rope) -> (usize, usize);
    fn from_byte_index(rope: &Rope, line: usize, col: usize) -> Self;
}