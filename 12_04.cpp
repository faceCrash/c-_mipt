#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <algorithm>

/**
 * Task 12.04: Email and Domain Extraction using Regex.
 * 
 * Requirements:
 * - Extract all email addresses and their domains.
 * - Use a simplified email format.
 * - Use a regex group to extract domains.
 * - Use raw string literals for test text.
 * - Based on std::sregex_iterator / std::sregex_token_iterator patterns.
 */

using namespace std::literals;

int main() {
    // 1. Test data using raw string literals
    auto text = R"(
        Contact us at support@example.com or sales@company.org.
        For technical issues, email tech.lead@dev.net.
        Invalid emails: plainaddress, @missinguser.com, user@.com should be ignored.
    )"s;

    // 2. Simplified email regex pattern
    // Group 0: Full email
    // Group 1: Local part (before @) - for internal logic if needed
    // Group 2: Domain part (after @) - as requested
    // Pattern: [sequence of chars] @ [sequence of chars with dots]
    std::regex email_pattern(R"(([a-zA-Z0-9._%+-]+)@([a-zA-Z0-9.-]+\.[a-zA-Z]{2,}))");

    std::cout << "Extracting emails and domains:" << std::endl;
    std::cout << "-------------------------------" << std::endl;

    // 3. Use sregex_iterator to iterate over matches and access groups
    auto matches_begin = std::sregex_iterator(text.begin(), text.end(), email_pattern);
    auto matches_end = std::sregex_iterator();

    for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
        std::smatch match = *i;
        std::string full_email = match.str(0);
        std::string domain = match.str(2); // Group 2 is the domain

        std::cout << "Email:  " << full_email << std::endl;
        std::cout << "Domain: " << domain << std::endl;
        std::cout << "-------------------------------" << std::endl;
    }

    // 4. Example using sregex_token_iterator to extract ONLY domains (group 2)
    // The {2} argument specifies that we want to extract the second capture group.
    std::cout << "\nExtracted domains only (using token iterator):" << std::endl;
    std::vector<std::string> domains;
    std::copy(std::sregex_token_iterator(text.begin(), text.end(), email_pattern, 2),
              std::sregex_token_iterator(),
              std::back_inserter(domains));

    for (const auto& d : domains) {
        std::cout << " - " << d << std::endl;
    }

    return 0;
}
