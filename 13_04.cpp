/////////////////////////////////////////////////////////////////////////////////////////

// chapter : Streams

/////////////////////////////////////////////////////////////////////////////////////////

// section : Filesystem

/////////////////////////////////////////////////////////////////////////////////////////

// content : Regex Directory Filter Algorithm

/////////////////////////////////////////////////////////////////////////////////////////

#include <array>
#include <chrono>
#include <filesystem>
#include <format>
#include <print>
#include <sstream>
#include <string>
#include <regex>
#include <iostream>

/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Returns a character representing the file type.
 */
auto make_type(std::filesystem::file_status const & status)
{
    if (std::filesystem::is_directory   (status)) { return 'd'; }

    if (std::filesystem::is_regular_file(status)) { return 'f'; }

    if (std::filesystem::is_symlink     (status)) { return 'l'; }

    return '?';
}

/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Returns a string representing owner permissions (rwx).
 */
auto make_permissions(std::filesystem::perms permissions) -> std::string
{
    auto lambda = [permissions](auto x, auto y)
    {
        return (permissions & x) == std::filesystem::perms::none ? '-' : y;
    };

    return
    {
        lambda(std::filesystem::perms::owner_read,  'r'),

        lambda(std::filesystem::perms::owner_write, 'w'),

        lambda(std::filesystem::perms::owner_exec,  'x')
    };
}

/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Recursively calculates the size of a directory.
 */
auto size(std::filesystem::path const & path)
{
    auto size = 0uz;

    if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
    {
        for (auto const & entry : std::filesystem::recursive_directory_iterator(path))
        {
            if (!std::filesystem::is_directory(entry.status()))
            {
                size += std::filesystem::file_size(entry);
            }
        }
    }

    return size;
}

/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Formats the size of a file or directory with units (B, K, M, G).
 */
auto size(std::filesystem::directory_entry const & entry)
{
    auto size = 0uz;

    if (std::filesystem::is_regular_file(entry.status()))
    {
        size = std::filesystem::file_size(entry);
    }
    else
    {
        size = ::size(entry.path());
    }

    std::array < char, 4 > array = { 'B', 'K', 'M', 'G' };

    auto i = 0uz;

    while (i++ < 3 && size >= 1 << 10)
    {
        size /= (1 << 10);
    }

    return (std::stringstream() << std::format("{: >4} ({})", size, array[i - 1])).str();
}

/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Displays directory contents filtered by a regex pattern.
 * 
 * Comparison with grep:
 * - This solution uses std::regex for programmatic filtering of directory entries.
 * - Unlike the 'grep' utility, which typically filters lines from a text stream (e.g., 'ls | grep pattern'),
 *   this implementation performs the filtering directly on the filesystem objects before printing.
 * - std::regex supports multiple engines (ECMAScript by default), while grep has its own BRE/ERE variants.
 */
void show(std::filesystem::path const & path, std::regex const & pattern)
{
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
    {
        for (auto const & entry : std::filesystem::directory_iterator(path))
        {
            std::string filename = entry.path().filename().string();

            // Filter entries using std::regex_search
            if (std::regex_search(filename, pattern))
            {
                std::print
                (
                    "show : entry : {} | {} | {} | {} | {}\n",

                    make_type(entry.status()),

                    make_permissions(entry.status().permissions()),

                    size(entry),

                    std::chrono::floor < std::chrono::seconds >
                    (
                        std::chrono::file_clock::to_sys(entry.last_write_time())
                    ),

                    filename
                );
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    std::string pattern_string;
    std::print("main : enter regex : ");
    
    if (!std::getline(std::cin, pattern_string) || pattern_string.empty())
    {
        // Default to showing everything if input is empty or failed
        pattern_string = ".*";
    }

    try
    {
        std::regex pattern(pattern_string);
        show(std::filesystem::current_path(), pattern);
    }
    catch (std::regex_error const & exception)
    {
        std::print("main : regex error : {}\n", exception.what());
        return 1;
    }

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
