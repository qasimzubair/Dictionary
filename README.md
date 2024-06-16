# C++ Dictionary Application

## Overview
This C++ program serves as a dictionary application with functionalities like word insertion, deletion, search, and providing suggestions based on a given prefix. It utilizes two main data structures: Trie (for efficient prefix-based searching and suggestion generation) and Hash Table (for storing word-meaning pairs).

## Data Structures Used:
1. **Trie (Prefix Tree)**:
   - The `TrieNode` class represents a node in the Trie structure.
   - It's utilized for efficient prefix-based search and suggestion generation.
   - Each node contains an array of pointers to child nodes (one for each alphabet letter) and a boolean flag marking the end of a word.
   - This structure enables quick retrieval of words based on prefixes and suggestion generation.

2. **Hash Table**:
   - The `DictHash` class implements a Hash Table for storing word-meaning pairs.
   - It utilizes a hash function to compute the index for storing each word.
   - Collision resolution is handled by chaining (using linked lists).
   - This structure facilitates fast insertion, deletion, and retrieval of word meanings.

## Description:
- The program allows users to interact with the dictionary through a simple command-line interface.
- It loads a dictionary from a text file on startup and saves any modifications back to the file upon exit.
- Users can add new words along with their meanings, delete existing words, search for specific words, and obtain suggestions based on a prefix.
- The program utilizes both Trie and Hash Table data structures to efficiently manage and access dictionary entries.
- Error handling is implemented for file I/O operations and user inputs to ensure smooth functionality.
- The code is well-structured, with clear separation of concerns and modular design, making it easy to understand and maintain.

## Usage
1. Clone the repository to your local machine.
2. Compile the C++ code using your preferred compiler.
3. Ensure a file named `Dictionary.txt` is present in the same directory as the executable, containing word-meaning pairs separated by spaces or on separate lines.
4. Run the compiled executable.
5. Follow the on-screen instructions to interact with the dictionary.

## Example
```cpp
#include <iostream>
#include "Dictionary.h"

int main() {
    // Instantiate DictTrie and DictHash objects
    DictTrie trie;
    DictHash hashTable;
    
    // Load dictionary from file
    loadDictionary(hashTable, trie, "Dictionary.txt");
    
    // Start menu function for user interaction
    menuFunction(trie, hashTable);

    return 0;
}
