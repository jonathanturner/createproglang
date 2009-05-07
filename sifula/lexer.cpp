#include <iostream>

#include "lexer.hpp"

NodePtr lex(const std::string &input) {
    NodePtr head(new Node());
    NodePtr curr = head;

    std::string::const_iterator left, right;  //the left and right hand side of the current token

    left = right = input.begin();

    while (right != input.end()) {
        if ((*left >= '0') && (*left <= '9')) {
            //Start a number parse
            right = left;
            while ((right != input.end()) &&
                    ((*right >= '0') && (*right <= '9'))) {
                ++right;
            }
            NodePtr next(new Node());
            next->text = std::string(left, right);
            next->type = NodeType::NUMBER;
            next->prev = curr;
            curr->next = next;
            curr = next;
        }
        else if ((*left >= 'a') && (*left <='z')) {
            //Start an identifier parse
            right = left;
            while ((right != input.end()) &&
                    ((*right >= 'a') && (*right <= 'z'))) {
                ++right;
            }
            NodePtr next(new Node());
            next->text = std::string(left, right);
            next->type = NodeType::IDENTIFIER;
            next->prev = curr;
            curr->next = next;
            curr = next;
        }
        else if (*left == ' ') {
            //Start a whitespace parse
            right = left;
            while ((right != input.end()) && (*right == ' ')) {
                ++right;
            }
            NodePtr next(new Node());
            next->text = " "; //we really only want one space
            next->type = NodeType::OPERATOR;
            next->prev = curr;
            curr->next = next;
            curr = next;
        }
        else if ((*left == '+') || (*left == '=')) {
            //Start an operator parse
            ++right;

            NodePtr next(new Node());
            next->text = std::string(left, right);
            next->type = NodeType::OPERATOR;
            next->prev = curr;
            curr->next = next;
            curr = next;
        }
        else {
            throw LexerError("Unknown character at: " +
                    std::string(left, right+1));
        }

        left = right;
    }

    return head;
}

void clean_whitespace(NodePtr head) {
    NodePtr curr = head;
    while (curr->next != NULL) {
        curr = curr->next;
        if (curr->text == " ") {
            if (curr->next == NULL) {
                curr->prev->next = NodePtr();
            }
            else {
                if (((curr->prev->type != NodeType::IDENTIFIER) && (curr->prev->type != NodeType::NUMBER)) ||
                        ((curr->next->type != NodeType::IDENTIFIER) && (curr->next->type != NodeType::NUMBER))) {
                    curr->prev->next = curr->next;
                    curr->next->prev = curr->prev;
                }
            }
        }
    }
}

void debug_print(const NodePtr node) {
    NodePtr curr = node;
    while (curr->next != NULL) {
        curr = curr->next;
        std::cout << "[" << curr->text << "]";
        if (curr->next != NULL) std::cout << " -> ";
    }
}
