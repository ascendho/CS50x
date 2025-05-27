#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, string argv[]){
    int key = -1;

    if (argc == 2 ){
        string key_str = argv[1];
        int LENGTH = strlen(key_str);

        for (int i = 0; i < LENGTH; i++)
            if (!isdigit(key_str[i])) {
                printf("Usage: ./caesar key\n");
                return 1;
            }

        key = atoi(argv[1]);
    }
    else{
        printf("Usage: ./caesar key\n");
        return 1;
    }

    string plainText = get_string("plaintext: ");
    int LENGTH = strlen(plainText);

    char cipherText[LENGTH+1];

    for (int i=0; i<LENGTH; i++)
        if (isupper(plainText[i]))
            cipherText[i] = (plainText[i] - 'A' + key) % 26 + 'A';
        else if (islower(plainText[i]))
            cipherText[i] = (plainText[i] - 'a' + key) % 26 + 'a';
        else
            cipherText[i] = plainText[i];

    cipherText[LENGTH] = '\0';

    printf("ciphertext: %s\n",cipherText);

    return 0;
}
