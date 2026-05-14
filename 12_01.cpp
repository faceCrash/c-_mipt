#include <iostream>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include <algorithm>

/**
 * Task 12.01: Currency converter from RUB to USD with localized I/O.
 * 
 * Requirements:
 * - Use std::locale for stream configuration.
 * - Input locale: ru_RU.utf8.
 * - Output locale: en_US.utf8.
 * - Use std::get_money and std::put_money manipulators.
 * - Use std::stringstream for I/O.
 * - Support RUB symbol before or after the amount.
 */

int main() {
    // 1. Read input from the user
    std::string input;
    std::cout << "Enter amount in RUB (e.g., 'RUB 100,00' or '100,00 RUB'): ";
    if (!std::getline(std::cin, input) || input.empty()) {
        return 0;
    }

    // 2. Pre-process input to support both 'RUB' prefix and suffix.
    // Standard ru_RU locale usually expects the international symbol as a suffix.
    // If 'RUB' is at the start, we move it to the end to satisfy the locale facet.
    std::string rub_symbol = "RUB";
    auto it_r = std::search(input.begin(), input.end(), rub_symbol.begin(), rub_symbol.end());
    if (it_r != input.end()) {
        size_t pos = std::distance(input.begin(), it_r);
        // If "RUB" is near the start (e.g., "RUB 100"), move it to the end.
        if (pos < input.find_first_of("0123456789")) {
             std::string value = input.substr(pos + 3);
             input = value + " RUB";
        }
    }

    // 3. Set up input stringstream with Russian locale
    std::stringstream ss_in(input);
    try {
        ss_in.imbue(std::locale("ru_RU.UTF-8"));
    } catch (const std::runtime_error&) {
        std::cerr << "Error: Locale ru_RU.UTF-8 is not supported on this system." << std::endl;
        return 1;
    }

    // 4. Read the amount using std::get_money.
    // amount_rub is stored in the smallest currency units (kopecks).
    long double amount_rub;
    if (!(ss_in >> std::get_money(amount_rub, true))) {
        std::cerr << "Error: Failed to parse RUB amount. Please use correct format (e.g., 100,00 RUB)." << std::endl;
        return 1;
    }

    // 5. Convert RUB to USD.
    // We'll use a fixed exchange rate for the algorithm implementation.
    const long double exchange_rate = 92.50; // 1 USD = 92.50 RUB
    
    // amount_rub is in kopecks. We need result in cents.
    // (Amount in kopecks / 100) / exchange_rate * 100 = amount_rub / exchange_rate.
    long double amount_usd = amount_rub / exchange_rate;

    // 6. Set up output stringstream with US locale
    std::stringstream ss_out;
    try {
        ss_out.imbue(std::locale("en_US.UTF-8"));
    } catch (const std::runtime_error&) {
        std::cerr << "Error: Locale en_US.UTF-8 is not supported on this system." << std::endl;
        return 1;
    }

    // 7. Write the amount using std::put_money
    ss_out << std::showbase << std::put_money(amount_usd, true);

    // 8. Output the result
    std::cout << "Amount in USD: " << ss_out.str() << std::endl;

    return 0;
}
