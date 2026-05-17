#include "algorithms.h"
#include "utils.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

static string g_text;

static bool textLoaded()
{
    if (g_text.empty()) {
        cout << "\n  [!] No text loaded. Use option 1 or 2 first.\n";
        return false;
    }
    return true;
}


void loadFile()
{
    printBanner("Load Text File");

    cout << "  Enter file name (default: input.txt): ";
    string fname;
    getline(cin, fname);

    if (fname.empty())
        fname = "input.txt";

    ifstream file(fname);

    if (!file.is_open()) {
        cout << "  [!] Cannot open file: " << fname << "\n";
        return;
    }

    // Read entire file into g_text
    ostringstream ss;
    ss << file.rdbuf();
    g_text = ss.str();

    toUpperInPlace(g_text);

    replace(g_text.begin(), g_text.end(), '\n', ' ');
    replace(g_text.begin(), g_text.end(), '\r', ' ');

    cout << "\n  File loaded successfully.\n";
    cout << "  Characters read: " << g_text.size() << "\n";

    cout << "\n  Preview (first 120 chars):\n  ";
    cout << g_text.substr(0, min<int>(120, static_cast<int>(g_text.size())));
    cout << "\n";
}


void manualInput()
{
    printBanner("Enter Text Manually");

    cout << "  Type or paste your text below.\n"
         << "  Enter a blank line to finish:\n\n";

    string line;
    ostringstream ss;
    bool first = true;

    while (true) {
        cout << "  > ";
        getline(cin, line);

        if (line.empty())
            break;

        if (!first)
            ss << ' ';

        ss << line;
        first = false;
    }

    g_text = ss.str();
    toUpperInPlace(g_text);

    if (g_text.empty()) {
        cout << "  [!] No text entered.\n";
        return;
    }

    cout << "\n  Text stored (" << g_text.size() << " characters).\n";
}


enum class Algo { BM, RK };

static void runSearch(Algo algo)
{
    if (!textLoaded())
        return;

    string title = (algo == Algo::BM) ? "Boyer-Moore Search" : "Rabin-Karp Search";

    printBanner(title);

    vector<string> patterns = readPatterns();

    if (patterns.empty()) {
        cout << "  [!] No pattern entered.\n";
        return;
    }

    for (const string& pat : patterns) {

        SearchResult res = (algo == Algo::BM) ? boyerMoore(g_text, pat) : rabinKarp(g_text, pat);

        printStatistics(res, g_text, pat);
    }
}


void boyerMooreSearch()
{
    runSearch(Algo::BM);
}


void rabinKarpSearch()
{
    runSearch(Algo::RK);
}


void compareAlgorithms()
{
    if (!textLoaded())
        return;

    printBanner("Compare Algorithms");

    vector<string> patterns = readPatterns();

    if (patterns.empty()) {
        cout << "  [!] No pattern entered.\n";
        return;
    }

    for (const string& pat : patterns) {

        cout << "\n  Searching for: \"" << pat << "\"\n";

        SearchResult bm = boyerMoore(g_text, pat);
        SearchResult rk = rabinKarp(g_text, pat);

        printStatistics(bm, g_text, pat);
        printStatistics(rk, g_text, pat);

        printComparison(bm, rk);
    }
}


static void runBuiltinTests()
{
    struct TestCase {
        string text;
        string pattern;
        string description;
    };

    vector<TestCase> tests = {
        { "AAAAAAAAA",      "AAA",       "Overlapping matches" },
        { "DATA STRUCTURE", "STRUCTURE", "Single match"        },
        { "HELLO WORLD",    "TEST",      "No match"            },
        { "MISSISSIPPI",    "ISSI",      "Multiple matches"    },
        { "ALGORITHM",      "RITH",      "No match (suffix)"   },
    };

    printBanner("Built-in Test Cases");

    for (auto& tc : tests) {

        cout << "\n  [TEST] " << tc.description << "\n";
        cout << "  Text   : " << tc.text << "\n";
        cout << "  Pattern: " << tc.pattern << "\n";

        SearchResult bm = boyerMoore(tc.text, tc.pattern);
        SearchResult rk = rabinKarp(tc.text, tc.pattern);

        cout << "  BM  -> " << bm.numMatches
             << " match(es), "
             << bm.comparisons
             << " comparisons\n";

        cout << "  RK  -> " << rk.numMatches
             << " match(es), "
             << rk.comparisons
             << " comparisons\n";

        if (!bm.matchPositions.empty()) {
            cout << "  Highlighted: "
                 << highlightMatches(tc.text, tc.pattern, bm.matchPositions)
                 << "\n";
        }
    }

    cout << "\n" << string(58, '=') << "\n";
}


int main()
{
    // Run built-in tests on startup
    runBuiltinTests();

    int choice = 0;

    while (true) {

        printMenu();

        // Robust integer read
        if (!(cin >> choice)) {

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "\n  [!] Invalid input. Please enter a number (1-6).\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {

            case 1:
                loadFile();
                break;

            case 2:
                manualInput();
                break;

            case 3:
                boyerMooreSearch();
                break;

            case 4:
                rabinKarpSearch();
                break;

            case 5:
                compareAlgorithms();
                break;

            case 6:
                cout << "\n  Goodbye!\n\n";
                return 0;

            default:
                cout << "\n  [!] Invalid choice. Please enter 1-6.\n";
        }

        
        cout << "\n  Press ENTER to return to the menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}