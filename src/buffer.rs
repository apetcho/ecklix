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