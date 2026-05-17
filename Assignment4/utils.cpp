#include "utils.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;


void printBanner(const string& title)
{
    const int WIDTH = 58;

    string line(WIDTH, '=');

    int pad = (WIDTH - static_cast<int>(title.size())) / 2;

    if (pad < 0)
        pad = 0;

    cout << "\n" << line << "\n";
    cout << string(pad, ' ') << title << "\n";
    cout << line << "\n";
}

void printMenu()
{
    printBanner("String Matching Algorithms – Assignment 4");

    cout
        << "  1. Load Text File\n"
        << "  2. Enter Text Manually\n"
        << "  3. Search Using Boyer-Moore\n"
        << "  4. Search Using Rabin-Karp\n"
        << "  5. Compare Algorithms\n"
        << "  6. Exit\n"
        << string(58, '-') << "\n"
        << "  Enter your choice: ";
}


string highlightMatches(const string& text, const string& pattern, const vector<int>& positions)
{
    if (positions.empty())
        return text;

    int m = static_cast<int>(pattern.size());

    
    vector<bool> inMatch(text.size(), false);

    for (int pos : positions)
        for (int k = 0; k < m && pos + k < static_cast<int>(text.size()); ++k)
            inMatch[pos + k] = true;

    // Build highlighted string
    string result;

    result.reserve(text.size() + positions.size() * 2);

    bool insideBracket = false;

    for (int i = 0; i < static_cast<int>(text.size()); ++i) {

        if (inMatch[i] && !insideBracket) {
            result += '[';
            insideBracket = true;
        }
        else if (!inMatch[i] && insideBracket) {
            result += ']';
            insideBracket = false;
        }

        result += text[i];
    }

    if (insideBracket)
        result += ']';

    return result;
}


void printStatistics(const SearchResult& result, const string& text, const string& pattern)
{
    const string sep(58, '-');

    cout << "\n" << sep << "\n";
    cout << "  SEARCH RESULTS  –  Algorithm: "
         << result.algorithmName << "\n";

    cout << sep << "\n";

    cout << "  Pattern        : \"" << pattern << "\"\n";
    cout << "  Text length    : "
         << text.size() << " characters\n";

    cout << "  Pattern length : "
         << pattern.size() << " characters\n";

    cout << sep << "\n";

    if (result.numMatches == 0) {

        cout << "  *** No match found ***\n";
    }
    else {

        cout << "  Matches found  : "
             << result.numMatches << "\n";

        cout << "  Match positions:\n";

        for (int pos : result.matchPositions)
            cout << "    -> Index " << pos << "\n";

        cout << "\n  Highlighted text:\n  ";

        cout << highlightMatches(
                    text,
                    pattern,
                    result.matchPositions
                ) << "\n";
    }

    cout << sep << "\n";

    cout << "  Comparisons    : "
         << result.comparisons << "\n";

    cout << "  Execution time : "
         << fixed << setprecision(4)
         << result.elapsedMs << " ms\n";

    cout << sep << "\n";
}



void printComparison(const SearchResult& bm, const SearchResult& rk)
{
    const string sep(58, '=');

    const int W = 22;

    cout << "\n" << sep << "\n";
    cout << "       ALGORITHM COMPARISON\n";
    cout << sep << "\n";

    // Header row
    cout << left
         << "  " << setw(W) << "Metric"
         << setw(W) << "Boyer-Moore"
         << setw(W) << "Rabin-Karp"
         << "\n";

    cout << string(58, '-') << "\n";

    // Matches
    cout << "  " << setw(W) << "Matches"
         << setw(W) << bm.numMatches
         << setw(W) << rk.numMatches
         << "\n";

    // Comparisons
    cout << "  " << setw(W) << "Comparisons"
         << setw(W) << bm.comparisons
         << setw(W) << rk.comparisons
         << "\n";

    // Time
    ostringstream bmTime, rkTime;

    bmTime << fixed << setprecision(4)
           << bm.elapsedMs << " ms";

    rkTime << fixed << setprecision(4)
           << rk.elapsedMs << " ms";

    cout << "  " << setw(W) << "Execution Time"
         << setw(W) << bmTime.str()
         << setw(W) << rkTime.str()
         << "\n";

    cout << sep << "\n";

    // Verdict
    cout << "  VERDICT:\n";

    if (bm.comparisons < rk.comparisons)
        cout << "  -> Boyer-Moore made fewer comparisons.\n";

    else if (rk.comparisons < bm.comparisons)
        cout << "  -> Rabin-Karp made fewer comparisons.\n";

    else
        cout << "  -> Both algorithms made the same number of comparisons.\n";

    if (bm.elapsedMs < rk.elapsedMs)
        cout << "  -> Boyer-Moore was faster.\n";

    else if (rk.elapsedMs < bm.elapsedMs)
        cout << "  -> Rabin-Karp was faster.\n";

    else
        cout << "  -> Both algorithms ran in equal time.\n";

    cout << sep << "\n";
}



void toUpperInPlace(string& s)
{
    transform(
        s.begin(),
        s.end(),
        s.begin(),
        [](unsigned char c) {
            return static_cast<char>(toupper(c));
        }
    );
}

vector<string> readPatterns()
{
    cout << "  Enter pattern(s) — one per line, blank line when done:\n";

    vector<string> patterns;

    string line;

    while (true) {

        cout << "  Pattern: ";

        getline(cin, line);

        if (line.empty())
            break;

        toUpperInPlace(line);

        patterns.push_back(line);
    }

    return patterns;
}