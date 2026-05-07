#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

// Помощна функция за принтиране на грешки
void handleErrors() {
    printf("An error occurred within OpenSSL logic.\n");
    exit(1);
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
        handleErrors();

    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;

    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))
        handleErrors();

    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

char* read_binary_file(const char* filename, size_t* out_size) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END); 
    long size = ftell(file);  
    rewind(file);             
    if (size < 0) {
        perror("Грешка при определяне размера на файла");
        fclose(file);
        return NULL;
    }

    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, size, file);
    if (bytes_read != (size_t)size) {
        perror("Error reading file");
        free(buffer); // Освобождаваме паметта при грешка
        fclose(file);
        return NULL;
    }

    buffer[size] = '\0';

    if (out_size) {
        *out_size = size;
    }

    fclose(file);
    return buffer;
}

int main() {
    unsigned char *key = (unsigned char *)"heheedatovaekluchami";
    
    unsigned char *plaintext = (unsigned char *)"Attack in 5am";
    unsigned char ciphertext[128];
    unsigned char decryptedtext[128];

    printf("Original Text: %s\n", plaintext);

    int cipher_len = encrypt(plaintext, strlen((char *)plaintext), key, ciphertext);

    printf("Ciphertext is (%d bytes):\n", cipher_len);
    for(int i = 0; i < cipher_len; i++) {
        printf("%02x", ciphertext[i]);
    }
    printf("\n\n");
    
    FILE *fp = fopen("cipher.bin", "wb");
    if (fp) {
        fwrite(ciphertext, 1, cipher_len, fp);
        fclose(fp);
        printf("Ciphertext saved to 'cipher.bin'\n\n");
    } else {
        printf("Failed to open file for writing.\n");
        return 1;
    }
    
    size_t read_cipher_size;
    unsigned char *read_ciphertext = (unsigned char *)read_binary_file("cipher.bin", &read_cipher_size);
    
    if (read_ciphertext) {
        int decrypted_len = decrypt(read_ciphertext, read_cipher_size, key, decryptedtext);
        
        decryptedtext[decrypted_len] = '\0'; 
        
        printf("Plaintext is (%d bytes): %s\n", decrypted_len, decryptedtext);
        
        free(read_ciphertext);
    }

    return EXIT_SUCCESS;
}