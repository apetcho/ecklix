
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