////////////////////////////////////////////////////////////////////////////////////////////

// chapter : Streams

////////////////////////////////////////////////////////////////////////////////////////////

// section : Iterators

////////////////////////////////////////////////////////////////////////////////////////////

// content : Comment Deletion and Blank Line Removal Algorithm

////////////////////////////////////////////////////////////////////////////////////////////

#include <filesystem>
#include <iostream>
#include <iterator>
#include <fstream>
#include <print>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Removes C++ comments and blank lines from a source file, correctly handling literals.
 * 
 * Features:
 * 1. Removes // and /* */ comments.
 * 2. Handles normal string literals "..." and character literals '...'.
 * 3. Handles raw string literals R"delimiter(...)delimiter".
 * 4. Removes lines that are empty or contain only whitespace, UNLESS they are part of a raw string literal.
 * 
 * Raw string literals (C++11) have the form: R"delimiter(raw_characters)delimiter"
 * - They start with R" followed by a delimiter (up to 16 chars, no space/paren/backslash) and a '('.
 * - They end with ')' followed by the same delimiter and a double quote.
 * - They can span multiple lines and contain any characters, including those that look like comments or other literals.
 */
void transform(std::string const & path_1, std::string const & path_2)
{
    std::ifstream file_in(path_1, std::ios::in);
    if (!file_in) return;
    
    // Read entire file into a string
    std::string s((std::istreambuf_iterator<char>(file_in)), std::istreambuf_iterator<char>());

    // Pass 1: Identify all literal ranges to avoid modifying them in subsequent steps.
    struct Range { size_t start, end; };
    std::vector<Range> literals;
    for (size_t i = 0; i < s.length(); )
    {
        if (s[i] == '\'') // Character literal
        {
            size_t start = i++;
            while (i < s.length() && !(s[i] == '\'' && s[i-1] != '\\')) i++;
            if (i < s.length()) i++;
            literals.push_back({start, i});
        }
        else if (s[i] == '\"') // String literal
        {
            size_t start = i;
            bool is_raw = false;
            
            // Check if it's a raw string literal: R"delimiter(
            if (i > 0 && s[i-1] == 'R') 
            {
                size_t open_p = s.find('(', i + 1);
                if (open_p != std::string::npos) 
                {
                    std::string del = s.substr(i + 1, open_p - (i + 1));
                    std::string end_tag = ")" + del + "\"";
                    size_t ep = s.find(end_tag, open_p);
                    if (ep != std::string::npos) 
                    {
                        // Found end of raw string. Identify start including potential prefixes (u8R, LR, etc.)
                        size_t literal_start = i - 1;
                        if (literal_start >= 2 && s.substr(literal_start - 2, 2) == "u8") literal_start -= 2;
                        else if (literal_start >= 1 && (s[literal_start - 1] == 'u' || s[literal_start - 1] == 'U' || s[literal_start - 1] == 'L')) literal_start -= 1;
                        
                        i = ep + end_tag.length();
                        literals.push_back({literal_start, i});
                        is_raw = true;
                    }
                }
            }
            
            if (!is_raw) // Normal string literal
            {
                i++;
                while (i < s.length() && !(s[i] == '\"' && s[i-1] != '\\')) i++;
                if (i < s.length()) i++;
                literals.push_back({start, i});
            }
        }
        else i++;
    }

    // Pass 2: "Soft" remove comments by replacing them with spaces.
    // This preserves indices for Pass 3 while effectively deleting content.
    // We preserve newlines to keep line structure for blank line identification.
    for (size_t i = 0; i < s.length(); )
    {
        // Skip literal regions
        bool in_literal = false;
        for (auto const & r : literals) 
        {
            if (i >= r.start && i < r.end) { i = r.end; in_literal = true; break; }
        }
        if (in_literal) continue;

        if (s[i] == '/' && i + 1 < s.length()) 
        {
            if (s[i+1] == '/') // Line comment
            {
                while (i < s.length() && s[i] != '\n') { s[i] = ' '; i++; }
                continue;
            }
            else if (s[i+1] == '*') // Block comment
            {
                size_t end_p = s.find("*/", i + 2);
                if (end_p != std::string::npos) 
                {
                    for (size_t j = i; j < end_p + 2; ++j) if (s[j] != '\n') s[j] = ' ';
                    i = end_p + 2;
                    continue;
                }
            }
        }
        i++;
    }

    // Pass 3: Construct the final result by filtering out lines that are now blank,
    // making sure we don't remove lines that are part of a literal (like raw string content).
    std::string result;
    size_t last_pos = 0;
    while (last_pos < s.length())
    {
        size_t next_nl = s.find('\n', last_pos);
        if (next_nl == std::string::npos) next_nl = s.length();
        
        std::string line = s.substr(last_pos, next_nl - last_pos);
        
        // A line is blank if it contains only whitespace
        bool is_blank = std::all_of(line.begin(), line.end(), [](unsigned char c) { return std::isspace(c); });
        
        // Check if this line range overlaps any literal (e.g. raw string multi-line content)
        bool is_protected = false;
        for (auto const & r : literals) 
        {
            if (last_pos < r.end && next_nl > r.start) { is_protected = true; break; }
        }
        
        if (!is_blank || is_protected) 
        {
            result += line;
            if (next_nl < s.length()) result += '\n';
        }
        last_pos = next_nl + 1;
    }

    std::ofstream file_out(path_2, std::ios::out);
    file_out << result;
}

////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    auto path_1 = "source.cpp";
    auto path_2 = "output.cpp";

//  ---------------------------------------------------

    transform(path_1, path_2);

//  ---------------------------------------------------

    std::print("main : enter char : "); std::cin.get();

//  ---------------------------------------------------

    std::filesystem::remove(path_2);
}

////////////////////////////////////////////////////////////////////////////////////////////
