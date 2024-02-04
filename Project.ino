/**
 * \file XTEATest_Arduino_style.ino
 * \brief XTEA cipher library testing program (C style).
 *
 * \copyright SPDX-FileCopyrightText: Copyright 2020-2021 Michal Protasowicki
 *
 * \license SPDX-License-Identifier: MIT
 *
 */

#include "Arduino.h"
#include "XTEA-Cipher.h"

#if defined(__AVR_ATmega4808__)
#define Serial Serial1
#endif

#define NUM_RUNS 100

void generateRandomByte(uint8_t* plaintext, size_t size) {
  for (size_t i = 0; i < size; i++) {
    plaintext[i] = random(256);
  }
}

void setup()
{
    Serial.begin(9600);
    Serial.println("Start tests... (Arduino style)");

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    

    uint8_t key[XTEA_KEY_SIZE] {0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F};
    uint8_t iv[XTEA_IV_SIZE]   {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
    uint8_t data[1024];
    generateRandomByte(data, sizeof(data));
    uint8_t encrEcb[1024];
    generateRandomByte(encrEcb, sizeof(encrEcb));
    uint8_t encrCfb[1024];
    generateRandomByte(encrCfb, sizeof(encrCfb));
    uint8_t encrOfb[1024];
    generateRandomByte(encrOfb, sizeof(encrOfb));


    uint8_t     resultBuffer[sizeof(data)];
    uint8_t     block[XTEA_BLOCK_SIZE] {0};
    uint32_t    start;
    uint32_t    time;
    uint8_t     chunks  = sizeof(data) / XTEA_BLOCK_SIZE;
    uint8_t     shift   = 0;

    String      timeStr = " time [us]: ";
    String      tiblStr = " time [us]/block: ";
    String      usStr   = ") [us]: ";
    String      chkStr  = "\t Checking data with test vector: ";
    String      encStr  = "Encrypt";
    String      decStr  = "Decrypt";
    String      pasStr  = "PASS";
    String      faiStr  = "FAIL";
    String      allStr  = " ALL loops (n = ";
    String      cfbMac  = "CFB-MAC";
    String      resultStr;

//----------------------------------------------------

    xtea.begin(XTEA_ROUNDS, XTEA_MAC_ROUNDS);
    Serial.println("XTEA_ROUNDS = " + String(XTEA_ROUNDS) + "   XTEA_MAC_ROUNDS = " + String(XTEA_MAC_ROUNDS));

    Serial.println("ECB:");
    unsigned long encryptionTime = 0;
    unsigned long decryptionTime = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
      start = micros();
      memcpy(resultBuffer, data, sizeof(data));                       // copying data to be encrypted into a buffer
      xtea.ecbEncrypt(key, resultBuffer, sizeof(data));               // data encryption in ECB mode
      time = micros() - start;
      encryptionTime += time;

      start = micros();
      memcpy(resultBuffer, encrEcb, sizeof(data));                    // copying data to be decrypted into a buffer
      xtea.ecbDecrypt(key, resultBuffer, sizeof(data));               // data decryption in ECB mode
      time = micros() - start;
      decryptionTime += time;
    }
    unsigned long averageEncryptionTime = encryptionTime / NUM_RUNS;
    unsigned long averageDecryptionTime = decryptionTime / NUM_RUNS;
    Serial.print("Plaintext size: ");
    Serial.print(sizeof(data));
    Serial.println(" bytes");
    Serial.print("Average Encryption Time: ");
    Serial.print(averageEncryptionTime);
    Serial.println(" microseconds");
    Serial.print("Average Decryption Time: ");
    Serial.print(averageDecryptionTime);
    Serial.println(" microseconds");
    Serial.println();

//----------------------------------------------------
    
    Serial.println("CFB:");

    encryptionTime = 0;
    decryptionTime = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
      start = micros();
      memcpy(resultBuffer, data, sizeof(data));                       // copying data to be encrypted into a buffer
      xtea.cfbEncrypt(key, iv, resultBuffer, sizeof(data));           // data encryption in CFB mode
      time = micros() - start;
      encryptionTime += time;

      start = micros();
      memcpy(resultBuffer, encrCfb, sizeof(data));                    // copying data to be decrypted into a buffer
      xtea.cfbDecrypt(key, iv, resultBuffer, sizeof(data));               // data decryption in CFB mode
      time = micros() - start;
      decryptionTime += time;
    }
    averageEncryptionTime = encryptionTime / NUM_RUNS;
    averageDecryptionTime = decryptionTime / NUM_RUNS;
    Serial.print("Plaintext size: ");
    Serial.print(sizeof(data));
    Serial.println(" bytes");
    Serial.print("Average Encryption Time: ");
    Serial.print(averageEncryptionTime);
    Serial.println(" microseconds");
    Serial.print("Average Decryption Time: ");
    Serial.print(averageDecryptionTime);
    Serial.println(" microseconds");
    Serial.println();

//----------------------------------------------------
    
    Serial.println("OFB:");

    encryptionTime = 0;
    decryptionTime = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
      start = micros();
      memcpy(resultBuffer, data, sizeof(data));                       // copying data to be encrypted into a buffer
      xtea.ofbEncrypt(key, iv, resultBuffer, sizeof(data));           // data encryption in OFB mode
      time = micros() - start;
      encryptionTime += time;

      start = micros();
      memcpy(resultBuffer, encrOfb, sizeof(data));                    // copying data to be decrypted into a buffer
      xtea.ofbDecrypt(key, iv, resultBuffer, sizeof(data));               // data decryption in OFB mode
      time = micros() - start;
      decryptionTime += time;
    }
    averageEncryptionTime = encryptionTime / NUM_RUNS;
    averageDecryptionTime = decryptionTime / NUM_RUNS;
    Serial.print("Plaintext size: ");
    Serial.print(sizeof(data));
    Serial.println(" bytes");
    Serial.print("Average Encryption Time: ");
    Serial.print(averageEncryptionTime);
    Serial.println(" microseconds");
    Serial.print("Average Decryption Time: ");
    Serial.print(averageDecryptionTime);
    Serial.println(" microseconds");
    Serial.println();
//----------------------------------------------------


    Serial.println("End tests.");
}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1950);
}

//----------------------------------------------------

uint32_t toBigEndian(uint32_t x)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    return __builtin_bswap32(x);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    return x;
#else
    #error "Unsupported hardware !!!" 
#endif
}
