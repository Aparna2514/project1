#include "Utility.h"
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

// Generate a new ID by reading last used ID from file
int Utility::generateID() {
    ifstream file("data/id_tracker.txt");
    int lastID = 1000; // Start from 1001

    if (file.is_open()) {
        file >> lastID;
        file.close();
    }

    lastID++;

    ofstream outFile("data/id_tracker.txt");
    outFile << lastID;
    outFile.close();

    return lastID;
}

// Get current date & time in format YYYY-MM-DD_HH-MM-SS
string Utility::getCurrentTimestamp() {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);

    stringstream ss;
    ss << 1900 + ltm->tm_year << "-"
       << setw(2) << setfill('0') << 1 + ltm->tm_mon << "-"
       << setw(2) << setfill('0') << ltm->tm_mday << "_"
       << setw(2) << setfill('0') << ltm->tm_hour << "-"
       << setw(2) << setfill('0') << ltm->tm_min << "-"
       << setw(2) << setfill('0') << ltm->tm_sec;

    return ss.str();
}

// Optional: Clean up extra spaces
string Utility::trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos) return "";

    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}
