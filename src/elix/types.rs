
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

#[derive(Debug, Clone)]
pub trait Object: std::fmt::Display + Send + Sync {}

#[derive(Debug, Clone)]
pub struct Type {
    pub name: String,
    pub fields: Vec<String>,
    pub methods: HashMap<String, Box<dyn Fn(&mut Value, &[Value]) -> Result<Value, EvalEnv>>>,
}

impl Object for Type {}

/// Implementation of Value
impl Value {
    // pub fn nil(...)
    // pub fn boolean(...)
    // pub fn integer(...)
    // pub fn float(...)
    // pub fn character(...)
    // pub fn string(...)
    // pub fn symbol(...)
    // pub fn keyword(...)
    // pub fn list(...)
    // pub fn array(...)
    // pub fn hashmap(...)
    // pub fn hashset(...)
    // pub fn nil_p(...)
    // pub fn boolean_p(...)
    // pub fn integer_p(...)
    // pub fn float_p(...)
    // pub fn number_p(...)
    // pub fn character_p(...)
    // pub fn string_p(...)
    // pub fn symbol_p(...)
    // pub fn keyword_p(...)
    // pub fn list_p(...)
    // pub fn array_p(...)
    // pub fn hashmap_p(...)
    // pub fn hashset_p(...)
    // pub fn function_p(...)
    // pub fn macro_p(...)
    // pub fn class_p(...)
    // pub fn type_name(...)
    // pub fn display(...)
    // pub fn as_integer(...)
    // pub fn as_float(...)
    // pub fn as_string(...)
    // pub fn as_list(...)
    // pub fn as_array(...)
    // pub fn as_hashmap(...)
}