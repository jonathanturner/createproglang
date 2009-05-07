// This file is distributed under the BSD License.
// See LICENSE.TXT for details.

#ifndef LEXER_HPP_
#define LEXER_HPP_

#include "node.hpp"

NodePtr lex(const std::string &input);
void clean_whitespace(NodePtr head);

void debug_print(const NodePtr head);

struct LexerError {
    std::string reason;

    LexerError(const std::string &why) : reason(why) { }
};

#endif /* LEXER_HPP_ */
