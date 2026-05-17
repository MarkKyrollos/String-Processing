# Assignment 4 – String Processing
### Boyer-Moore & Rabin-Karp String Matching in C++

---

## Project Structure

```
Assignment4/
├── main.cpp          – Interactive menu + program entry point
├── algorithms.h      – SearchResult struct + algorithm declarations
├── algorithms.cpp    – Boyer-Moore and Rabin-Karp implementations
├── utils.h           – Utility function declarations
├── utils.cpp         – Highlighting, statistics, comparison helpers
├── input.txt         – Sample text file for testing
└── README.md         – This file
```

---

## How to Compile

```bash
g++ -std=c++17 -O2 -o Assignment4 main.cpp algorithms.cpp utils.cpp
```

Requires a C++17-capable compiler (GCC 8+, Clang 7+, MSVC 2017+).

---

## How to Run

```bash
./Assignment4
```

On startup the program automatically runs the five built-in test cases  
so output is visible immediately.

---

## Menu Options

| # | Option | Description |
|---|--------|-------------|
| 1 | Load Text File | Load text from `input.txt` or any file you name |
| 2 | Enter Text Manually | Type or paste your own text |
| 3 | Search Using Boyer-Moore | Search for one or more patterns with BM |
| 4 | Search Using Rabin-Karp | Search for one or more patterns with RK |
| 5 | Compare Algorithms | Run both algorithms and show a side-by-side table |
| 6 | Exit | Quit the program |

---

## Features

### Multiple Pattern Search
At the pattern prompt, enter one pattern per line.  
Leave a **blank line** to start the search.

### Pattern Highlighting
Every match is wrapped in `[brackets]` inside the text:

```
Input:  DATA STRUCTURE COURSE
Output: DATA [STRUCTURE] COURSE
```

### Overlapping Match Support
```
Text:    AAAAAA
Pattern: AAA
Matches: index 0, 1, 2, 3
```

### Statistics Output (after every search)
- Number of matches  
- Match positions (0-based index)  
- Number of character comparisons  
- Execution time in milliseconds  

---

## Algorithm Details

### Boyer-Moore
- Scans the pattern **right-to-left**  
- Uses the **bad-character rule** to skip ahead  
- After a full match, advances by 1 to catch overlapping occurrences  
- Best-case O(n/m), worst-case O(nm)

### Rabin-Karp
- Computes a **rolling hash** (base 256, mod 101) for each window  
- Only does full character comparison when hashes agree  
- Always advances by 1 → naturally catches overlapping matches  
- Average-case O(n + m)

---

## Built-in Test Cases

| Text | Pattern | Expected |
|------|---------|----------|
| AAAAAAAAA | AAA | 7 overlapping matches |
| DATA STRUCTURE | STRUCTURE | 1 match |
| HELLO WORLD | TEST | 0 matches |
| MISSISSIPPI | ISSI | 2 matches (idx 1 & 4) |
| ALGORITHM | RITH | 0 matches |
