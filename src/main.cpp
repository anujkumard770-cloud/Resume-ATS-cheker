
#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

string readFile(const string& path) {
    ifstream file(path, ios::binary);

    if (!file) {
        throw runtime_error("Cannot open file: " + path);
    }

    ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Convert text to lowercase and replace punctuation with spaces.
string normalize(const string& input) {
    string output;
    bool previousSpace = true;

    for (unsigned char ch : input) {
        if (isalnum(ch)) {
            output += static_cast<char>(tolower(ch));
            previousSpace = false;
        } else if (!previousSpace) {
            output += ' ';
            previousSpace = true;
        }
    }

    if (!output.empty() && output.back() == ' ') {
        output.pop_back();
    }

    return output;
}

// Match a complete word or phrase, not just a substring.
bool containsPhrase(
    const string& text,
    const string& phrase
) {
    const string normalizedText =
        " " + normalize(text) + " ";

    const string normalizedPhrase =
        " " + normalize(phrase) + " ";

    return normalizedText.find(normalizedPhrase)
           != string::npos;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: ats_checker.exe "
             << "<resume.txt> <job_description.txt>\n";
        return 1;
    }

    try {
        const string resume = readFile(argv[1]);
        const string jobDescription = readFile(argv[2]);

        // Expand this dictionary as the project grows.
        const vector<string> skillDictionary = {
            "python", "java", "javascript", "typescript",
            "react", "next js", "node js", "express",
            "html", "css", "tailwind css", "sql",
            "postgresql", "mongodb", "git", "github",
            "rest api", "data structures", "algorithms",
            "object oriented programming", "problem solving",
            "unit testing", "docker", "linux",
            "communication", "teamwork"
        };

        const vector<string> sections = {
            "summary", "skills", "education",
            "experience", "projects", "certifications"
        };

        vector<string> requiredSkills;
        vector<string> matchedSkills;
        vector<string> missingSkills;
        vector<string> foundSections;

        // Detect known skills mentioned in the job description.
        for (const string& skill : skillDictionary) {
            if (containsPhrase(jobDescription, skill)) {
                requiredSkills.push_back(skill);

                if (containsPhrase(resume, skill)) {
                    matchedSkills.push_back(skill);
                } else {
                    missingSkills.push_back(skill);
                }
            }
        }

        if (requiredSkills.empty()) {
            cout << "No known skills detected in the job "
                    "description.\n";
            cout << "Add relevant skills to the dictionary "
                    "or check the input file.\n";
            return 1;
        }

        for (const string& section : sections) {
            if (containsPhrase(resume, section)) {
                foundSections.push_back(section);
            }
        }

        const double keywordRate =
            static_cast<double>(matchedSkills.size())
            / requiredSkills.size();

        const double sectionRate =
            static_cast<double>(foundSections.size())
            / sections.size();

        // Illustrative weights, not a real ATS formula.
        const int score = static_cast<int>(
            round(70.0 * keywordRate + 30.0 * sectionRate)
        );

        cout << "\n===== ATS RESUME ANALYSIS =====\n";
        cout << "Heuristic score: " << score << "/100\n";

        cout << "\nMatched skills ("
             << matchedSkills.size() << "):\n";

        for (const string& skill : matchedSkills) {
            cout << "  [MATCH] " << skill << '\n';
        }

        cout << "\nMissing skills ("
             << missingSkills.size() << "):\n";

        for (const string& skill : missingSkills) {
            cout << "  [MISSING] " << skill << '\n';
        }

        cout << "\nDetected resume sections ("
             << foundSections.size() << "/"
             << sections.size() << "):\n";

        for (const string& section : foundSections) {
            cout << "  [FOUND] " << section << '\n';
        }

        cout << "\nNote: This is a rule-based estimate, "
                "not an actual ATS g++ -std=c++17 -Wall -Wextra main.cpp -o ats_checker.exeg++ -std=c++17 -Wall -Wextra main.cpp -o ats_checker.exeresult.\n";

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
