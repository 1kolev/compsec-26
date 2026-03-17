#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXN 1024

char *vigenere_encrypt(const char *plaintext, const char *key)
{
    int plaintext_len = strlen(plaintext);
    int key_len = strlen(key);
    int key_index = 0;
    char *output = malloc(sizeof(char) * plaintext_len + 1);

    int i = 0;
    while (plaintext[i] != '\0')
    {
        char symbol = plaintext[i];

        if (isalpha(symbol))
        {
            char key_symbol = toupper(key[key_index % key_len]);
            int shift = key_symbol - 'A';

            if (islower(symbol))
            {
                output[i] = ((symbol - 'a' + shift) % 26 + 'a');
            }
            else if (isupper(symbol))
            {
                output[i] = ((symbol - 'A' + shift) % 26 + 'A');
            }
            key_index++;
        }
        else
        {
            output[i] = symbol;
        }
        i++;
    }
    output[plaintext_len] = '\0';
    return output;
}

char *vigenere_decrypt(const char *cypher, const char *key)
{
    int cypher_len = strlen(cypher);
    int key_len = strlen(key);
    int key_index = 0;
    char *output = malloc(sizeof(char) * cypher_len + 1);

    int i = 0;
    while (cypher[i] != '\0')
    {
        char symbol = cypher[i];

        if (isalpha(symbol))
        {
            char key_symbol = toupper(key[key_index % key_len]);
            int shift = key_symbol - 'A';

            if (islower(symbol))
            {
                output[i] = ((symbol - 'a' - shift + 26) % 26 + 'a');
            }
            else if (isupper(symbol))
            {
                output[i] = ((symbol - 'A' - shift + 26) % 26 + 'A');
            }
            key_index++;
        }
        else
        {
            output[i] = symbol;
        }
        i++;
    }
    output[cypher_len] = '\0';
    return output;
}

int main()
{
    char plaintext[MAXN];
    scanf("%s", plaintext);

    char key[MAXN];
    scanf("%s", key);

    char *cypher = vigenere_encrypt(plaintext, key);

    printf("Encrypted: %s\n", cypher);

    char *plaintext2 = vigenere_decrypt(cypher, key);

    printf("Decrypted: %s\n", plaintext2);

    return EXIT_SUCCESS;
}
