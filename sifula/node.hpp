// This file is distributed under the BSD License.
// See LICENSE.TXT for details.

#ifndef NODE_HPP_
#define NODE_HPP_

#include <tr1/memory>
#include <vector>
#include <string>

#define NodePtr std::tr1::shared_ptr<Node>

class NodeType {
public: enum Type { IDENTIFIER, NUMBER, OPERATOR, FUNCTION };
};

struct Node {
    std::string text;
    NodeType::Type type;

    //Used by lexer:
    NodePtr next;
    NodePtr prev;

    //Used by parser:
    NodePtr parent;
    std::vector<NodePtr> children;
};

#endif /* NODE_HPP_ */
