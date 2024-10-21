
# MultiThreaded File Search

Welcome to the **MultiThreaded File Search** tool! This program allows you to efficiently search for a keyword in all the files within a specified directory, leveraging the power of multi-threading using OpenMP for faster performance.

## Overview

The MultiThreaded File Search tool allows users to search for a specified keyword within all files located under a chosen directory. It uses OpenMP to parallelize the file search across multiple threads, providing a faster search experience compared to sequential searches, especially when dealing with a large number of files.

## Features

- **Multi-threaded Search**: Efficient searching using OpenMP for parallel processing.
- **Recursive Directory Traversal**: Automatically searches through all files in subdirectories.
- **Keyword Search**: Finds all occurrences of a given keyword in text files.
- **Search Results Display**: Displays the filenames and matching lines where the keyword was found.
- **Execution Time Measurement**: Provides the total time taken to perform the search.

## Prerequisites

To run and compile this application, you will need the following:

- A C++ compiler that supports C++17 or higher.
- OpenMP library installed for multi-threading support.

1. **Compile the program** using the provided compilation commands (see [Compilation](#compilation)).

2. **Run the program**:

   ```bash
   ./search
   ```

3. **Enter the required inputs**:
   - The directory path where you want to search for files.
   - The keyword you want to search for in the files.

4. The program will search all files in the specified directory and subdirectories, and display the results including the matched lines and the time taken for the search.

## Compilation

### On Linux:

```bash
g++ -fopenmp -std=c++17 filesearch.cpp -o search
```

### On MacOS:

```bash
g++ -Xpreprocessor -fopenmp -lomp -std=c++17 filesearch.cpp -o search
```

Alternatively, if you have `libomp` installed via Homebrew, you can use:

```bash
g++ -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -lomp -std=c++17 filesearch.cpp -o search
```
