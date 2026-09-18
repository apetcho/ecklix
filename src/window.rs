
#[derive(Debug, Clone)]
pub struct Window {
    pub x: usize,
    pub y: usize,
    pub width: usize,
    pub height: usize,
    pub scroll_x: usize,
    pub scroll_y: usize,
    pub buffer_id: Option<String>,
}

impl Window {
    // pub fn new(...)
    // pub fn resize(...)
    // pub fn set_buffer(...)
    // pub fn get_buffer_id(...)
    // pub fn scroll_up(...)
    // pub fn scroll_down(...)
    // pub fn scroll_left(...)
    // pub fn scroll_right(...)
}

#[derive(Debug, Clone)]
pub enum WindowLayout {
    Horizontal,
    Vertical,
    Tab,
}

impl Default for WindowLayout {
    fn default() -> Self {
        Self::Horizontal
    }
}