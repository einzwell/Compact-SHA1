/*!
 * @file compactSHA1-test.c
 * @author Einzwell (einzwell\@protonmail.com)
 * @brief A file test to check compactSHA1()'s functionality
 * @see <a href="https://www.di-mgt.com.au/sha_testvectors.html">Test vector reference</a>
 */

#include <stdio.h>
#include "compactSHA1.c"

// TODO: Inspect test vector 6

int main() {
    const uint8_t *strings[] = {
            "The quick brown fox jumps over the lazy dog",
            "The quick brown fox jumps over the lazy cog",
            "",
            "abc",
            "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
            "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
    };

    const uint8_t testVectors[][20] = {
            "\x2f\xd4\xe1\xc6\x7a\x2d\x28\xfc\xed\x84\x9e\xe1\xbb\x76\xe7\x39\x1b\x93\xeb\x12",
            "\xde\x9f\x2c\x7f\xd2\x5e\x1b\x3a\xfa\xd3\xe8\x5a\x0b\xd1\x7d\x9b\x10\x0d\xb4\xb3",
            "\xda\x39\xa3\xee\x5e\x6b\x4b\x0d\x32\x55\xbf\xef\x95\x60\x18\x90\xaf\xd8\x07\x09",
            "\xa9\x99\x3e\x36\x47\x06\x81\x6a\xba\x3e\x25\x71\x78\x50\xc2\x6c\x9c\xd0\xd8\x9d",
            "\x84\x98\x3e\x44\x1c\x3b\xd2\x6e\xba\xae\x4a\xa1\xf9\x51\x29\xe5\xe5\x46\x70\xf1",
            "\xa4\x9b\x24\x46\xa0\x2c\x64\x5b\xf4\x19\xf9\x95\xb6\x70\x91\x25\x3a\x04\xa2\x59"
    };

    uint8_t computedResult[20];

    for (int i = 0; i < 6; i++) {
        printf("[TEST %d]\nSTRING     : \"%s\"\n", i + 1, strings[i]);
        printf("TEST VECTOR: ");
        for (int j = 0; j < 20; j++)
            printf("%02x", testVectors[i][j]);

        compactSHA1(strings[i], strlen((char *)strings[i]), (uint8_t *)computedResult);
        printf("\nHASH RESULT: ");
        for (int j = 0; j < 20; j++)
            printf("%02x", computedResult[j]);

        printf("\nMATCH      : %s\n\n", memcmp(testVectors[i], computedResult, 20) ? "FALSE" : "TRUE");
    }

    return 0;
}