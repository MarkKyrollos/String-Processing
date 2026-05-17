#include "algorithms.h"

#include <chrono>
#include <climits>
#include <unordered_map>


static unordered_map<char, int>
buildBadCharTable(const string& pattern)
{
    unordered_map<char, int> table;
    int m = static_cast<int>(pattern.size());

    for (int i = 0; i < m; ++i)
        table[pattern[i]] = i;

    return table;
}

SearchResult boyerMoore(const string& text, const string& pattern)
{
    SearchResult result;
    result.algorithmName = "Boyer-Moore";
    result.comparisons   = 0;

    auto start = std::chrono::high_resolution_clock::now();

    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());

    if (m == 0 || m > n) {
        result.numMatches = 0;
        result.elapsedMs  = 0.0;
        return result;
    }

    auto badChar = buildBadCharTable(pattern);

    int s = 0; 

    while (s <= n - m) {
        int j = m - 1;   // start from the rightmost character

        // Keep moving j to the left while characters match
        while (j >= 0) {
            ++result.comparisons;
            if (pattern[j] != text[s + j]) break;
            --j;
        }

        if (j < 0) {     
            result.matchPositions.push_back(s);
            s += 1;
        } else {
            char mismatchChar = text[s + j];
            int  bcShift      = 0;

            if (badChar.count(mismatchChar))
                bcShift = j - badChar[mismatchChar];
            else
                bcShift = j + 1;

            s += std::max(1, bcShift);
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dur = end - start;

    result.elapsedMs  = dur.count();
    result.numMatches = static_cast<int>(result.matchPositions.size());
    return result;
}


namespace {
    constexpr long long BASE  = 256;
    constexpr long long MOD   = 101;
}

SearchResult rabinKarp(const string& text, const string& pattern)
{
    SearchResult result;
    result.algorithmName = "Rabin-Karp";
    result.comparisons   = 0;

    auto start = std::chrono::high_resolution_clock::now();

    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());

    if (m == 0 || m > n) {
        result.numMatches = 0;
        result.elapsedMs  = 0.0;
        return result;
    }

    
    long long h = 1;
    for (int i = 0; i < m - 1; ++i)
        h = (h * BASE) % MOD;

    long long patHash  = 0;
    long long winHash  = 0;
    for (int i = 0; i < m; ++i) {
        patHash = (BASE * patHash + pattern[i]) % MOD;
        winHash = (BASE * winHash + text[i])    % MOD;
    }

    for (int s = 0; s <= n - m; ++s) {
        if (patHash == winHash) {
            bool matched = true;
            for (int k = 0; k < m; ++k) {
                ++result.comparisons;
                if (text[s + k] != pattern[k]) { matched = false; break; }
            }
            if (matched)
                result.matchPositions.push_back(s);
        } else {
            ++result.comparisons;
        }

        if (s < n - m) {
            winHash = (BASE * (winHash - text[s] * h) + text[s + m]) % MOD;
            if (winHash < 0) winHash += MOD;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dur = end - start;

    result.elapsedMs  = dur.count();
    result.numMatches = static_cast<int>(result.matchPositions.size());
    return result;
}
