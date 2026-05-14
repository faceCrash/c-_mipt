#include <iostream>
#include <istream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>
#include <cmath>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

/**
 * Task 12.05 (based on 12.26): Extended Boost Spirit X3 Calculator.
 * 
 * Added features:
 * - Binary operator '%' for modulo.
 * - Binary operator '^' for power.
 * - Unary postfix operator '!' for factorial.
 * - Support for '[]' and '{}' brackets.
 */

namespace ast {
    namespace x3 = boost::spirit::x3;

    struct Sign;
    struct List;
    struct Postfix;

    struct Operand : x3::variant
    <
        double,
        x3::forward_ast<Sign>,
        x3::forward_ast<List>,
        x3::forward_ast<Postfix>
    >
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct Sign {
        char operation;
        Operand operand;
    };

    struct Step {
        char operation;
        Operand operand;
    };

    struct List {
        Operand head;
        std::vector<Step> steps;
    };

    struct Postfix {
        Operand operand;
        std::vector<char> operations;
    };
}

BOOST_FUSION_ADAPT_STRUCT(ast::Sign, operation, operand)
BOOST_FUSION_ADAPT_STRUCT(ast::Step, operation, operand)
BOOST_FUSION_ADAPT_STRUCT(ast::List, head, steps)
BOOST_FUSION_ADAPT_STRUCT(ast::Postfix, operand, operations)

namespace parser {
    namespace x3 = boost::spirit::x3;

    x3::rule<struct expression_tag, ast::List> const expression = "expression";
    x3::rule<struct term_tag, ast::List> const term = "term";
    x3::rule<struct power_tag, ast::List> const power = "power";
    x3::rule<struct unary_tag, ast::Operand> const unary = "unary";
    x3::rule<struct postfix_tag, ast::Postfix> const postfix = "postfix";
    x3::rule<struct primary_tag, ast::Operand> const primary = "primary";

    auto const expression_def =
        term >> *(x3::char_("+") >> term | x3::char_("-") >> term);

    auto const term_def =
        power >> *(x3::char_("*") >> power | x3::char_("/") >> power | x3::char_("%") >> power);

    auto const power_def =
        unary >> *(x3::char_("^") >> unary);

    auto const unary_def =
        (x3::char_("+") >> unary) | (x3::char_("-") >> unary) | postfix;

    auto const postfix_def =
        primary >> *x3::char_('!');

    auto const primary_def =
        x3::double_ |
        ('(' >> expression >> ')') |
        ('[' >> expression >> ']') |
        ('{' >> expression >> '}');

    BOOST_SPIRIT_DEFINE(expression, term, power, unary, postfix, primary);
}

class Calculator {
public:
    double operator()(double x) const { return x; }

    double operator()(ast::Sign const& sign) const {
        double x = boost::apply_visitor(*this, sign.operand);
        switch (sign.operation) {
            case '+': return x;
            case '-': return -x;
        }
        return x;
    }

    double operator()(ast::Step const& step, double x) const {
        double y = boost::apply_visitor(*this, step.operand);
        switch (step.operation) {
            case '+': return x + y;
            case '-': return x - y;
            case '*': return x * y;
            case '/': return (y != 0) ? x / y : 0;
            case '%': return (y != 0) ? std::fmod(x, y) : 0;
            case '^': return std::pow(x, y);
        }
        return x;
    }

    double operator()(ast::List const& list) const {
        double x = boost::apply_visitor(*this, list.head);
        for (auto const& step : list.steps) {
            x = (*this)(step, x);
        }
        return x;
    }

    double operator()(ast::Postfix const& pf) const {
        double x = boost::apply_visitor(*this, pf.operand);
        for (char op : pf.operations) {
            if (op == '!') {
                double res = 1.0;
                for (int i = 2; i <= (int)x; ++i) res *= i;
                x = res;
            }
        }
        return x;
    }
};

auto parse(std::string_view view) {
    ast::List list;
    auto iter = view.begin();
    auto end = view.end();
    bool r = boost::spirit::x3::phrase_parse(iter, end, parser::expression, boost::spirit::x3::ascii::space, list);

    if (r && iter == end) {
        static Calculator calculator;
        return calculator(list);
    }
    return 0.0;
}

int main() {
    std::string string;
    std::cout << "Calculator (Boost Spirit X3) : enter expressions (e.g., 3! + 2^3, [10 % 3]) :\n";

    while (std::getline(std::cin >> std::ws, string, '\n')) {
        if (string == "exit") break;
        std::cout << string << " = " << parse(string) << std::endl;
    }
    return 0;
}
