#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHUNK_SIZE 4096

int vigenere_file(const char *input_path, const char *output_path, const unsigned char *key, int key_len, int encrypt)
{
    FILE *fin = fopen(input_path, "rb");
    FILE *fout = fopen(output_path, "wb");
    if (!fin || !fout)
    {
        perror("fopen");
        if (fin)
            fclose(fin);
        if (fout)
            fclose(fout);
        return -1;
    }

    unsigned char buffer[CHUNK_SIZE];
    size_t bytes_read;
    int key_index = 0;

    while ((bytes_read = fread(buffer, 1, CHUNK_SIZE, fin)) > 0)
    {
        for (size_t i = 0; i < bytes_read; i++)
        {
            int shift = key[key_index % key_len];

            if (encrypt)
                buffer[i] = (unsigned char)((buffer[i] + shift) % 256);
            else
                buffer[i] = (unsigned char)((buffer[i] - shift + 256) % 256);

            key_index++;
        }
        fwrite(buffer, 1, bytes_read, fout);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s <e|d> <input> <output> <key>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int encrypt = (argv[1][0] == 'e');
    const unsigned char *key = (const unsigned char *)argv[4];
    int key_len = strlen(argv[4]);

    if (vigenere_file(argv[2], argv[3], key, key_len, encrypt) == 0)
        printf("%s successful: %s -> %s\n",
               encrypt ? "Encryption" : "Decryption",
               argv[2], argv[3]);
    else
        fprintf(stderr, "Operation failed.\n");

    return EXIT_SUCCESS;
}
// ./vigenere <e|d> <input_file> <output_file> <key>
