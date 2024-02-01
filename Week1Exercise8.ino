#include <AES.h>
#include <Arduino.h>
#include <Crypto.h>
#include <CTR.h>
#include <EAX.h>
#include <GCM.h>
#include <XTS.h>

#define NUM_RUNS 100

void generateRandomPlaintext(uint8_t* plaintext, size_t size) {
    for (size_t i = 0; i < size; i++) {
        plaintext[i] = random(256);
    }
}

CTR<AES128> ctrAes128;
CTR<AES192> ctrAes192;
CTR<AES256> ctrAes256;

EAX<AES128> eaxAes128;
EAX<AES192> eaxAes192;
EAX<AES256> eaxAes256;

GCM<AES128> gcmAes128;
GCM<AES192> gcmAes192;
GCM<AES256> gcmAes256;

// XTS<AES128> *xtsAes128;
// XTS<AES192> *xtsAes192;
// XTS<AES256> *xtsAes256;

void runExperiment(Cipher *cipher, uint8_t size) {
    uint8_t plaintext[16]; // Assuming 128-bit block size
    uint8_t key[32]; // Maximum key size for AES-256
    uint8_t iv[16]; // Initialization Vector
    uint8_t ciphertext[16];
    uint8_t decrypted[16];

    // Generate random iv and set it
    generateRandomPlaintext(iv, 16);
    cipher -> setIV(iv, sizeof(iv));

    // Measure encryption time
    unsigned long encryptionTime = 0;
    unsigned long decryptionTime = 0;
    for (int i = 0; i < NUM_RUNS; i++) {

        // Generate random key
        generateRandomPlaintext(key, size);

        //key generation
        generateRandomPlaintext(plaintext, sizeof(plaintext));

        //encryption
        unsigned long startTime = micros();
        cipher -> setKey(key, sizeof(key));
        cipher -> encrypt(ciphertext, plaintext, sizeof(plaintext));
        unsigned long endTime = micros();
        encryptionTime += (endTime - startTime);
    
        //decryption
        startTime = micros();
        cipher -> setKey(key, sizeof(key));
        cipher -> decrypt(decrypted, ciphertext, sizeof(ciphertext));
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
    Serial.println();
    Serial.println("------------------------------------------------");
    Serial.println("CTR");
    Serial.println("-----");
    Serial.println("AES128");
    runExperiment(&ctrAes128,16);
    Serial.println("AES192");
    runExperiment(&ctrAes192,24);
    Serial.println("AES256");
    runExperiment(&ctrAes256,32);
    Serial.println();

    Serial.println("------------------------------------------------");
    Serial.println("EAX");
    Serial.println("-----");
    Serial.println("AES128");
    runExperiment(&eaxAes128,16);
    Serial.println("AES192");
    runExperiment(&eaxAes192,24);
    Serial.println("AES256");
    runExperiment(&eaxAes256,32);
    Serial.println();

    Serial.println("------------------------------------------------");
    Serial.println("GCM");
    Serial.println("-----");
    Serial.println("AES128");
    runExperiment(&gcmAes128,16);
    Serial.println("AES192");
    runExperiment(&gcmAes192,24);
    Serial.println("AES256");
    runExperiment(&gcmAes256,32);
    Serial.println();

    // Serial.println("------------------------------------------------");
    // Serial.println("XTS");
    // Serial.println("-----");
    // Serial.println("AES128");
    // runExperiment(&xtsAes128,16);
    // Serial.println("AES192");
    // runExperiment(&xtsAes192,24);
    // Serial.println("AES256");
    // runExperiment(&xtsAes256,32);
    // Serial.println();

    //CTR, EAX, GCM, XTS
}

void loop() {
    // Empty loop
}
