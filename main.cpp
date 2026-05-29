#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool isValidTimestamp(string ts) {
    if (ts.length() != 19) return false;
    // check separators: YYYY-MM-DD HH:MM:SS
    if (ts[4] != '-' || ts[7] != '-' || ts[10] != ' ' || ts[13] != ':' || ts[16] != ':')
        return false;
    // check hours, minutes, seconds are in valid range
    int hours = stoi(ts.substr(11, 2));
    int mins  = stoi(ts.substr(14, 2));
    int secs  = stoi(ts.substr(17, 2));
    if (hours > 23 || mins > 59 || secs > 59) return false;
    return true;
}

bool isValidIP(string ip) {
    int dots = 0;
    string part = "";
    for (int i = 0; i <= ip.length(); i++) {
        if (i == ip.length() || ip[i] == '.') {
            if (part.empty()) return false;
            int val = stoi(part);
            if (val > 255) return false;
            part = "";
            dots++;
        } else {
            part += ip[i];
        }
    }
    return dots == 4;
}

int main(int argc, char* argv[]) {
    string filename = "sample.log";
    if (argc >= 2) filename = argv[1];

    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Cannot open file!" << endl;
        return 1;
    }

    int errors = 0;
    int warnings = 0;
    int malformed = 0;
    string line;

    cout << "=== LOG ANALYZER ===" << endl;
    cout << endl;

    while (getline(file, line)) {
        if (line.length() == 0) continue;

        string timestamp = line.substr(0, 19);
        bool goodTimestamp = isValidTimestamp(timestamp);

        bool isError = false;
        bool isWarning = false;

        if (line.find("[ERROR]") != string::npos) isError = true;
        if (line.find("[WARNING]") != string::npos) isWarning = true;

        bool hasIP = false;
        bool goodIP = true;
        string ip = "";

        int start = line.find("(");
        int end = line.find(")");
        if (start != string::npos && end != string::npos) {
            hasIP = true;
            ip = line.substr(start + 1, end - start - 1);
            goodIP = isValidIP(ip);
        }

        if (isError || isWarning) {
            if (!goodTimestamp || !goodIP) {
                cout << "[MALFORMED] " << line << endl;
                if (!goodTimestamp) cout << "  -> bad timestamp" << endl;
                if (!goodIP) cout << "  -> bad IP: " << ip << endl;
                malformed++;
            } else if (isError) {
                cout << "[ERROR] " << timestamp;
                if (hasIP) cout << " (" << ip << ")";
                cout << endl;
                errors++;
            } else {
                cout << "[WARNING] " << timestamp;
                if (hasIP) cout << " (" << ip << ")";
                cout << endl;
                warnings++;
            }
        }
    }

    cout << endl;
    cout << "--- Summary ---" << endl;
    cout << "Errors: " << errors << endl;
    cout << "Warnings: " << warnings << endl;
    cout << "Malformed: " << malformed << endl;

    file.close();
    return 0;
}