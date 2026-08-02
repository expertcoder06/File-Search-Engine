#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>
#include <cctype>



class FileSearchEngine {
private:
    // Inverted Index: word -> (filename -> frequency)
    std::unordered_map<std::string, std::unordered_map<std::string, int>> invertedIndex;

    std::string normalizeWord(const std::string& word) {
        std::string normalized = "";
        for (char c : word) {
            if (std::isalnum(c)) {
                normalized += std::tolower(c);
            }
        }
        return normalized;
    }

    std::vector<std::string> tokenize(const std::string& text) {
        std::vector<std::string> tokens;
        std::string currentWord = "";
        for (char c : text) {
            if (std::isspace(c) || std::ispunct(c)) {
                if (!currentWord.empty()) {
                    tokens.push_back(normalizeWord(currentWord));
                    currentWord = "";
                }
            } else {
                currentWord += c;
            }
        }
        if (!currentWord.empty()) {
            tokens.push_back(normalizeWord(currentWord));
        }
        return tokens;
    }

public:
    void indexDirectoryRecursive(const std::string& directoryPath) {
        DIR* dir;
        struct dirent* ent;
        if ((dir = opendir(directoryPath.c_str())) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                std::string fileName = ent->d_name;
                if (fileName == "." || fileName == "..") continue;
                
                std::string fullPath = directoryPath + "/" + fileName;
                
                struct stat st;
                if (stat(fullPath.c_str(), &st) == 0) {
                    if (st.st_mode & S_IFDIR) {
                        indexDirectoryRecursive(fullPath);
                    } else if (st.st_mode & S_IFREG) {
                        if (fileName.length() >= 4 && fileName.substr(fileName.length() - 4) == ".txt") {
                            indexFile(fullPath);
                        }
                    }
                }
            }
            closedir(dir);
        } else {
            std::cerr << "Error accessing directory: " << directoryPath << "\n";
        }
    }

    void indexDirectory(const std::string& directoryPath) {
        invertedIndex.clear();
        indexDirectoryRecursive(directoryPath);
        std::cout << "Indexing complete!\n";
    }

    void indexFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Could not open file: " << filePath << "\n";
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::vector<std::string> words = tokenize(line);
            for (const std::string& word : words) {
                if (!word.empty()) {
                    invertedIndex[word][filePath]++;
                }
            }
        }
        file.close();
        std::cout << "Indexed: " << filePath << "\n";
    }

    // Helper to sort and display results
    void displayResults(const std::unordered_map<std::string, int>& scores) {
        if (scores.empty()) {
            std::cout << "No matching files found.\n";
            return;
        }

        std::vector<std::pair<std::string, int>> sortedResults(scores.begin(), scores.end());
        std::sort(sortedResults.begin(), sortedResults.end(), 
            [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                return a.second > b.second; // Descending order of score
            });

        std::cout << "\nSearch Results:\n";
        for (const auto& result : sortedResults) {
            std::cout << "File: " << result.first << " | Score: " << result.second << "\n";
        }
    }

    void searchOR(const std::vector<std::string>& queryWords) {
        std::unordered_map<std::string, int> scores;

        for (const std::string& word : queryWords) {
            std::string normWord = normalizeWord(word);
            if (invertedIndex.find(normWord) != invertedIndex.end()) {
                for (const auto& fileFreq : invertedIndex[normWord]) {
                    scores[fileFreq.first] += fileFreq.second;
                }
            }
        }
        
        displayResults(scores);
    }

    void searchAND(const std::vector<std::string>& queryWords) {
        if (queryWords.empty()) return;

        std::unordered_map<std::string, int> scores;
        std::vector<std::string> normQueryWords;
        for(const std::string& w: queryWords) {
            normQueryWords.push_back(normalizeWord(w));
        }
        
        std::string firstWord = normQueryWords[0];
        if (invertedIndex.find(firstWord) == invertedIndex.end()) {
             displayResults(scores);
             return;
        }

        for (const auto& fileFreq : invertedIndex[firstWord]) {
            scores[fileFreq.first] = fileFreq.second;
        }

        for (size_t i = 1; i < normQueryWords.size(); ++i) {
            std::string word = normQueryWords[i];
            if (invertedIndex.find(word) == invertedIndex.end()) {
                scores.clear(); 
                break;
            }
            
            std::unordered_map<std::string, int> tempScores;
            for (const auto& fileFreq : invertedIndex[word]) {
                if (scores.find(fileFreq.first) != scores.end()) {
                    tempScores[fileFreq.first] = scores[fileFreq.first] + fileFreq.second;
                }
            }
            scores = tempScores; 
        }

        displayResults(scores);
    }
};

int main() {
    FileSearchEngine engine;
    std::string command;
    std::string input;

    std::cout << "=== C++ File Search Engine ===\n";
    
    while (true) {
        std::cout << "\nCommands: [index <dir>] | [or <term1> <term2>...] | [and <term1> <term2>...] | [exit]\n";
        std::cout << "> ";
        if (!std::getline(std::cin, input)) break;
        
        if (input.empty()) continue;

        std::stringstream ss(input);
        ss >> command;

        if (command == "exit") {
            break;
        } else if (command == "index") {
            std::string dirPath;
            std::getline(ss, dirPath);
            // Trim leading space
            size_t start = dirPath.find_first_not_of(" \t");
            if (start != std::string::npos) {
                dirPath = dirPath.substr(start);
            } else {
                dirPath = "";
            }

            if (dirPath.empty()) {
                std::cout << "Usage: index <directory_path>\n";
            } else {
                engine.indexDirectory(dirPath);
            }
        } else if (command == "or" || command == "and") {
            std::vector<std::string> words;
            std::string word;
            while (ss >> word) {
                words.push_back(word);
            }
            
            if (words.empty()) {
                std::cout << "Usage: " << command << " <term1> [term2] ...\n";
            } else {
                if (command == "or") {
                    engine.searchOR(words);
                } else {
                    engine.searchAND(words);
                }
            }
        } else {
            std::cout << "Unknown command.\n";
        }
    }

    return 0;
}
