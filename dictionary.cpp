#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
using namespace std;

const int TABLE_SIZE = 60689;
const int ALPHABET_SIZE = 26;

class WordNode
{
public:
    string word;
    string meaning;
    WordNode *next;

    WordNode(const string &w, const string &m)
    {
        word = w;
        meaning = m;
        next = nullptr;
    }
};
class TrieNode
{
public:
    TrieNode *children[ALPHABET_SIZE];
    bool EndOfWord;

    TrieNode()
    {
        EndOfWord = false;
        for (int i = 0; i < ALPHABET_SIZE; ++i)
        {
            children[i] = nullptr;
        }
    }
    ~TrieNode()
    {
        for (int i = 0; i < ALPHABET_SIZE; ++i)
        {
            delete children[i];
        }
    }
};

class DictTrie
{
public:
    TrieNode *root;

    DictTrie()
    {
        root = new TrieNode();
    }
    ~DictTrie()
    {
        delete root;
    }

    void insert(const string &word)
    {
        TrieNode *node = root;
        for (char c : word)
        {
            if (isalpha(c)) // chech for special characters
            {
                int index = tolower(c) - 'a';
                if (!node->children[index])
                {
                    node->children[index] = new TrieNode();
                }
                node = node->children[index];
            }
        }
        node->EndOfWord = true;
    }

    bool search(const string &word)
    {
        TrieNode *node = root;
        for (char c : word)
        {
            int index = tolower(c) - 'a';
            if (!node->children[index])
            {
                return false; // word not found
            }
            node = node->children[index];
        }
        return node->EndOfWord;
    }

    void getSuggestionsUtil(TrieNode *node, const string &prefix, string currentWord, int &curr)
    {

        if (node->EndOfWord)
        {
            cout << "Suggestion " << curr + 1 << ": " << currentWord << endl;
            curr++;
        }

        for (int i = 0; i < ALPHABET_SIZE; ++i)
        {
            if (node->children[i])
            {
                getSuggestionsUtil(node->children[i], prefix, currentWord + char('a' + i), curr);
                if (curr >= 10) // termination condition for 10 suggestions
                {
                    return;
                }
            }
        }
    }

    void getSuggestions(const string &prefix)
    {
        TrieNode *node = root;
        string currentWord = "";
        int curr = 0;

        for (char c : prefix) // finds words with the prefix
        {
            int index = tolower(c) - 'a';
            if (!node->children[index])
            {
                return; // not found
            }
            node = node->children[index];
            currentWord += c;
        }
        getSuggestionsUtil(node, prefix, currentWord, curr); // dfs on trie tree to get the suggestions
    }
};

class DictHash
{

    WordNode *table[TABLE_SIZE];

public:
    DictHash()
    {
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            table[i] = nullptr;
        }
    }

    int hashFunction(const string &word)
    {
        int hash = 0;
        for (char c : word)
        {
            hash = (hash * 31 + tolower(c)) % TABLE_SIZE;    //index to store the word
        }
        return hash;
    }

    void insertWord(const string &word, const string &meaning)
    {
        int key = hashFunction(word);
        WordNode *newNode = new WordNode(word, meaning);
        newNode->next = table[key];
        table[key] = newNode;
    }

    void displayMeaning(const string &word)       // gets the meaning of the word
    {
        int key = hashFunction(word);
        WordNode *current = table[key];

        while (current != nullptr)
        {
            if (current->word == word)
            {
                cout << "Meaning for '" << word << "': " << current->meaning << endl;
                return;
            }
            current = current->next;
        }

        cout << "Meaning not found for '" << word << "'" << endl;
    }

    void deleteWord(const string &word)
    {
        int key = hashFunction(word);
        WordNode *current = table[key];
        WordNode *prev = nullptr;

        while (current != nullptr && current->word != word)
        {
            prev = current;
            current = current->next;
        }

        if (current == nullptr)
        {
            cout << "Word not found for deletion: " << word << endl;
            return;
        }

        if (prev == nullptr)
        {
            table[key] = current->next;
        }
        else
        {
            prev->next = current->next;
        }

        cout << "Deleted: Word = " << current->word << ", Meaning = " << current->meaning << " at hash index " << key << endl;
        delete current;
        saveToFile();
    }

    ~DictHash()
    {
        saveToFile();
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            WordNode *current = table[i];
            while (current != nullptr)
            {
                WordNode *next = current->next;
                delete current;
                current = next;
            }
        }
    }

    void addWord(const string &word, const string &meaning)
    {

        string newWord;
        for (char c : word)
        {
            if (isalpha(c))
            {
                newWord += tolower(c);
            }
        }

        int key = hashFunction(newWord);
        WordNode *current = table[key];

        while (current != nullptr)
        {

            if (current->word == newWord)
            {
                cout << "Word already exists in the dictionary." << endl;
                cout << "Do you want to update meaning ? " << endl;
                cout << "Enter 'y' for yes and 'n' for no : " << endl;
                char c;
                cin >> c;
                cin.ignore();

                if (c == 'y')
                {
                    string newMeaning;
                    cout << "Enter new meaning:" << endl;
                    getline(cin, newMeaning);
                    current->meaning = newMeaning;
                    cout << "Word updated successfully!" << endl;

                    ofstream file("Dictionary.txt");
                    for (int i = 0; i < TABLE_SIZE; i++)
                    {
                        WordNode *node = table[i];
                        while (node != nullptr)
                        {
                            file << node->word << " " << node->meaning << endl;
                            node = node->next;
                        }
                    }
                    file.close();
                }
                return;
            }
            current = current->next;
        }

        insertWord(newWord, meaning);
        cout << "Word added successfully!" << endl;
       /* ofstream file("Dictionary.txt", ios::app);
        file << newWord << " " << meaning << endl;
        file.close();*/
        saveToFile();
    }

    void saveToFile()
    {
        ofstream file("Dictionary.txt");
        if (!file.is_open())
        {
            cout << "Error!! Dictionary not saved " << endl;
            sleep(2);
            system("clear");
            return;
        }

        for (int i = 0; i < TABLE_SIZE; i++)
        {
            WordNode *current = table[i];
            while (current != nullptr)
            {
                file << current->word << " " << current->meaning << "\n";
                current = current->next;
            }
        }

        file.close();
        cout << "Dictionary saved successfully!" << endl;
        sleep(2);
        system("clear");
    }
};

void loadDictionary(DictHash &h, DictTrie &trie, string filename)
{
    cout << "Dictionary loading.....\n";
    sleep(1);
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error opening file: " << filename << endl;
        return;
    }

    string line, word, meaning;
    while (getline(file, line))
    {
        istringstream iss(line);
        if (iss >> word >> meaning)
        {
            h.insertWord(word, meaning);
            trie.insert(word);
        }
    }

    file.close();
    cout << "Dictionary loaded successfully!" << endl;
    sleep(2);
    system("clear");
}

void menuFunction(DictTrie &trie, DictHash &h)
{
    cout << "Enter 1 to Insert a word and meaning" << endl;
    cout << "Enter 2 to Delete a word " << endl;
    cout << "Enter 3 to Search a word" << endl;
    cout << "Enter 4 to get suggestions" << endl;
    cout << "Enter 0 to Exit" << endl;

    char choice;
    cin >> choice;
    cin.ignore();
    system("clear");
    switch (choice)
    {
    case '1':
    {
        string word, meaning;
        cout << "Enter word : ";
        getline(cin, word);
        cout << "Enter meaning : ";
        getline(cin, meaning);
        h.addWord(word, meaning);
        sleep(2);
        system("clear");
        break;
    }

    case '2':
    {
        string word;
        cout << "Enter word you want to delete : ";
        getline(cin, word);
        h.deleteWord(word);
        sleep(2);
        system("clear");
        break;
    }

    case '3':
    {
        string word;
        cout << "Enter Text you want to search: ";
        getline(cin, word);
        if (trie.search(word))
        {
            cout << word << " found !" << endl;
            h.displayMeaning(word);
        }
        else
        {
            cout << word << " not found !" << endl;
        }
        sleep(3);
        system("clear");
        break;
    }

    case '4':
    {
        string prefix;
        cout << "Enter the prefix for suggestions: ";
        getline(cin, prefix);
        trie.getSuggestions(prefix);
        sleep(2);
        system("clear");
        break;
    }

    case '0':
        h.saveToFile();
        cout << "Program Ending..." << endl;
        sleep(2);
        return;

    default:
        cout << "Invalid Input" << endl;
        sleep(2);
        system("clear");
        break;
    }

    menuFunction(trie, h);
}

int main()
{
    DictTrie trie;
    DictHash h;
    system("clear");
    loadDictionary(h, trie, "Dictionary.txt");
    menuFunction(trie, h);
    system("clear");
}
