#include <AES.h>
#include <Arduino.h>
#include <Crypto.h>
#include <CTR.h>
#include <EAX.h>
#include <GCM.h>
#include <XTS.h>

#define NUM_RUNS 1

// Define plaintext sizes to test
const size_t plaintextSizes[] = {1024, 2048};

void generateRandomPlaintext(uint8_t* plaintext, size_t size) {
   for (size_t i = 0; i < size; i++) {
       plaintext[i] = random(256);
   }
}

void generateRandomByte(uint8_t* plaintext, size_t size) {
   for (size_t i = 0; i < size; i++) {
       plaintext[i] = random(256);
   }
}

// Declare cipher instances for different key sizes
CTR<AES128> ctrAes128;
CTR<AES192> ctrAes192;
CTR<AES256> ctrAes256;

EAX<AES128> eaxAes128;
EAX<AES192> eaxAes192;
EAX<AES256> eaxAes256;

GCM<AES128> gcmAes128;
GCM<AES192> gcmAes192;
GCM<AES256> gcmAes256;

const size_t chunkSize = 1024; // Adjust as needed


void encryptChunks(Cipher *cipher, uint8_t *plaintext, size_t textSize, uint8_t *ciphertext) {
  size_t numChunks = textSize / chunkSize;
  size_t remainingBytes = textSize % chunkSize;

  for (size_t i = 0; i < numChunks; i++) {
    cipher->encrypt(ciphertext + i * chunkSize, plaintext + i * chunkSize, chunkSize);
  }
}

void decryptChunks(Cipher *cipher, uint8_t *ciphertext, size_t textSize, uint8_t *decrypted) {
  size_t numChunks = textSize / chunkSize;

  for (size_t i = 0; i < numChunks; i++) {
    cipher->decrypt(decrypted + i * chunkSize, ciphertext + i * chunkSize, chunkSize);
  }
}

void runExperiment(Cipher *cipher, uint8_t keySize) {
   for (size_t textSize : plaintextSizes) {
       uint8_t key[32];              // Maximum key size for AES-256
       uint8_t iv[16];               // Initialization Vector
       uint16_t ciphertext[textSize]; // Ciphertext buffer
       uint16_t decrypted[textSize];  // Decrypted text buffer

       size_t numChunks = textSize / chunkSize;
       size_t remainingBytes = textSize % chunkSize;


       // Generate random iv and set it
       generateRandomByte(iv, 16);
       cipher->setIV(iv, sizeof(iv));

       // Measure encryption and decryption time
       unsigned long encryptionTime = 0;
       unsigned long decryptionTime = 0;
       for (int i = 0; i < NUM_RUNS; i++) {

           // Generate random key
           generateRandomByte(key, keySize);

          for(size_t textSize : plaintextSizes){
            // Generate random plaintext
            uint8_t plaintext[textSize];
            generateRandomPlaintext(plaintext, textSize);

            // Encryption
            uint8_t ciphertext[textSize];
            unsigned long startTime = micros();
            cipher->setKey(key, sizeof(key));
            encryptChunks(cipher, plaintext, textSize, ciphertext);
            unsigned long endTime = micros();
            encryptionTime += (endTime - startTime);

            // Decryption
            uint8_t decrypted[textSize];
            startTime = micros();
            cipher->setKey(key, sizeof(key));
            decryptChunks(cipher, ciphertext, textSize, decrypted);
            endTime = micros();
            decryptionTime += (endTime - startTime);
        }
       }

       // Generate output for the current plaintext size
       unsigned long averageEncryptionTime = encryptionTime / NUM_RUNS;
       unsigned long averageDecryptionTime = decryptionTime / NUM_RUNS;
       Serial.print("Plaintext size: ");
       Serial.print(textSize);
       Serial.println(" bytes");
       Serial.print("Average Encryption Time: ");
       Serial.print(averageEncryptionTime);
       Serial.println(" microseconds");
       Serial.print("Average Decryption Time: ");
       Serial.print(averageDecryptionTime);
       Serial.println(" microseconds");
       Serial.println();
   }
}

void setup() {
   Serial.begin(9600);
   Serial.println();
   Serial.println();
   // ... (rest of setup code)

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
}

void loop() {
    // Empty loop
}
