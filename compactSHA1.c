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
 * @param mLen Size of the input data (excluding null terminator).
 * @param digest The output buffer. Must be at least 20 bytes.
 *
 * @return 0 on success, otherwise non-zero.
 */

int compactSHA1(const uint8_t *restrict msg, const uint64_t mLen, uint8_t *restrict digest) {
	/* Sanity check */
	if (!msg || !digest) return -1;

	/* Calculate the total length of message plus padding */
	uint64_t pmLen = mLen + (64 - (mLen % 56) + ((mLen < 56) ? 0 : 8));

	/* Initialise the word schedule, the padded message, the IVs, and the constants */
	uint32_t word[80];
	uint8_t padMsg[pmLen];
	uint32_t h[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
	const uint32_t k[4] = {0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6};

	/* Begin the padding process */
	memset(padMsg, 0x00, pmLen);
	memcpy(padMsg, msg, mLen);
	padMsg[mLen] = 0x80;
	for (size_t pmIdx = 8; pmIdx > 0; pmIdx--) {
		padMsg[pmLen - pmIdx] = (uint8_t)((mLen * 8) >> (pmIdx * 8 - 8));
	}

	/* Divide the padded message into 512-bit chunks */
	for (size_t cIdx = 0; cIdx < pmLen / 64; cIdx++) {
		/* Prepare the message schedule */
		for (size_t wIdx = 0; wIdx < 16; wIdx++) {
			word[wIdx] = (padMsg[cIdx * 64 + wIdx * 4 + 0] << 24) +
						 (padMsg[cIdx * 64 + wIdx * 4 + 1] << 16) +
						 (padMsg[cIdx * 64 + wIdx * 4 + 2] << 8) +
						 (padMsg[cIdx * 64 + wIdx * 4 + 3] << 0);
		}

		for (size_t wIdx = 16; wIdx < 80; wIdx++) {
			word[wIdx] = word[wIdx - 3] ^ word[wIdx - 8] ^ word[wIdx - 14] ^ word[wIdx - 16];
			word[wIdx] = (word[wIdx] << 1) | (word[wIdx] >> 31);
		}
		
		/* Initialise the working variables with the intermediate hash value */
		uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4], t, f;

		/* Begin the compression function */
		for (size_t wIdx = 0; wIdx < 80; wIdx++) {
			f = (wIdx / 20 == 0) ? d ^ (b & (c ^ d)) : (wIdx / 20 == 1 || wIdx / 20 == 3) ? b ^ c ^ d : (b & c) | (d & (b | c));
			t = ((a << 5) | (a >> 27)) + f + e + k[wIdx / 20] + word[wIdx];
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
	for (size_t dIdx = 0; dIdx < 20; dIdx++) {
		digest[dIdx] = (uint8_t)(h[dIdx / 4] >> (24 - ((dIdx % 4) * 8)));
	}

	return 0;
}