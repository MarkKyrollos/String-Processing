#ifndef UTILS_H
#define UTILS_H

#include "algorithms.h"

#include <string>
#include <vector>


void printBanner(const std::string& title);

void printMenu();


string highlightMatches(const string& text,const string& pattern, const vector<int>& positions);


void printStatistics(const SearchResult& result, const string&  text, const string&  pattern);


void printComparison(const SearchResult& bm, const SearchResult& rk);

void toUpperInPlace(string& s);


vector<string> readPatterns();

#endif
