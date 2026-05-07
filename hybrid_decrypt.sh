#!/bin/bash

# 1. Декриптиране на AES ключа с RSA частния ключ на получателя
openssl pkeyutl -decrypt -in encrypted_aes_key.enc -inkey private.pem -pkeyopt rsa_padding_mode:oaep -out decrypted_aes_key.bin

# 2. Декриптиране на големия файл (data.enc) с възстановения AES-256 ключ
openssl enc -d -aes-256-cbc -pass file:./decrypted_aes_key.bin -in data.enc -out restored_big_data.zip

# 3. Почистване: Изтриваме възстановения (некриптиран) AES ключ от диска!
rm decrypted_aes_key.bin

echo "Хибридното декриптиране е завършено! Файлът е възстановен като 'restored_big_data.zip'."