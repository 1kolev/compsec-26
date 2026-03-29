#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLEN      10000
#define MIN_REP     3
#define MAX_FACTORS 64

int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }

int factorize(int n, int factors[], int counts[]) {
    int nf = 0;
    for (int d = 2; d * d <= n; d++) {
        if (n % d == 0) {
            factors[nf] = d; counts[nf] = 0;
            while (n % d == 0) { counts[nf]++; n /= d; }
            nf++;
        }
    }
    if (n > 1) { factors[nf] = n; counts[nf++] = 1; }
    return nf;
}

int clean(const char *in, char *out) {
    int n = 0;
    for (int i = 0; in[i] && n < MAXLEN - 1; i++)
        if (isupper((unsigned char)in[i]))
            out[n++] = toupper((unsigned char)in[i]);
    out[n] = '\0';
    return n;
}

int kasiski_distances(const char *C, int Clen,
                      int distancesArr[], int max_dist) {
    int founddistances = 0;
    char sub[MAXLEN];
    for (int i = 0; i < Clen - MIN_REP; i++) {
        for (int len = MIN_REP; i + len <= Clen; len++) {
            int found = 0;
            for (int j = i + 1; j <= Clen - len; j++) {
                if (strncmp(C + i, C + j, len) == 0) {
                    int dist = j - i;
                    strncpy(sub, C + i, len);
                    sub[len] = '\0';
                    printf("  Repeat %-*s  at %4d and %4d  dist = %d\n",
                           MIN_REP + 4, sub, i, j, dist);
                    if (founddistances < max_dist) distancesArr[founddistances++] = dist;
                    found = 1;
                }
            }
            if (!found) break;
        }
    }
    return founddistances;
}

int best_key_length(const int distancesArr[], int nd, int max_key) {
    int score[256] = {0};
    for (int i = 0; i < nd; i++) {
        int factors[MAX_FACTORS], counts[MAX_FACTORS];
        int nf = factorize(distancesArr[i], factors, counts);
        for (int f = 0; f < nf; f++) {
            int p = factors[f];
            for (int m = p; m <= max_key; m += p)
                score[m]++;
        }
    }
    printf("\n  Most probable key lengths:\n");
    printf("  %-10s %-10s\n", "Length", "Score");
    printf("  %-8s %-8s\n", "--------", "--------");
    for (int k = 2; k <= max_key; k++)
        if (score[k] > 0)
            printf("  %-10d %d\n", k, score[k]);

    int best = 2, best_score = 0;
    for (int k = 2; k <= max_key; k++)
        if (score[k] > best_score) { best_score = score[k]; best = k; }
    return best;
}

//key: CHUSHKA(7) cypher: VOYCPXGQYXWYODVOYKVVDKLLKAYMCYWZMYRYHLVARRQBAZAREFHLCUSGJAMASONVSSLOOKKUAGYNETLXLOOSQSXALBSVVBGSNTJLFAUOAPKJJVDEEANZLFINSUYLKTFHQFAREMPHYVBDGYYVAREUVFVPORUAIJLDRGHNTLRIPKNZLGANSMTLPOTLNZLCTQYGZPDTJLEAUQOTKYJLNTJLMGSNIGYMLVBAKZYLOOITZBALVDUHHVZDAPKNZLSRIYIMUNTJLEAUQOTKYJLNTJLMGSNIGYMLVWATJBXVBWCYXLOBOWNBLOODCYEFPQHVZCDLXTNFNZLUIPNIJKORGKNZLCONKCWYCTQSCYODTJLZAYOSCUXOHSTHVLLOORGPHXVBCGTYFACTJLEAUQOTKYJLNTJLMGSNIGYMLVRONKNZLVIPLUFKZRQAYUADHGCCDSKGGHNVHGN

//key: SAPUN(5) cypher: LHTLVNEGVNFKXMDMITNNLDPQASNSNUWRXPRJBPHXASFOVWTPNQSWCGRKSTHTWRHQNATQYFADTNUWOAXFLOCYOJISARMNICYKUCLVKEIBRJIKYETACEVKQJCRLAIXNONPHQLHTLVNEGVNFKXMDMITNNLDPQALHTWNHTPCAKECXFLHTMVYNPFNLMXXAAGWNNFDIBRYUPLQKRTJYQTWYEAVTLOSNZCFIUXYGSTSUJFACXGZEGCIWRQUACIHKHAEIUGVALHFMPEFVWSPLRZISXRFNTUELHTQVFDBCYDBTBVFDIBRGRRBNJDLUYDTWYEAVTLOSNZCFIUXYGSTSUJFACXGZEGCIWRQUACIHKHAEIUGVALH

//key: LIP(3) cypher: EPTCMPCMHFXEWQTDPXOLTYQCEPTNMAWIGEPTCMPCMHFXEWQTDPXOLTYQCEPTNMAWIGEPTCMPCMHFXEWQTDPXOLTYQCEPTNMAWIG

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s '<CIPHERTEXT>'\n", argv[0]);
        fprintf(stderr, "Example: %s 'LLGHYBUODISPFJQON...'\n", argv[0]);
        return EXIT_FAILURE;
    }

    char C[MAXLEN];
    int  Clen = clean(argv[1], C);

    if (Clen < MIN_REP * 2) {
        fprintf(stderr, "Error: ciphertext too short (need at least %d uppercase letters).\n",
                MIN_REP * 2);
        return EXIT_FAILURE;
    }

    printf("=== Kasiski Analysis ===\n");
    printf("Ciphertext length: %d characters\n\n", Clen);

    int distancesArr[MAXLEN];
    int founddistances = kasiski_distances(C, Clen, distancesArr, MAXLEN);

    if (founddistances == 0) {
        printf("No repeating substrings of length >= %d found.\n", MIN_REP);
        return EXIT_FAILURE;
    }

    int g = distancesArr[0];
    for (int i = 1; i < founddistances; i++) g = gcd(g, distancesArr[i]);
    printf("\n  GCD of all distances: %d\n", g);

    int key_len = best_key_length(distancesArr, founddistances, 20); //Assuming that max key len = 20
    printf("\nMost possible result is %d\n", key_len);

    return EXIT_SUCCESS;
}