# Compact-SHA1

Compact-SHA1 is a compact and portable implementation of the Secure Hash Algorithm 1 in C with no external dependencies. It only consists of a single function, `compactSHA1()`, which takes a contiguous input buffer to be hashed as an argument and generates a 160-bit (40-byte) binary digest.

This implementation is largely based on NIST's FIPS Publication 180-4 with a couple minor tweaks for slight optimisation.

**Warning:** Not recommended for bulk hashing.

## Usage

You may include the file or simply copy the function and paste it into your own project.

## Contributing

Pull requests are welcome. Open an issue first to discuss what you would like to change.

## Reference

[FIPS 180-4](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf)

## License

[MIT](https://choosealicense.com/licenses/mit/)
