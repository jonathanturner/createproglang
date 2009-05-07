// This file is distributed under the BSD License.
// See LICENSE.TXT for details.

#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"
#include "eval.hpp"

int main(int argc, char *argv[]) {
    std::string input;
    std::map<std::string, NodePtr> symbols;

    std::cout << "Expression> ";
    std::getline(std::cin, input);
    while (input != "quit") {
        if (input == "vars") {
            debug_print(symbols);
        }
        else {
            try {
                NodePtr lex_nodes = lex(input);
                clean_whitespace(lex_nodes);
                NodePtr parse_nodes = parse(lex_nodes);
                Result result = eval(parse_nodes, symbols);

                //debug_print(lex_nodes);
                //std::cout << std::endl;
                //debug_print(parse_nodes, "");
                if (result.type == ResultType::NUMBER) {
                    std::cout << result.value << std::endl;
                }
            }
            catch (LexerError &le) {
                std::cerr << "Lexer error: " << le.reason << std::endl;
            }
            catch (ParserError &pe) {
                std::cerr << "Parser error: " << pe.reason << std::endl;
            }
            catch (EvalError &ee) {
                std::cerr << "Eval error: " << ee.reason << std::endl;
            }
        }
        std::cout << "Expression> ";
        std::getline(std::cin, input);
    }
}
