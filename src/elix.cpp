#include "elix.hpp"

// -*--------------------------------------------------------------------------*-
// -*- begin::namespace::ekasoft::elx                                         -*-
// -*--------------------------------------------------------------------------*-
namespace ekasoft::elx{
// -

/*

// ------------------------------
// -*- ELix : the interpreter -*-
// ------------------------------
class ELix final: public ExprVisitor{
public:
    struct Module{
        UniquePtr<Symbol> name;
        Context ctx;
    };
    
    static std::map<std::string, Vec<std::string>> docs;

    static UniquePtr<Symbol> Quote;
    static UniquePtr<Symbol> Unquote;
    static UniquePtr<Symbol> Quasiquote;
    static UniquePtr<Symbol> UnquoteSplicing;

    //static UniquePtr<Module> mathModule;    // "Math"
    // static Context sysModule;      // "System"
    // static Context fileModule;     // "File"
    // static Context datetimeModule; // "DateTime"
    // static Context jsonModule;     // "JSON"

    static ExprVisitor* visitor;
    static Context prelude;
    explicit ELix(Context ctx);
    Object eval(Expression expr); // evalExprRaw

    static void setup(void);
    static bool is_reserved_word(const std::string& word);
    static void repl(const Vec<Object>& args);
    static void run(const fs::path& scriptpath, const Vec<Object>& args);

    static Context runtime;

private:
    Loader m_loader;

    Object eval(LiteralExpr& expr) override;
    Object eval(SymbolExpr& expr) override;
    Object eval(ListExpr& expr) override;
    Object eval(ArrayExpr& expr) override;
    Object eval(DictExpr& expr) override;
    Object eval(SetExpr& expr) override;

    static void initialize_constructors(void);
    static void initialize_predicates(void);
    static void initialize_operators(void);
    static void initialize_functional(void);
    static void initialize_basic_io(void);
    static void initialize_misc(void);
    static void initialize_string(void);
    static void initialize_symbol(void);
    static void initialize_pair(void);
    static void initialize_array(void);
    static void initialize_list(void);
    static void initialize_dict(void);
    static void initialize_set(void);
    // static void initialize_file(void);
    // static void initialize_datetime(void);
    // static void initialize_json(void);
    // static void initialize_process(void);

    void print(const Object& obj);

    // -*-
    Object handle_progn(Vec<Expression> exprs);
    Object handle_if(Vec<Expression> exprs);
    Object handle_let(Vec<Expression> exprs);
    Object handle_var(Vec<Expression> exprs);
    Object handle_cond(Vec<Expression> exprs);
    Object handle_while(Vec<Expression> exprs);
    Object handle_for(Vec<Expression> exprs);
    Object handle_fun(Vec<Expression> exprs);
    Object handle_macro(Vec<Expression> exprs);
    Expression handle_quote(Vec<Expression> exprs);
    Expression handle_quasiquote(Vec<Expression> exprs);
    Expression handle_unquote(Vec<Expression> exprs);
    Expression handle_unquote_splicing(Vec<Expression> exprs);
    Object handle_list(Vec<Expression> exprs); // function-call

    friend class ModuleLoader;
    friend struct Macro;
};


// -*-
class ModuleLoader{
public:
ModuleLoader::ModuleLoader();

void ModuleLoader::load(const Symbol& name,  Context ctx, Context targetCtx);
void ModuleLoader::load(const fs::path& scritpepath, Context targetCtx);
void ModuleLoader::setup(ELix* elix);

private:
    ELix* m_elix;
    std::map<std::string, Context> m_cache;
};
*/

// -*--------------------------------------------------------------------------*-
}//-*- end::namespace::ekasoft::elx                                           -*-
// -*--------------------------------------------------------------------------*-