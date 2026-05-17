#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <string>
#include <vector>


struct SearchResult {
    vector<int> matchPositions;
    long long comparisons;    
    double elapsedMs;       
    int numMatches;      
    string algorithmName;
};


SearchResult boyerMoore(const string& text, const string& pattern);

SearchResult rabinKarp(const string& text, const string& pattern);

#endif
