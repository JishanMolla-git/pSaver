#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <ctime>
#include <Windows.h>

std::string generatePassword(int length)
{
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
    std::string password;
    size_t charactersLength = characters.length();

    for (int i = 0; i < length; ++i)
    {
        password += characters[std::rand() % charactersLength];
    }

    return password;
}

void savePasswords(const std::unordered_map<std::string, std::string>& passwords)
{
    std::ofstream file("passwords-store.txt");

    if (file.is_open())
    {
        for (const auto& entry : passwords)
        {
            file << entry.first << "\n" << entry.second << "\n";
        }

        std::cout << "Passwords saved successfully!\n";
    }
    else
    {
        std::cerr << "Error opening file for writing.\n";
    }

    file.close();
}

void loadPasswords(std::unordered_map<std::string, std::string>& passwords)
{
    std::ifstream file("passwords-store.txt");

    if (file.is_open())
    {
        passwords.clear();

        std::string line;
        while (std::getline(file, line))
        {
            size_t delimiterPos = line.find(',');
            if (delimiterPos != std::string::npos)
            {
                std::string account = line.substr(0, delimiterPos);
                std::string password = line.substr(delimiterPos + 1);
                passwords[account] = password;
            }
        }

        std::cout << "Passwords loaded successfully!\n";
    }
    else
    {
        std::cerr << "Error opening file for reading.\n";
    }

    file.close();
}

void openFile(const std::string& filename)
{
    std::string command = "notepad.exe " + filename;
    system(command.c_str());
}

void copyToClipboard(const std::string& text)
{
    if (OpenClipboard(nullptr))
    {
        EmptyClipboard();

        const int size = (static_cast<int>(text.length()) + 1) * sizeof(wchar_t);
        HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE, size);

        if (hClipboardData != nullptr)
        {
            wchar_t* pchData = static_cast<wchar_t*>(GlobalLock(hClipboardData));
            if (pchData != nullptr)
            {
                MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, pchData, size);
                GlobalUnlock(hClipboardData);
                SetClipboardData(CF_UNICODETEXT, hClipboardData);
            }
        }

        CloseClipboard();
    }
}

int main()
{
    std::unordered_map<std::string, std::string> passwords;
    std::string latestAccount;
    std::string latestPassword;
    std::string retrievedAccount;
    std::string retrievedPassword;

    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed the random number generator

    loadPasswords(passwords);

    std::cout << "Password Manager\n";

    while (true)
    {
        std::cout << "\n1. Generate and Store Password\n";
        std::cout << "2. Retrieve Password\n";
        std::cout << "3. Save Passwords\n";
        std::cout << "4. Open Password File\n";
        std::cout << "5. Copy Latest Account and Password to Clipboard\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        if (choice == 1)
        {
            std::string account;

            std::cout << "Enter the account name: ";
            std::cin >> account;

            int length;
            std::cout << "Enter the password length: ";
            std::cin >> length;

            std::string password = generatePassword(length);
            passwords[account] = password;

            std::cout << "Generated password: " << password << "\n";

            latestAccount = account;
            latestPassword = password;
        }
        else if (choice == 2)
        {
            std::string account;

            std::cout << "Enter the account name: ";
            std::cin >> account;

            if (passwords.find(account) != passwords.end())
            {
                std::cout << "Account: " << account << "\n";
                std::cout << "Password: " << passwords[account] << "\n";

                // Save the retrieved account and password
                retrievedAccount = account;
                retrievedPassword = passwords[account];
            }
            else
            {
                std::cout << "Account not found!\n";
            }
        }
        else if (choice == 3)
        {
            savePasswords(passwords);
        }
        else if (choice == 4)
        {
            openFile("passwords-store.txt");
        }
        else if (choice == 5)
        {
            if (!latestAccount.empty() && !latestPassword.empty())
            {
                std::string clipboardText = latestAccount + " " + latestPassword;
                copyToClipboard(clipboardText);
                std::cout << "Latest account and password copied to clipboard!\n";
            }
            else
            {
                std::cout << "No latest account and password available!\n";
            }
        }
        else if (choice == 6)
        {
            savePasswords(passwords);
            break;
        }
        else
        {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}

