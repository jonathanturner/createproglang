// This file is distributed under the BSD License.
// See LICENSE.TXT for details.

#ifndef EVAL_HPP_
#define EVAL_HPP_

#include <map>
#include <string>

#include "node.hpp"

class ResultType {
public: enum Type { NUMBER, STATUS} ;
};

struct Result {
    ResultType::Type type;
    int value;
};

Result eval(NodePtr expression, std::map<std::string, NodePtr> &symbols);

struct EvalError {
    std::string reason;

    EvalError(const std::string &why) : reason(why) { }
};

void debug_print(std::map<std::string, NodePtr> &symbols);

#endif /* EVAL_HPP_ */
