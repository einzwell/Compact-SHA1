# Compact-SHA1

Compact-SHA1 is a compact and portable implementation of the Secure Hash Algorithm 1 in C with no external dependencies. It only consists of a single function, `SHA1Hash()`, which takes a contiguous input buffer to be hashed and generates a 160-bit (40-byte) binary digest.

This implementation is largely based on NIST's FIPS Publication 180-4 with a couple minor tweaks (see below) for slight optimisiation.

**Warning:** Not recommended for bulk hashing.

## Minor Tweaks
-  The bitwise choice function as specified in FIPS 180-4 (`(A ∧ B) ∨ (~A ∧ C)`) has been replaced with its minimised equivalent (`C ⊕ (A ∧ (C ⊕ B))`). Assuming non-optimising compiler, this should produce ~3 fewer assembly instructions than the original one.
- The bitwise majority function as specified in FIPS 180-4 (`(A ∧ B) ∨ (A ∧ C) ∨ (B ∧ C)`) has also been replaced with its minimised equivalent (`(A ∧ B) ∨ (C ∧ (A ∨ B)`) to achieve similar effect.

## Usage

You may include the file or simply copy the function and paste it into your own project.

## Contributing

Pull requests are welcome. Open an issue first to discuss what you would like to change.

## Reference

[FIPS 180-4](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf)

## License

[MIT](https://choosealicense.com/licenses/mit/)
