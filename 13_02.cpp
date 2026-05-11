#include <boost/locale.hpp>
#include <cassert>
#include <fstream>
#include <iostream>
#include <locale>
#include <streambuf>
#include <string>
#include <vector>

using namespace std::literals;

/////////////////////////////////////////////////////////////////////////////////////////////////

class Adapter
{
public :

    Adapter(std::string const & path) : m_stream(path, std::ios::out)
    {
        if (m_stream) 
        {
            m_streambuf = std::cout.rdbuf(m_stream.rdbuf());
        }
    }

//  -----------------------------------------------------------------

   ~Adapter() 
    { 
        if (m_streambuf) 
        {
            std::cout.rdbuf(m_streambuf);
        }
    }

private :

    std::fstream m_stream;
    
    std::streambuf * m_streambuf = nullptr;
};

/////////////////////////////////////////////////////////////////////////////////////////////////

auto make_string_v1(std::string const & string, std::locale const & locale) 
{
    auto size = std::size(string);
    std::vector < wchar_t > vector(size, L'\0');

    std::use_facet < std::ctype < wchar_t > > (locale).widen
    (
        std::data(string), 
        std::data(string) + size, std::data(vector)
    );

    return std::wstring(std::data(vector), size);
}

auto make_string_v2(std::wstring const & wstring, std::locale const & locale)
{
    auto size = std::size(wstring);
    std::vector < char > vector(size, '\0');

    std::use_facet < std::ctype < wchar_t > > (locale).narrow
    (
        std::data(wstring),
        std::data(wstring) + size, '?', std::data(vector)
    );

    return std::string(std::data(vector), size);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Adapter adapter("output.txt");

    // Initialize boost locale
    boost::locale::generator gen;
    auto locale = gen(boost::locale::util::get_system_locale());

    // Open file for reading
    std::fstream file("test_data.txt", std::ios::in);
    if (!file)
    {
        std::cerr << "Error: Could not open test_data.txt\n";
        return 1;
    }

    std::string instruction;
    while (file >> instruction)
    {
        if (instruction == "widen")
        {
            std::string input, expected;
            file >> input >> expected;
            
            auto result = make_string_v1(input, locale);
            // Since we can't easily read wstring from file without complex logic, 
            // we assume 'expected' is provided as a regular string and we widen it for comparison
            auto w_expected = make_string_v1(expected, locale);
            
            assert(result == w_expected);
            std::cout << "Instruction 'widen' passed for input: " << input << "\n";
        }
        else if (instruction == "narrow")
        {
            std::string input, expected;
            file >> input >> expected;
            
            auto w_input = make_string_v1(input, locale);
            auto result = make_string_v2(w_input, locale);
            
            assert(result == expected);
            std::cout << "Instruction 'narrow' passed for input: " << input << "\n";
        }
        else if (instruction == "utf")
        {
            std::string input;
            file >> input; // Read UTF-8 string

            auto method = boost::locale::conv::method_type::skip;

            // Perform sequence of conversions as in 12.05
            auto string_2 = boost::locale::conv::to_utf < char > (input, locale, method);
            auto string_3 = boost::locale::conv::utf_to_utf < char32_t > (string_2, method);
            auto string_4 = boost::locale::conv::utf_to_utf < char > (string_3, method);
            auto string_5 = boost::locale::conv::from_utf < char > (string_4, locale, method);

            assert(string_4 == string_2);
            assert(string_5 == input);

            std::cout << "Instruction 'utf' passed for input: " << input << "\n";
        }
    }

    return 0;
}
