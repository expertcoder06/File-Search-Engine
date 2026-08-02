# 🔍 File Search Engine

![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Data Structures](https://img.shields.io/badge/Data_Structures-Hash_Maps-ff69b4?style=for-the-badge)
![Algorithms](https://img.shields.io/badge/Algorithms-Inverted_Index-success?style=for-the-badge)

A fast, text-based File Search Engine built in C++. This application indexes text files within a directory and allows users to perform highly efficient keyword searches using an **Inverted Index**. It supports both `AND` and `OR` boolean queries, and results are ranked based on word frequency relevance.

---

## ✨ Features

- **🚀 Fast Keyword Search:** Achieves O(1) average time complexity for word lookups.
- **📂 Directory Indexing:** Recursively traverses directories to index all `.txt` files.
- **🧠 Inverted Index:** Uses nested Hash Maps to store relationships between words, files, and frequencies.
- **🔍 Boolean Queries:**
  - **OR Queries:** Find files containing *any* of the keywords.
  - **AND Queries:** Find files containing *all* of the keywords.
- **🏆 Relevance Ranking:** Search results are sorted in descending order based on the frequency of the matched keywords.
- **⚙️ String Tokenization:** Automatically sanitizes input by removing punctuation and converting to lowercase for accurate matching.

---

## 🛠️ How It Works (Under the Hood)

### 1. Data Structures (Inverted Index)
The core of the search engine is the Inverted Index, implemented using C++ Standard Template Library (STL) unordered maps:
```cpp
std::unordered_map<std::string, std::unordered_map<std::string, int>> invertedIndex;
```
- **Outer Map Key:** The individual keyword (e.g., `"search"`).
- **Inner Map Key:** The file path containing the keyword (e.g., `"./test1.txt"`).
- **Inner Map Value:** The frequency (count) of how many times the keyword appears in that file.

### 2. Algorithms
- **Tokenization:** Reads line by line, splits by spaces/punctuation, and normalizes characters.
- **Intersection (AND Query):** Starts with the document set of the first keyword, then iterates through remaining keywords, keeping only the intersection of documents and accumulating their frequencies.
- **Union (OR Query):** Aggregates the document sets for all keywords and sums up the term frequencies.
- **Sorting:** Results are transferred to a `std::vector<std::pair>` and sorted using `std::sort` based on the frequency score to rank the most relevant files first.

---

## 💻 Installation & Usage

### Prerequisites
- A C++ Compiler (e.g., `g++` / MinGW for Windows, or GCC/Clang for Linux/macOS)

### 1. Clone the repository
```bash
git clone https://github.com/expertcoder06/File-Search-Engine.git
cd File-Search-Engine
```

### 2. Compile the Code
```bash
g++ fse.cpp -o fse.exe
```

### 3. Run the Application
```bash
./fse.exe
```

---

## 🕹️ Command-Line Interface (CLI) Guide

Once the program is running, you can use the following commands:

| Command | Description | Example |
| :--- | :--- | :--- |
| `index <dir>` | Scans and indexes all `.txt` files in the specified directory. | `index .` |
| `or <terms>` | Searches for files containing **any** of the given terms. | `or search engine` |
| `and <terms>` | Searches for files containing **all** of the given terms. | `and file c++` |
| `exit` | Closes the search engine. | `exit` |

### 📝 Example Usage

```text
=== C++ File Search Engine ===

Commands: [index <dir>] | [or <term1> <term2>...] | [and <term1> <term2>...] | [exit]
> index .
Indexed: ./commands.txt
Indexed: ./test1.txt
Indexed: ./test2.txt
Indexing complete!

Commands: [index <dir>] | [or <term1> <term2>...] | [and <term1> <term2>...] | [exit]
> or search engine

Search Results:
File: ./test1.txt | Score: 4
File: ./test2.txt | Score: 3
File: ./commands.txt | Score: 2
```

---

## 🤝 Contributing
Contributions, issues, and feature requests are welcome! 
Feel free to check out the issues page if you want to contribute.

## 📝 License
This project is open-source and available under the [MIT License](LICENSE).
