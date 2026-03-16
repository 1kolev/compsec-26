#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/* Print a histogram of alphabetic characters in *text* */
void print_histogram(const char *text)
{
    int counts[26] = {0};
    int total_letters = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            int index = toupper(text[i]) - 'A';
            counts[index]++;
            total_letters++;
        }
    }
    printf("\n--- FREQUENCY HISTOGRAM ---\n");
    for (int i = 0; i < 26; i++) {
        if (counts[i] > 0) {
            printf("%c (%3d): ", 'A' + i, counts[i]);
            for (int j = 0; j < counts[i]; j++) {putchar('*');}
            putchar('\n');
        }
    }
    printf("Total letters: %d\n", total_letters);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <text>\n", argv[0]);
        return 1;
    }
    print_histogram(argv[1]);
    return EXIT_SUCCESS;
}
