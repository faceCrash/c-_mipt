#include <cctype>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <cmath>

/**
 * Task 12.05 (based on 12.19): Extended Recursive Descent Calculator.
 */

class Stream
{
public :
	using token_t = std::variant < char, double, std::string > ;

	Stream(std::string const & string) : m_stream(string + ';') {}

	auto empty()
	{
		return m_stream.peek() == ';';
	}

	auto get()
	{
		if (m_has_token)
		{
			m_has_token = false;
			return m_token;
		}

		auto x = '\0';
		m_stream >> std::ws >> x;

		if (m_stream.eof()) return token_t(';');

		switch (x)
		{
			case '+' : case '-' : case '*' : case '/' : case '%' : case '^' : case '!' :
			case '(' : case ')' : case '[' : case ']' : case '{' : case '}' : case ';' :
			{
				return token_t(x);
			}

			case '0' : case '1' : case '2' : case '3' : case '4' :
			case '5' : case '6' : case '7' : case '8' : case '9' :
			case '.' :
			{
				m_stream.unget();
				auto y = 0.0;
				m_stream >> y;
				return token_t(y);
			}

			default :
			{
				std::string string(1, x);
				while (m_stream.get(x) && (std::isalpha(x) || std::isdigit(x)))
				{
					string += x;
				}
				if (!std::isspace(x))
				{
					m_stream.unget();
				}
				return token_t(string);
			}
		}
	}

	void put(token_t const & token)
	{
		m_token = token;
		m_has_token = true;
	}

private :
	std::stringstream m_stream;
	token_t m_token;
	bool m_has_token = false;
};

class Calculator
{
public :
	void test()
	{
		std::string string;
		std::cout << "Calculator (Recursive Descent) : enter expressions (e.g., 3! + 2^3, [10 % 3]) :\n";

		while (std::getline(std::cin >> std::ws, string, '\n'))
		{
			if (string == "exit") break;
			Stream stream(string);
			if (!stream.empty())
			{
				try {
					std::cout << string << " = " << statement(stream) << std::endl;
				} catch (const std::exception& e) {
					std::cout << "Error: " << e.what() << std::endl;
				}
			}
		}
	}

private :
	auto factorial(double x) const -> double
	{
		if (x < 0) return 0;
		double res = 1.0;
		for (int i = 2; i <= (int)x; ++i) res *= i;
		return res;
	}

	auto statement(Stream & stream) -> double
	{
		auto token = stream.get();
		if (std::holds_alternative < std::string > (token))
		{
			if (std::get < std::string > (token) == "set")
			{
				return declaration(stream);
			}
		}
		stream.put(token);
		return expression(stream);
	}

	auto declaration(Stream & stream) -> double
	{
		auto token = stream.get();
		if (!std::holds_alternative<std::string>(token)) throw std::runtime_error("Variable name expected");
		auto string = std::get < std::string > (token);
		m_variables[string] = expression(stream);
		return m_variables[string];
	}

	auto expression(Stream & stream) -> double
	{
		auto x = term(stream);
		auto token = stream.get();
		while (true)
		{
			if (!std::holds_alternative<char>(token)) {
				stream.put(token);
				return x;
			}
			char op = std::get<char>(token);
			if (op == '+') x += term(stream);
			else if (op == '-') x -= term(stream);
			else {
				stream.put(token);
				return x;
			}
			token = stream.get();
		}
	}

	auto term(Stream & stream) -> double
	{
		auto x = power_expr(stream);
		auto token = stream.get();
		while (true)
		{
			if (!std::holds_alternative<char>(token)) {
				stream.put(token);
				return x;
			}
			char op = std::get<char>(token);
			if (op == '*') x *= power_expr(stream);
			else if (op == '/') {
				double y = power_expr(stream);
				if (y == 0) throw std::runtime_error("Division by zero");
				x /= y;
			}
			else if (op == '%') {
				double y = power_expr(stream);
				if (y == 0) throw std::runtime_error("Modulo by zero");
				x = std::fmod(x, y);
			}
			else {
				stream.put(token);
				return x;
			}
			token = stream.get();
		}
	}

	auto power_expr(Stream & stream) -> double
	{
		auto x = factorial_expr(stream);
		auto token = stream.get();
		while (true)
		{
			if (!std::holds_alternative<char>(token)) {
				stream.put(token);
				return x;
			}
			if (std::get<char>(token) == '^') x = std::pow(x, factorial_expr(stream));
			else {
				stream.put(token);
				return x;
			}
			token = stream.get();
		}
	}

	auto factorial_expr(Stream & stream) -> double
	{
		auto x = primary(stream);
		auto token = stream.get();
		while (true)
		{
			if (std::holds_alternative<char>(token) && std::get<char>(token) == '!') {
				x = factorial(x);
				token = stream.get();
			} else {
				stream.put(token);
				return x;
			}
		}
	}

	auto primary(Stream & stream) -> double
	{
		auto token = stream.get();

		if (std::holds_alternative < char > (token))
		{
			char op = std::get < char > (token);
			switch (op)
			{
				case '(' :
				{
					auto x = expression(stream);
					stream.get(); 
					return x;
				}
				case '[' :
				{
					auto x = expression(stream);
					stream.get(); 
					return x;
				}
				case '{' :
				{
					auto x = expression(stream);
					stream.get(); 
					return x;
				}
				case '+' : return primary(stream);
				case '-' : return -primary(stream);
			}
		}

		if (std::holds_alternative < double > (token))
		{
			return std::get < double > (token);
		}

		if (std::holds_alternative < std::string > (token)) {
			auto name = std::get < std::string > (token);
			if (m_variables.find(name) != m_variables.end()) return m_variables[name];
			throw std::runtime_error("Undefined variable: " + name);
		}
		
		throw std::runtime_error("Primary expected");
	}

	std::unordered_map < std::string, double > m_variables;
};


int main()
{
	Calculator().test();
	return 0;
}
