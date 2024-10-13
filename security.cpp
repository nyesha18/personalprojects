#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <openssl/sha.h> // Include OpenSSL for hashing
#include <iomanip>

using namespace std;

// Function to hash a password using SHA-256
string hashPassword(const string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(password.c_str()), password.size(), hash);
    
    // Convert hash to hexadecimal string
    ostringstream oss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        oss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return oss.str();
}

int main() {
    int choice;
    string text, oldPassword, newPassword1, newPassword2, username, inputUser, inputPass, storedUser, storedPass, age;

    cout << "    Security System " << endl;
    cout << "_" << endl << endl;
    cout << "|          1.Register        |" << endl;
    cout << "|          2.Login           |" << endl;
    cout << "|          3.Change Password  |" << endl;
    cout << "|_________ 4.End Program     |" << endl << endl;

    do {
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: { // Registration
                cout << "Please enter username: ";
                cin >> username;
                cout << "Please enter the password: ";
                string password;
                cin >> password;
                cout << "Please enter your age: ";
                cin >> age;

                // Hash the password before storing it
                string hashedPassword = hashPassword(password);

                ofstream outFile("File.txt", ios::app); // Open in append mode
                if (outFile.is_open()) {
                    outFile << username << "," << hashedPassword << "," << age << "\n";
                    cout << "Registration successful!" << endl;
                } else {
                    cerr << "Error opening file for registration." << endl;
                }
                break;
            }
            case 2: { // Login
                cout << "Please enter your username: ";
                cin >> inputUser;
                cout << "Please enter your password: ";
                cin >> inputPass;

                ifstream inFile("File.txt");
                bool loggedIn = false;

                if (inFile.is_open()) {
                    while (getline(inFile, text)) {
                        istringstream iss(text);
                        getline(iss, storedUser, ',');
                        getline(iss, storedPass, ',');
                        getline(iss, age, ',');

                        // Hash the input password for comparison
                        if (inputUser == storedUser && hashPassword(inputPass) == storedPass) {
                            cout << "---Login successful---" << endl;
                            cout << "Username: " << storedUser << endl;
                            cout << "Age: " << age << endl;
                            loggedIn = true;
                            break;
                        }
                    }

                    if (!loggedIn) {
                        cout << "Incorrect credentials." << endl;
                    }
                } else {
                    cerr << "Error opening file for login." << endl;
                }
                break;
            }
            case 3: { // Change Password
                cout << "Enter your username: ";
                cin >> inputUser;
                cout << "Enter your old password: ";
                cin >> oldPassword;

                ifstream inFileChange("File.txt");
                bool passwordChanged = false;
                string tempFileName = "Temp.txt";
                ofstream tempFile(tempFileName); // Temporary file for updating records

                if (inFileChange.is_open() && tempFile.is_open()) {
                    while (getline(inFileChange, text)) {
                        istringstream iss(text);
                        getline(iss, storedUser, ',');
                        getline(iss, storedPass, ',');
                        getline(iss, age, ',');

                        // Check if the user exists and the old password matches
                        if (inputUser == storedUser && hashPassword(oldPassword) == storedPass) {
                            cout << "Enter your new password: ";
                            cin >> newPassword1;
                            cout << "Enter your new password again: ";
                            cin >> newPassword2;

                            if (newPassword1 == newPassword2) {
                                // Update the password in the temporary file
                                tempFile << storedUser << "," << hashPassword(newPassword1) << "," << age << "\n";
                                cout << "Password change successful!" << endl;
                                passwordChanged = true;
                            } else {
                                // If passwords do not match, retain the old password
                                tempFile << storedUser << "," << storedPass << "," << age << "\n";
                                cout << "Passwords do not match!" << endl;
                            }
                        } else {
                            // Write back the unchanged record
                            tempFile << storedUser << "," << storedPass << "," << age << "\n";
                        }
                    }

                    inFileChange.close();
                    tempFile.close();

                    // Replace the old file with the new one
                    remove("File.txt");
                    rename(tempFileName.c_str(), "File.txt");
                } else {
                    cerr << "Error opening file for changing password." << endl;
                }
                break;
            }
            case 4: { // End Program
                cout << "Thank you!" << endl;
                break;
            }
            default:
                cout << "Enter a valid choice." << endl;
        }
    } while (choice != 4);

    return 0;
}
