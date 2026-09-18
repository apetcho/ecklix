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


