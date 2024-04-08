/*!
 * @file compactSHA1.c
 * @author Einzwell (einzwell\@protonmail.com)
 * @brief A compact and portable implementation of SHA-1 in C with no external dependencies.
 *
 * This implementation is largely based on NIST's FIPS Publication 180-4 with
 * a few minor tweaks to slightly optimise the algorithm.
 *
 * @warning Not recommended for bulk hashing.
 * @see <a href="https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf">NIST's FIPS Publication 180-4</a>
 */

#include <stdint.h> // For fixed-width datatypes.
#include <string.h> // For memset and memcpy; may be replaced with customised functions if required.

/*!
 * @brief Calculates the SHA-1 hash of given input.
 *
 * @param msg The input buffer.
 * @param msg_len Size of the input data (excluding null terminator).
 * @param digest The output buffer. Must be at least 20 bytes.
 *
 * @return 0 on success, otherwise non-zero.
 */

int compactSHA1(const uint8_t *restrict msg, const uint64_t msg_len, uint8_t *restrict digest) {
    /* Sanity check */
    if (!msg || !digest)
        return -1;

    /* Calculate the total length of message plus padding */
    uint64_t pad_msg_len = msg_len + (64 - (msg_len % 64));

    /* Initialise the word schedule, the padded message, the IVs, and the constants */
    uint32_t word[80];
    uint8_t pad_msg[pad_msg_len];
    uint32_t h[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
    const uint32_t k[4] = {0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6};

    /* Begin the padding process */
    memset(pad_msg, 0x00, pad_msg_len);
    memcpy(pad_msg, msg, msg_len);
    pad_msg[msg_len] = 0x80;
    for (size_t pad_msg_i = 8; pad_msg_i > 0; pad_msg_i--) {
        pad_msg[pad_msg_len - pad_msg_i] = (uint8_t) ((msg_len * 8) >> (pad_msg_i * 8 - 8));
    }

    /* Divide the padded message into 512-bit chunks */
    for (size_t c_i = 0; c_i < pad_msg_len / 64; c_i++) {
        /* Prepare the message schedule */
        for (size_t w_i = 0; w_i < 16; w_i++) {
            word[w_i] = (pad_msg[c_i * 64 + w_i * 4 + 0] << 24) +
                        (pad_msg[c_i * 64 + w_i * 4 + 1] << 16) +
                        (pad_msg[c_i * 64 + w_i * 4 + 2] << 8) +
                        (pad_msg[c_i * 64 + w_i * 4 + 3]);
        }

        for (size_t w_i = 16; w_i < 80; w_i++) {
            word[w_i] = word[w_i - 3] ^ word[w_i - 8] ^ word[w_i - 14] ^ word[w_i - 16];
            word[w_i] = (word[w_i] << 1) | (word[w_i] >> 31);
        }

        /* Initialise the working variables with the intermediate hash value */
        uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], t, f;

        /* Begin the compression function */
        for (size_t w_i = 0; w_i < 80; w_i++) {
            f = (w_i / 20 == 0) ? d ^ (b & (c ^ d)) : (w_i / 20 == 1 || w_i / 20 == 3) ? b ^ c ^ d : (b & c) | (d & (b | c));
            t = ((a << 5) | (a >> 27)) + f + e + k[w_i / 20] + word[w_i];
            e = d;
            d = c;
            c = (b << 30) | (b >> 2);
            b = a;
            a = t;
        }

        /* Compute the new intermediate hash value */
        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
        h[4] += e;
    }

    /* Copy the resultant hash into the digest buffer (big-endian) */
    for (size_t d_i = 0; d_i < 20; d_i++) {
        digest[d_i] = (uint8_t) (h[d_i / 4] >> (24 - ((d_i % 4) * 8)));
    }

    return 0;
}