#include <AES.h>
#include <Arduino.h>
#include <Crypto.h>
#include <CTR.h>
#include <GCM.h>

#define NUM_RUNS 100

void generateRandomPlaintext(uint8_t* plaintext, size_t size) {
    for (size_t i = 0; i < size; i++) {
        plaintext[i] = random(256);
    }
}

AES128 aes128;
AES192 aes192;
AES256 aes256;

void runExperiment(BlockCipher *cipher, uint8_t size) {
    uint8_t plaintext[16]; // Assuming 128-bit block size
    uint8_t key[32]; // Maximum key size for AES-256
    uint8_t ciphertext[16];
    uint8_t decrypted[16];


    // Measure encryption time
    unsigned long encryptionTime = 0;
    unsigned long decryptionTime = 0;
    for (int i = 0; i < NUM_RUNS; i++) {

        // Generate random key
        generateRandomPlaintext(key, size);

        //key generation + encryption
        generateRandomPlaintext(plaintext, sizeof(plaintext));
        unsigned long startTime = micros();
        cipher -> setKey(key, sizeof(key));
        cipher -> encryptBlock(ciphertext, plaintext);
        unsigned long endTime = micros();
        encryptionTime += (endTime - startTime);
    
        //decryption
        startTime = micros();
        cipher -> setKey(key, sizeof(key));
        cipher -> decryptBlock(decrypted, ciphertext);
        endTime = micros();
        decryptionTime += (endTime - startTime);
    }

    //generating output
    unsigned long averageEncryptionTime = encryptionTime / NUM_RUNS;
    unsigned long averageDecryptionTime = decryptionTime / NUM_RUNS;
    Serial.print("Average Encryption Time: ");
    Serial.print(averageEncryptionTime);
    Serial.println(" microseconds");

    Serial.print("Average Decryption Time: ");
    Serial.print(averageDecryptionTime);
    Serial.println(" microseconds");
    Serial.println();
}

void setup() {
    Serial.begin(9600);
    Serial.println();
    Serial.println("AES128");
    runExperiment(&aes128,16);
    Serial.println("AES192");
    runExperiment(&aes192,24);
    Serial.println("AES256");
    runExperiment(&aes256,32);
}

void loop() {
    // Empty loop
}
