//#include "frontend/parser.h"
#include "runtime/interpreter.h"

std::string readTextFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

void run() {
    Parser* parser = new Parser();
    Environment* env = createGlobalEnv();

    std::string input = readTextFile("test.txt"); 

    std::cout << "SLO++ v0.1" << std::endl;
    Program program = parser->produceAST(input);
    Statement* stmt = dynamic_cast<Statement*>(&program);
    RuntimeValue* result = evaluate(stmt, env);
    std::cout << "The program has finished. Press ENTER key to exit...";
    std::cin.get();
    std::cout << "Bye";
}

void slopp() {
    Parser parser;
    Environment* env = createGlobalEnv();
    std::cout << "SLO++ v0.1" << std::endl;
    while (true)
    {
        std::string input = "";
        std::cout << ">>> ";
        getline(std::cin, input);
        if(input.empty() || input == "koncaj")
            exit(1);
        
        Program program = parser.produceAST(input);
        // consoleLog(program);

        // std::cout << "\n----------------\n\n";

        Statement* stmt = dynamic_cast<Statement*>(&program);
        RuntimeValue* result = evaluate(stmt, env);
        // consoleLog(program);
        if(dynamic_cast<NumberValue*>(result)) {
            std::cout << dynamic_cast<NumberValue*>(result)->value << std::endl;
        }else if(dynamic_cast<NullValue*>(result)) {
            std::cout << "null" << std::endl;
        }else if(dynamic_cast<BoolValue*>(result)) {
            if (dynamic_cast<BoolValue*>(result)->value){
                std::cout << "true" << std::endl;
            }else{
                std::cout << "false" << std::endl;
            }
        }
        // consoleLog(program);
    }
}

int main() {
   // Total lines of code in .cpp and .h files: 1862
   run();
}