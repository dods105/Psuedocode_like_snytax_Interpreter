#include "parser.h"

double getInput(){
    double stmt;
    std::cin >> stmt;
    return stmt;
}

class Interpreter {
    using NodeVal = std::variant<double, std::string, bool, TokenType, int, std::vector<int>>;
    std::unordered_map<std::string, NodeVal> vars;

public:
    NodeVal visit(AST* n){
        if(auto x = dynamic_cast<InputNode*>(n)){
            std::cout << std::get<std::string>(visit(x->text));
            std::variant<double, std::string> inputVar;
            std::string inpt;
            getline(std::cin, inpt);

            try{
                double value = std::stod(inpt);
                inputVar = value;
                return std::get<double>(inputVar);
            }
            catch(const std::exception& e){
                inputVar = inpt;
                return std::get<std::string>(inputVar);
            }
        }

        if(auto x = dynamic_cast<NumberNode*>(n)) return x->v;
        if(auto x = dynamic_cast<StringNode*>(n)){
            return x->v;
        }
        if(auto x =dynamic_cast<VarNode*>(n)) return vars[x->n];
        if(auto x =dynamic_cast<AssignNode*>(n)){
            NodeVal val = visit(x->v);
            vars[x->n] = val;
            return val;
        }
        if(auto x =dynamic_cast<BinOpNode*>(n)){
            double a = std::get<double>(visit(x->l));
            double b = std::get<double>(visit(x->r));
            if(x->o.type == TokenType ::PLUS){
                return a+b;
            }
            if(x->o.type == TokenType ::MINUS) {
                return a-b;
            }
            if(x->o.type == TokenType ::STAR) return a*b;
            if(x->o.type == TokenType ::SLASH) return a/b;
            if(x->o.type == TokenType ::GREATER) return a > b;
            if(x->o.type == TokenType ::LESS) return a < b;
            if(x->o.type == TokenType::STRICTEQ) return a == b;
            if (x->o.type == TokenType::NOTEQ) return a != b;
            if (x->o.type == TokenType::LESSEQ) return a <= b;
            if (x->o.type == TokenType::GREATEQ) return a >= b;
        }
        if(auto p = dynamic_cast<PrintNode*>(n)){
            NodeVal val = visit(p->e);
            std::string str;
            if(std::holds_alternative<double>(val)){
                std::cout << std::get<double>(val);
            }else{
                str = std::get<std::string>(val);
                if(str == "\n"){
                    std::cout << std::endl;
                }else{
                    std::cout << str << std::endl;
                }
            }
        }


        if(auto x= dynamic_cast<BlockNode*>(n)){
            for(auto s:x->s){
                visit(s);

            }
        }
        if(auto x=dynamic_cast<IfNode*>(n)){
            if(std::get<bool>(visit(x->c))){
                visit(x->t);
            }else if(x->e) {
                visit(x->e);
            }
        }
        if(auto x = dynamic_cast<WhileNode*>(n)){
            while(std::get<bool>(visit(x->c))){
                visit(x->b);
            }
        }
        if(auto x = dynamic_cast<ForNode*>(n)){
            NodeVal startVal = visit(x->start);
            NodeVal endVal = visit(x->end);
            if(!std::holds_alternative<double>(startVal) || !std::holds_alternative<double>(endVal)){
                throw std::runtime_error("For loops bounds must be numbers");
            }
            double s = std::get<double>(startVal);
            double e = std::get<double>(endVal);

            vars[x->v] = s;

            while(std::get<double>(vars[x->v]) <= e){
                visit(x->body);
                vars[x->v] = std::get<double>(vars[x->v]) + 1;
            }
        }
        return 0.0;
    }
};

//to run this in a .bas file
//cd C:\Users\Lenovo Ryzen 5\Documents\Builing a compiler\MiniLang
//g++ bas_main.cpp -o basic
//basic [bas_file_name.bas]

// ================= MAIN =================
int main(int argc, char* argv[]) {
    std::cout << "=== BASIC Interpreter ===\n";

    if (argc < 2) {
        std::cerr << "Usage: interpreter <file.bas>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Cannot open file: " << argv[1] << "\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    try {
        Lexer lexer(source);
        Parser parser(lexer);
        AST* program = parser.parse();

        Interpreter interpreter;
        interpreter.visit(program);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
