//#include "frontend/parser.h"
#include "runtime/interpreter.h"

// Prebere tekst iz datoteke
// Vrne string v katerem se nahaja izvorna koda

std::string readTextFile(const std::string& filename) {
    // ifstream odpre datoteko
    std::ifstream file(filename);

    // Preveri ali je datoteka odprta
    if (!file.is_open()) {
        // Vrne napako v primeru da se datoteka ni odprla
        throw std::runtime_error("Failed to open file: " + filename);
    }

    // Prebere vse podatke iz datoteke
    std::stringstream buffer;
    buffer << file.rdbuf();

    // Zapre datoteko
    file.close();

    // Vrne podatke v string formatu
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
    std::cout << "Program se je koncal. Pritisnite tipko ENTER za izhod...";
    std::cin.get();
    std::cout << "Nasvidenje";
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
   return 0;
}