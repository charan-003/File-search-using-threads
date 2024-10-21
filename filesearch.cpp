#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <ctime>
#include <omp.h>
using namespace std;


string to_lowercase(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);  // Convert each character to lowercase
    return result;
}

// Function to search for a query string in a specific file (case-insensitive)
vector<string> search_in_file(const string& file_path, const string& search_query) {
    vector<string> matching_lines;  // Store lines that contain the search query
    ifstream file(file_path);
    string line;

    string lower_query = to_lowercase(search_query);  // Convert the search query to lowercase

    // Read the file line by line
    while (getline(file, line)) {
        string lower_line = to_lowercase(line);  // Convert each line to lowercase for comparison
        if (lower_line.find(lower_query) != string::npos) {  // Check if the query is found
            matching_lines.push_back(line);  // Store the original matching line
        }
    }
    return matching_lines;
}


int main() {
    cout << "Welcome to the MultiThreaded File Search Tool!\n";
    cout << "---------------------------------------------\n\n";

    // Prompt the user for the directory path and the keyword to search for
    string directory_path, search_query;
    cout << "Enter the directory path to search:\n> ";
    cin >> directory_path;

    // Check if the directory exists before proceeding
    if (!filesystem::exists(directory_path)) {
        cout << "Error: The directory \"" << directory_path << "\" was not found.\n";
        return 1;  // Exit the program if the directory doesn't exist
    }



    cout << "\nEnter the keyword to search for in the files:\n> ";
    cin >> search_query;

    cout << "\nSearching for \"" << search_query << "\" in all files under " << directory_path << "...\n\n";

    // Start the timer to measure how long the search takes
    clock_t start_time = clock();

    // Gather all file paths from the directory
    vector<string> files_to_search;
    for (const auto& entry : filesystem::recursive_directory_iterator(directory_path)) {
        if (!filesystem::is_directory(entry)) {  // Only add files, not directories
            files_to_search.push_back(entry.path().string());
        }
    }

    int total_files = files_to_search.size();  // Total number of files found in the directory
    vector<pair<string, vector<string>>> search_results;  // Store file names and matching lines

    // Perform the file search in parallel using OpenMP
    #pragma omp parallel
    {
        vector<pair<string, vector<string>>> local_results;  // Thread-local results

        // Divide work across threads
        #pragma omp for nowait
        for (int i = 0; i < total_files; ++i) {
            // Search for the query in the current file
            vector<string> matches = search_in_file(files_to_search[i], search_query);
            if (!matches.empty()) {
                local_results.push_back({files_to_search[i], matches});  // Store file path and matches
            }
        }

        // Combine the results from all threads
        #pragma omp critical  // Ensure only one thread accesses shared results at a time
        search_results.insert(search_results.end(), local_results.begin(), local_results.end());
    }

    // Stop the timer
    clock_t end_time = clock();
    double search_duration = double(end_time - start_time) / CLOCKS_PER_SEC;  // Calculate elapsed time

    // Output search summary
    cout << "Files scanned: " << total_files << "\n";
    cout << "Threads used: " << omp_get_max_threads() << "\n\n";
    cout << "Maximum threads available: " << omp_get_max_threads() << endl;

    // Display the search results
    if (!search_results.empty()) {
        cout << "Found matches for the keyword \"" << search_query << "\" in the following files:\n\n";
        int file_index = 1;
        for (const auto& result : search_results) {
            cout << file_index << ". File: " << result.first << "\n";
            for (const auto& match : result.second) {
                cout << "   " << match << "\n";
            }
            cout << "\n";
            ++file_index;
        }
    } else {
        cout << "No matches found for the keyword \"" << search_query << "\" in any files.\n\n";
    }

    // Display total time taken for the search
    cout << "Search completed in " << search_duration << " seconds.\n";
    cout << "---------------------------------------------\n";

    return 0;
}

// Compile with: 
// g++ -fopenmp -std=c++17 filesearch.cpp -o search 
// g++ -Xpreprocessor -fopenmp -lomp -std=c++17 filesearch.cpp -o search
// g++ -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -lomp -std=c++17 filesearch.cpp -o search
