#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

int main() {
    const char *target = "ef797c8118f02dfb649607dd5d3f8c7623048c9c063d532cc95c5ed7a898a64f";
    const char *salt = "2026-";
    char candidate[15];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    char hash_str[65];

    for (int pin = 0; pin < 10000; pin++) {
        sprintf(candidate, "%s%04d", salt, pin);
        
        SHA256((unsigned char*)candidate, strlen(candidate), hash);
        
        for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            sprintf(&hash_str[i*2], "%02x", hash[i]);
        }
        
        if (strcmp(hash_str, target) == 0) {
            printf("Found PIN: %04d", pin);
            return 0;
        }
    }
    
    printf("Not found");
    return 1;
}
