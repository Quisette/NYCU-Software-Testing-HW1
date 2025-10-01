#include <iostream>
#include <string>
#include <cstring>

struct UserProfile {
    char username[1];
    int user_id;
    char profile_status;
    bool is_active;
    int last_login_year;

    UserProfile() : user_id(0), is_active(true), last_login_year(2025) {
        memset(username, 0, sizeof(username));
        memset(&profile_status, 0, sizeof(profile_status));
    }
};

void printProfile(const UserProfile& profile) {
    std::cout << "--- User Profile Details ---" << std::endl;
    std::cout << "Username: " << profile.username << std::endl;
    std::cout << "User ID: " << profile.user_id << std::endl;
    std::cout << "Status: " << profile.profile_status << std::endl;
    std::cout << "Is Active: " << (profile.is_active ? "Yes" : "No") << std::endl;
    std::cout << "Last Login: " << profile.last_login_year << std::endl;
    std::cout << "----------------------------" << std::endl;
}

void updateUserProfile(UserProfile& profile, const std::string& newUsername) {
    std::cout << "Authenticating user..." << std::endl;
    std::cout << "Updating user profile with new username..." << std::endl;
    strcpy(profile.username, newUsername.c_str());
    std::cout << "Profile update attempt complete." << std::endl;
}

int main() {
    UserProfile my_profile;
    my_profile.user_id = 12345;
    my_profile.profile_status = 'A'; // 'A' for Active
    my_profile.is_active = true;
    my_profile.last_login_year = 2024;

    std::cout << "--- Profile Before Update ---" << std::endl;
    printProfile(my_profile);

    // This string is much longer than the 1-byte buffer for 'username'.
    // It will overflow and overwrite the adjacent struct members on the stack.
    std::string overflow_string = "ThisIsAVeryLongUsernameThatWillCauseABufferOverflow";

    std::cout << "\nAttempting to update with a malicious, oversized username...\n" << std::endl;

    // Trigger the fault.
    updateUserProfile(my_profile, overflow_string);

    std::cout << "\n--- Profile After Malicious Update ---" << std::endl;
    // The values for user_id, is_active, etc., will be corrupted by the overflow.
    printProfile(my_profile);

    std::cout << "\nTest conclusion: Observe how the 'User ID', 'Is Active', and 'Last Login' fields have been overwritten with garbage data from the long username string." << std::endl;

    return 0;
}
