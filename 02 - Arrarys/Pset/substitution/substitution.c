#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool isValidHc(string s);
bool isValidAlpha(string s);
bool isRepeated(string s);

int main(int argc, string argv[]) {
    if (argc != 2) {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = argv[1];

    if (!isValidHc(key) || !isValidAlpha(key) || isRepeated(key)) {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string plainText = get_string("plaintext: ");
    int LENGTH = strlen(plainText);

    for (int i = 0; i < LENGTH; i++) {
        if (isupper(plainText[i])) {
            plainText[i] = toupper(key[plainText[i] - 'A']);
        } else if (islower(plainText[i])) {
            plainText[i] = tolower(key[plainText[i] - 'a']);
        }
    }

    printf("ciphertext: %s\n", plainText);
    return 0;
}

bool isValidHc(string s) {
    return strlen(s) == 26;
}

bool isValidAlpha(string s) {
    for (int i = 0, len = strlen(s); i < len; i++)
        if (!isalpha(s[i]))
            return false;

    return true;
}

bool isRepeated(string s) {
    for (int i = 0, len = strlen(s); i < len; i++)
        for (int j = i + 1; j < len; j++)
            if (toupper(s[i]) == toupper(s[j]))
                return true;

    return false;
}
