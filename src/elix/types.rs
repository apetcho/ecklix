
#[derive(Debug, Clone, PartialEq)]
pub enum Value {
    Nil,
    Bool(bool),
    Integer(i64),
    Float(f64),
    Char(char),
    String(String),
    Symbol(String),
    Keyword(String),
    List(Vec<Value>),
    Array(Vec<Value>),
    HashMap(HashMap<String, Value>),
    HashSet(Vec<Value>),
    Function(String, Vec<String>, Vec<Form>, EnvRef),
    Macros(String, Vec<String>, Vec<Form>, EnvRef),
    Class(String, Vec<String>, HashMap<String, (String, Vec<Value>)>),
    Closure(String, Vec<String>, Vec<Form>, EnvRef),
}

#[derive(Debug, Clone)]
pub struct EnvRef {
    pub id: usize,
    pub data: std::sync::Arc<Env>,
}