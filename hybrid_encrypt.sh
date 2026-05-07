#!/bin/bash

# 1. Генериране на случаен 32-байтов (256-bit) AES ключ и записването му във файл
openssl rand -out aes_key.bin 32
echo "1. Генериран е случаен AES-256 ключ."

# 2. Криптиране на големия файл (big_data.zip) с AES-256-CBC, използвайки ключа от файла
# (Забележка: В реалността трябва да генерирате и запазите IV, но за теста го пропускаме)
openssl enc -aes-256-cbc -pass file:./aes_key.bin -in big_data.zip -out data.enc
echo "2. Файлът 'big_data.zip' е криптиран с AES ключа."

# 3. Криптиране на самия AES ключ с RSA публичния ключ на получателя
openssl pkeyutl -encrypt -in aes_key.bin -pubin -inkey public.pem -pkeyopt rsa_padding_mode:oaep -out encrypted_aes_key.enc
echo "3. AES ключът е криптиран с RSA публичния ключ."

# 4. Почистване: Изтриваме оригиналния (некриптиран) AES ключ от диска!
rm aes_key.bin
echo "4. Оригиналният AES ключ е изтрит за сигурност."

echo "Хибридното криптиране е завършено! Изпратете 'data.enc' и 'encrypted_aes_key.enc'."