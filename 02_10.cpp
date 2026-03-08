#include <algorithm>
#include <cassert>
#include <cstddef>
#include <numeric>
#include <utility>
#include <vector>
#include <ranges>

////////////////////////////////////////////////////////////////////////////////////

void order(std::vector<int>& vector, std::size_t left, std::size_t right)
{
    for (auto i = left + 1; i < right; ++i)
    {
        for (auto j = i; j > left; --j)
        {
            if (vector[j - 1] > vector[j])
            {
                std::swap(vector[j], vector[j - 1]);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////

std::size_t median_of_three(std::vector<int>& vector, std::size_t a, std::size_t b, std::size_t c)
{
    if ((vector[a] < vector[b] && vector[b] < vector[c]) ||
        (vector[c] < vector[b] && vector[b] < vector[a]))
        return b;

    if ((vector[b] < vector[a] && vector[a] < vector[c]) ||
        (vector[c] < vector[a] && vector[a] < vector[b]))
        return a;

    return c;
}

////////////////////////////////////////////////////////////////////////////////////

std::size_t partition(std::vector<int>& vector, std::size_t left, std::size_t right)
{
    auto mid = std::midpoint(left, right - 1);
    auto pivot_index = median_of_three(vector, left, mid, right - 1);

    auto pivot = vector[pivot_index];
    std::swap(vector[pivot_index], vector[left]);

    std::size_t i = left - 1;
    std::size_t j = right;

    while (true)
    {
        do { ++i; } while (vector[i] < pivot);
        do { --j; } while (vector[j] > pivot);

        if (i >= j)
            return j;

        std::swap(vector[i], vector[j]);
    }
}

////////////////////////////////////////////////////////////////////////////////////

void split(std::vector<int>& vector, std::size_t left, std::size_t right)
{
    if (right - left > 16)
    {
        auto p = partition(vector, left, right);

        split(vector, left,     p + 1);
        split(vector, p + 1,    right);
    }
    else
    {
        order(vector, left, right);
    }
}

////////////////////////////////////////////////////////////////////////////////////

void sort(std::vector<int>& vector)
{
    split(vector, 0, std::size(vector));
}

////////////////////////////////////////////////////////////////////////////////////

int main()
{
    auto size = 1'000uz;

    std::vector<int> vector(size, 0);

    for (auto i = 0uz; i < size; ++i)
    {
        vector[i] = size - i;
    }

    sort(vector);

    assert(std::ranges::is_sorted(vector));
}

////////////////////////////////////////////////////////////////////////////////////
