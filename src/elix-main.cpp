#include "elix.hpp"
#include<iostream>
#include<sstream>


// -*-
ekasoft::elx::Object as_object(const std::string& str){
    if(str=="true"){ return ekasoft::elx::Object(true); }
    if(str=="false"){ return ekasoft::elx::Object(false); }
    if(str=="nil"){ return ekasoft::elx::Object(); }
    bool fixed{true};
    size_t pos{};
    if(str.find('.')!=std::string::npos){ fixed = false; }
    if(str.find('e')!=std::string::npos){ fixed = false; }
    if(str.find('E')!=std::string::npos){ fixed = false; }

    if(fixed){
        auto num = std::stoll(str, &pos);
        if(pos==str.length()){
            return ekasoft::elx::Object(static_cast<std::int64_t>(num));
        }
    }else{
        auto num = std::stod(str, &pos);
        if(pos==str.length()){
            return ekasoft::elx::Object(num);
        }
    }

    auto result = ekasoft::elx::String{str};

    return ekasoft::elx::Object(result);
}

int main(int argc, char **argv){
    /*
        --no-banner
        --version | -v
        --eval | -e
        --help | -h
        filaneme
        --
        ARV
    */
    bool nobanner{false};
    bool version{false};
    bool help{false};
    bool license{false};
    std::string code{};
    std::string filename{};
    std::vector<std::string> args{};
    if(argc > 1){
        for(auto i=1; i < argc; i++){
            args.push_back(std::string(argv[i]));
        }
    }
    //ekasoft::elx::ELix::setup();
    std::vector<ekasoft::elx::Object> ARGV{};
    ARGV.push_back(as_object(std::string(argv[0])));
    for(auto i=0; i < args.size(); i++){
        auto arg = args[i];
        if(arg=="--version" || arg=="-v"){
            version = true;
            continue;
        }
        if(arg=="--license"){
            license = true;
            continue;
        }
        if(arg=="--no-banner"){
            nobanner = true;
            continue;
        }
        if(arg=="--eval" || arg=="-e"){
            if((i+1) >= args.size()){
                std::cerr << "Error: '--eval' or '-e' expect an argument." << std::endl;
                std::exit(EXIT_FAILURE);
            }
            code = args[++i];
        }
        if(arg=="--help" || arg=="-h"){
            help = true;
            continue;
        }
        if(arg.length() > 2 || arg[0]!='-'){
            filename = arg;
            if((i+1) < arg.length()){
                if(args[(i+1)]!=std::string("--")){
                    break;
                }
                auto pos = args.begin()+i+2;
                auto myargs = std::vector<std::string>(pos, args.end());
                for(auto val: myargs){
                    ARGV.push_back(as_object(val));
                }
                break;
            }
        }
    }

    if(!help){
        std::stringstream usage;
        usage << argv[0] << " [OPTIONS]\n";
        usage << argv[0] << " filename [-- args]\n\n";
        usage << "OPTIONS\n";
        usage << "-------\n\n";
        usage << "--help | -h       Print this help\n";
        usage << "--version | -v    Show the version of the interpreter\n";
        usage << "--eval code       Evaluate `code'\n";
        usage << " -e code          Evaluate `code'\n";
        std::cerr << usage.str() << std::endl;
        std::exit(0);
    }

    if(version){
        std::cerr << "ELix VERSION 0.1" << std::endl;
        std::exit(0);
    }

    if(license){
        std::cerr << "ELix MIT License" << std::endl;
        std::exit(0);
    }

    namespace elx = ekasoft::elx;

    // -*-
    struct Runner{
        void run(std::vector<elx::Object> xargv){
            elx::ELix::repl(xargv);
        }

        // -*-
        void run(const std::string& code){
            elx::ELix::setup();
            try{
                elx::ELix runner;
                elx::Tokenizer tokenizer(code);
                elx::Parser parser(tokenizer);
                auto exprs = parser.parse();
                elx::Object result;
                for(auto& expr: exprs){ result = expr->eval(&runner); }
                std::cout << result.str() << std::endl;
                std::exit(0);
            }catch(const elx::ELixError& err){
                std::cerr << err.describe() << std::endl;
                std::exit(EXIT_FAILURE);
            }catch(const std::exception& err){
                std::cerr << err.what() << std::endl;
                std::exit(EXIT_FAILURE);
            }catch(...){
                std::cerr << "Unexpected error occured." << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }

        // -*-
        void run(const fs::path& path, std::vector<elx::Object> eargv){
            elx::ELix::run(fs::path(path), eargv);
        }
    };

    Runner runner;
    if(code.length() > 0){ runner.run(code); }
    else if(filename.length() > 0){
        runner.run(fs::path(filename), ARGV);
    }else{
        runner.run(ARGV);
    }

    return EXIT_SUCCESS;
}