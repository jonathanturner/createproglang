// This file is distributed under the BSD License.
// See LICENSE.TXT for details.

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <map>

#include "node.hpp"

NodePtr parse(NodePtr head);
NodePtr parse_recurse(NodePtr &curr, NodePtr lhs, int min_prec, std::map<std::string, int> &prec_table);
void debug_print(NodePtr node, const std::string &prepend);

struct ParserError {
    std::string reason;

    ParserError(const std::string &why) : reason(why) { }
};

#endif /* PARSER_HPP_ */
