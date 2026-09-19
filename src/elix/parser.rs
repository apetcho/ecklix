
use super::types::*;
use crate::elix::EvalError;
use std::iter::Peekable;
use std::str::Chars;

#[derive(Debug, Clone, PartialEq)]
pub enum Form {
    Atom(Value),
    List(Vec<Form>),
    Quoted(Box<Form>),
    Backquoted(Box<Form>),
    BackquotedSplice(Box<Form>),
    Unquote(Box<Form>),
    UnquoteSplice(Box<Form>),
    Progn(Vec<Form>),
    Fun {           // Define
        name: String,
        params: Vec<String>,
        body: Vec<Form>,
    },
    Var {               // Set
        name: String,
        value: Box<Form>,
    },
    Let {
        bindings: Vec<(String, Form)>,
        body: Vec<Form>,
    },
    If {
        condition: Box<Form>,
        then: Box<Form>,
        alt: Option<Box<Form>>, 
    },
    Cond {
        clauses: Vec<(Vec<Form>, Vec<Form>)>,
    },
    Match {
        value: Box<Form>,
        patterns: Vec<(Vec<Form>, Vec<Form>)>,
    },
    For{
        init: Box<Form>,
        condition: Box<Form>,
        update: Box<Form>,
        body: Vec<Form>,
    },
    While{
        condition: Box<Form>,
        body: Vec<Form>,
    },
    Lambda {
        params: Vec<String>,
        body: Vec<Form>,
    },
    MacroDef {
        name: String,
        params: Vec<String>,
        body: Vec<Form>,
    },
    ClassDef {
        name: String,
        parent: Object<String>,
        fields: Vec<String>,
        methods: Vec<(String, Vec<String>, Vec<Form>)>,
    },
    Import {
        package: String,
        symbols: Vec<String>,
    },
    Export {
        symbols: Vec<String>,
    },
    Package {
        name: String,
        imports: Vec<String>,
    },
    And(Vec<Form>),
    Or(Vec<Form>),
    Not(Box<Form>),
    Eval(Box<Form>),
    Case {
        value: Box<Form>,
        branches: Vec<(Vec<Value>, Vec<Form>)>,
    },
    TryCatch {
        body: Box<Form>,
        catch_var: String,
        handler: Vec<Form>,
    },
    With {
        bindings: Vec<(String, Form)>,
        body: Vec<Form>,
    },
    Call {
        func: Box<Form>,
        args: Vec<Form>,
    },
    DotAccess {
        obj: Box<Form>,
        field: String,
    },
    IndexAccess {
        collection: Box<Form>,
        index: Box<Form>,
    },
    Slice {
        collection: Box<Form>,
        start: Option<Box<Form>>,
        end: Option<Box<Form>>,
    },
    Range {
        start: Option<Box<Form>>,
        end: Option<Box<Form>>,
        step: Option<Box<Form>>,
    },
    BinaryOp {
        op: String,
        lhs: Box<Form>,
        rhs: Box<Form>,
    },
    UnaryOp {
        op: String,
        operand: Box<Form>,
    },
    Block {
        forms: Vec<Form>,
    },
    Other(String, Vec<Form>),
}