#include <iostream>
#include <map>

#include "node.hpp"
#include "parser.hpp"

void debug_print(NodePtr node, const std::string &prepend) {
    std::cout << prepend << node->text << ": " << node->type << std::endl;
    for (unsigned int i = 0; i < node->children.size(); ++i) {
        debug_print(node->children[i], prepend + "  ");
    }
}

NodePtr parse(NodePtr head) {
    NodePtr curr = head->next;
    NodePtr root = head->next;

    std::map<std::string, int> precedence;
    precedence[" "] = 30;
    precedence["+"] = 20;
    precedence["="] = 10;

    if (curr == NULL) {
        return root;
    }
    else {
        if ((curr->type != NodeType::IDENTIFIER) && (curr->type != NodeType::NUMBER)) {
            throw ParserError("Expected number or identifier, saw: " + curr->text);
        }

        if (curr->next == NULL) {
            return root;
        }
        NodePtr lhs = curr;
        curr = curr->next;

        return parse_recurse(curr, lhs, 0, precedence);
    }
}

NodePtr parse_recurse(NodePtr &curr, NodePtr lhs, int min_prec, std::map<std::string, int> &prec_table) {
    NodePtr op;
    NodePtr rhs;
    int prec;

    while (curr->next != NULL) {
        op = curr;
        if (curr->next == NULL) {
            throw ParserError("Incomplete expression at: " + curr->text);
        }
        prec = prec_table[op->text];

        if (prec < min_prec) {
            return lhs;
        }

        curr = curr->next;
        rhs = curr;
        if (curr->next != NULL) {
            NodePtr peek_op = curr->next;
            int peek_prec = prec_table[peek_op->text];
            curr = curr->next;
            if (peek_prec > prec) {
                rhs = parse_recurse(curr, rhs, peek_prec, prec_table);
            }
        }
        lhs->parent = op;
        rhs->parent = op;
        op->children.push_back(lhs);
        op->children.push_back(rhs);
        lhs = op;
    }

    return lhs;
}

