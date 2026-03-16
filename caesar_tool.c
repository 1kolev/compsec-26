#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

static char shift_char(char ch, int shift, int encrypt)
{
    if (isalpha(ch)) {
        char base = islower(ch) ? 'a' : 'A';
        int offset = ch - base;
        if (encrypt) {offset = (offset + shift) % 26;} 
        else {offset = (offset - shift + 26) % 26;}
        return base + offset;
    }
    return ch;
}

static void caesar_process(FILE *in, FILE *out, int shift, int encrypt)
{
    int ch;
    while ((ch = fgetc(in)) != EOF) {
        fputc(shift_char((char)ch, shift, encrypt), out);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 5) {
        fprintf(stderr,
                "Usage: %s <-e|-d> <input_file> <key_file> <output_file>\n",argv[0]);
        return EXIT_FAILURE;
    }

    const char *mode= argv[1];
    const char *infile = argv[2];
    const char *keyfile = argv[3];
    const char *outfile = argv[4];

    int encrypt = (strcmp(mode, "-enc") == 0) ? 1 :
                  (strcmp(mode, "-dec") == 0) ? 0 : -1;
    if (encrypt == -1) {
        fprintf(stderr, "Invalid mode: %s. Use -enc for encrypt or -dec for decrypt.\n", mode);
        return EXIT_FAILURE;
    }

    FILE *kf = fopen(keyfile, "r");
    if (!kf) {
        perror("Error opening key file");
        return EXIT_FAILURE;
    }
    int shift;
    if (fscanf(kf, "%d", &shift) != 1) {
        fprintf(stderr, "Error reading integer from key file.\n");
        fclose(kf);
        return EXIT_FAILURE;
    }
    fclose(kf);

    shift = shift % 26;
    if (shift < 0) shift += 26;

    FILE *inf = fopen(infile, "r");
    if (!inf) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }
    FILE *outf = fopen(outfile, "w");
    if (!outf) {
        perror("Error opening output file");
        fclose(inf);
        return EXIT_FAILURE;
    }

    caesar_process(inf, outf, shift, encrypt);
    fclose(inf);
    fclose(outf);
    return EXIT_SUCCESS;
}
