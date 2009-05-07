// This file is distributed under the BSD License.
// See LICENSE.TXT for details.

#include <iostream>
#include <map>
#include <string>

#include "eval.hpp"
#include "node.hpp"
#include "lexer.hpp"
#include "parser.hpp"

void debug_print(std::map<std::string, NodePtr> &symbols) {
    for (std::map<std::string, NodePtr>::iterator iter = symbols.begin(), end = symbols.end();
        iter != end; ++iter) {

        std::cout << iter->first << ": " << std::endl;
        debug_print(iter->second, "  ");
        std::cout << std::endl;
    }
}

Result eval_number(NodePtr exp, std::map<std::string, NodePtr> &symbols) {
    Result answer;
    answer.value = atoi(exp->text.c_str());
    answer.type = ResultType::NUMBER;

    return answer;
}

Result eval_identifier(NodePtr exp, std::map<std::string, NodePtr> &symbols) {
    if (symbols.find(exp->text) == symbols.end()) {
        throw EvalError("Can not find identifer: " + exp->text);
    }
    if (symbols[exp->text]->type == NodeType::FUNCTION) {
        throw EvalError("Mismatched number of arguments to function: " + exp->text);
    }
    return eval(symbols[exp->text], symbols);
}

Result eval_add(NodePtr exp, std::map<std::string, NodePtr> &symbols) {
    Result answer;
    answer.value = 0;
    answer.type = ResultType::NUMBER;

    for (unsigned int i = 0; i < exp->children.size(); ++i) {
        Result child = eval(exp->children[i], symbols);
        if (child.type != ResultType::NUMBER) {
            throw EvalError("Expression could not be solved: " + exp->children[i]->text);
        }

        answer.value += child.value;
    }

    return answer;
}

Result eval_equate(NodePtr exp, std::map<std::string, NodePtr> &symbols) {
    Result answer;
    answer.value = 0;
    answer.type = ResultType::STATUS;

    if (exp->children[0]->text == " ") {
        std::vector<NodePtr> args;
        NodePtr node = exp->children[0];
        while (node->text == " ") {
            args.push_back(node->children[1]);
            node = node->children[0];
        }

        NodePtr func(new Node());
        func->type = NodeType::FUNCTION;
        while (args.size() > 0) {
            NodePtr arg = args.back();
            args.pop_back();
            func->children.push_back(arg);
        }

        func->children.push_back(exp->children[1]);

        symbols[node->text] = func;
    }
    else {
        symbols[exp->children[0]->text] = exp->children[1];
    }

    return answer;
}

Result eval_funcall(NodePtr exp, std::map<std::string, NodePtr> &symbols) {

    std::vector<NodePtr> args;
    NodePtr node = exp;
    while (node->text == " ") {
        args.insert(args.begin(), node->children[1]);
        node = node->children[0];
    }

    if (symbols.find(node->text) == symbols.end()) {
        throw EvalError("Can not find identifier: " + node->text);
    }

    NodePtr func = symbols[node->text];
    if (func->type != NodeType::FUNCTION) {
        throw EvalError("Function call on non-function: " + node->text);
    }

    if (args.size() != func->children.size() - 1) {
        throw EvalError("Mismatched number of arguments to function: " + node->text);
    }

    //Set the args we know as expressions, making sure to retain their original values
    std::map<std::string, NodePtr> orig;
    for (unsigned int i = 0; i < args.size(); ++i) {
        orig[func->children[i]->text] = symbols[func->children[i]->text];
        if (args[i]->text == func->children[i]->text) {
            throw EvalError("Variables can't be defined in terms of itself: " + args[i]->text);
        }
        symbols[func->children[i]->text] = args[i];
    }

    Result answer = eval(func->children.back(), symbols);

    //Reset the variables to the original
    for (unsigned int i = 0; i < func->children.size()-1; ++i) {
        if (orig[func->children[i]->text] != NULL) {
            symbols[func->children[i]->text] = orig[func->children[i]->text];
        }
        else {
            symbols.erase(symbols.find(func->children[i]->text));
        }
    }

    return answer;
}

Result eval(NodePtr exp, std::map<std::string, NodePtr> &symbols) {
    switch (exp->type) {
        case (NodeType::OPERATOR): {
            if (exp->text == "+") {
                return eval_add(exp, symbols);
            }
            else if (exp->text == "=") {
                return eval_equate(exp, symbols);
            }
            else if (exp->text == " ") {
                return eval_funcall(exp, symbols);
            }
        }
        break;
        case (NodeType::NUMBER) : {
            return eval_number(exp, symbols);
        }
        break;
        case (NodeType::IDENTIFIER) : {
            return eval_identifier(exp, symbols);
        }
        break;
        default : {

        }
        break;
    }

    return Result();
}
