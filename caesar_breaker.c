#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static char decrypt_char(char ch, int shift)
{
    if (isalpha(ch)) {
        char base = islower(ch) ? 'a' : 'A';
        int offset = ch - base;
        offset = (offset - shift + 26) % 26;
        return base + offset;
    }
    return ch;
}

static void decrypt_string(const char *input, int shift)
{
    for (size_t i = 0; input[i] != '\0'; ++i)
        putchar(decrypt_char(input[i], shift));
    putchar('\n');
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ciphertext>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *cipher = argv[1];
    printf("Ciphertext: %s\n\n", cipher);
    printf("Trying all possible Caesar shifts (1‑25):\n");
    for (int shift = 1; shift <= 25; ++shift) {
        printf("Shift %2d: ", shift);
        decrypt_string(cipher, shift);
    }
    return EXIT_SUCCESS;
}
