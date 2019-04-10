#include "vBlake.h"

using namespace std;

namespace vbk {

    #define B2B_G(v, a, b, c, d, x, y, c1, c2) { \
        v[a] = v[a] + v[b] + (x ^ c1); \
        v[d] ^= v[a]; \
        v[d] = ROTR64(v[d], 60); \
        v[c] = v[c] + v[d]; \
        v[b] = ROTR64(v[b] ^ v[c], 43); \
        v[a] = v[a] + v[b] + (y ^ c2); \
        v[d] = ROTR64(v[d] ^ v[a], 5); \
        v[c] = v[c] + v[d]; \
        v[b] = ROTR64(v[b] ^ v[c], 18); \
        v[d] ^= (~v[a] & ~v[b] & ~v[c]) | (~v[a] & v[b] & v[c]) | (v[a] & ~v[b] & v[c]) | \
                (v[a] & v[b] & ~v[c]); \
        v[d] ^= (~v[a] & ~v[b] & v[c]) | (~v[a] & v[b] & ~v[c]) | (v[a] & ~v[b] & ~v[c]) | \
                (v[a] & v[b] & v[c]); \
    }

    /**
     * Rotate x, a 64-bit long, to the right by y places
     */
    #define ROTR64(x, n)  (((x) >> (n)) | ((x) << (64 - (n))))

    const uint64_t vBlake::vBlake_iv[8] =   {
                                                0x4BBF42C1F006AD9Dl, 0x5D11A8C3B5AEB12El,
                                                0xA64AB78DC2774652l, 0xC67595724658F253l,
                                                0xB8864E79CB891E56l, 0x12ED593E29FB41A1l,
                                                0xB1DA3AB63C60BAA8l, 0x6D20E50C1F954DEDl
                                            };

    const uint8_t vBlake::sigma[16][16] =   {
                                                {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
                                                {14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3},
                                                {11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4},
                                                {7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8},
                                                {9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13},
                                                {2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9},
                                                {12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11},
                                                {13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10},
                                                {6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5},
                                                {10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0},
                                                {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
                                                {14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3},
                                                {11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4},
                                                {7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8},
                                                {9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13},
                                                {2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9}
                                            };

    const uint64_t vBlake::vBlake_c[16] =   {
                                                0xA51B6A89D489E800L, 0xD35B2E0E0B723800L,
                                                0xA47B39A2AE9F9000L, 0x0C0EFA33E77E6488L,
                                                0x4F452FEC309911EBL, 0x3CFCC66F74E1022CL,
                                                0x4606AD364DC879DDL, 0xBBA055B53D47C800L,
                                                0x531655D90C59EB1BL, 0xD1A00BA6DAE5B800L,
                                                0x2FE452DA9632463EL, 0x98A7B5496226F800L,
                                                0xBAFCD004F92CA000L, 0x64A39957839525E7L,
                                                0xD859E6F081AAE000L, 0x63D980597B560E6BL
                                            };

    vector<char> vBlake::hash(vector<char> &input)
    {
        // BEGIN INIT
        uint64_t h[8];
        vector<char> b(64);

        h[0] = vBlake_iv[0];
        h[1] = vBlake_iv[1];
        h[2] = vBlake_iv[2];
        h[3] = vBlake_iv[3];
        h[4] = vBlake_iv[4];
        h[5] = vBlake_iv[5];
        h[6] = vBlake_iv[6];
        h[7] = vBlake_iv[7];

        // outlen = 24, as VeriBlock uses a 192-bit hash
        h[0] ^= static_cast<uint64_t>(0x01010000 ^ 0x18);
        // END INIT

        // BEGIN UPDATE
        for (int i = 0; i < input.size(); i++) {
            b[i] = input[i];
        }

        compress(h, b);
        // END UPDATE

        return recombineB2Bh(h);
    }

    void vBlake::compress(uint64_t h[], vector<char> &b)
    {
        uint64_t v[16];
        uint64_t m[16] =    { 0, 0, 0, 0, 0, 0, 0, 0,
                              0, 0, 0, 0, 0, 0, 0, 0 };

        for (int i = 0; i < 8; i++) {
            v[i] = h[i];
            v[i + 8] = vBlake_iv[i];
        }

        v[12] ^= 64; // Input count low
        v[13] ^= 0; // Input count high (no overflow, therefore 0)
        v[14] ^= static_cast<long>(-1); // f[0] = 0xFF..FF
        v[15] ^= 0; // f[1] = 0x00..00

        for (int i = 0; i < 8; i++) {
            char byytes[8] = {b[i * 8 + 7], b[i * 8 + 6], b[i * 8 + 5], b[i * 8 + 4], b[i * 8 + 3],
                    b[i * 8 + 2], b[i * 8 + 1], b[i * 8 + 0]};

            m[i] = bytesToLong(byytes);
        }

        // Using 16 rounds of the Blake2 G function, drawing on the additional 4 rows
        // of sigma from reference BLAKE implementation
        for (auto i : sigma) {
            B2B_G(v, 0, 4, 8, 12, m[i[1]], m[i[0]], vBlake_c[i[1]],
                  vBlake_c[i[0]]);

            B2B_G(v, 1, 5, 9, 13, m[i[3]], m[i[2]], vBlake_c[i[3]],
                  vBlake_c[i[2]]);

            B2B_G(v, 2, 6, 10, 14, m[i[5]], m[i[4]], vBlake_c[i[5]],
                  vBlake_c[i[4]]);

            B2B_G(v, 3, 7, 11, 15, m[i[7]], m[i[6]], vBlake_c[i[7]],
                  vBlake_c[i[6]]);

            B2B_G(v, 0, 5, 10, 15, m[i[9]], m[i[8]], vBlake_c[i[9]],
                  vBlake_c[i[8]]);

            B2B_G(v, 1, 6, 11, 12, m[i[11]], m[i[10]], vBlake_c[i[11]],
                  vBlake_c[i[10]]);

            B2B_G(v, 2, 7, 8, 13, m[i[13]], m[i[12]], vBlake_c[i[13]],
                  vBlake_c[i[12]]);

            B2B_G(v, 3, 4, 9, 14, m[i[15]], m[i[14]], vBlake_c[i[15]],
                  vBlake_c[i[14]]);
        }

        // Update h[0 .. 7]
        for (int i = 0; i < 8; i++) {
            h[i] ^= v[i] ^ v[i + 8];
        }

        h[0] ^= h[3] ^ h[6];
        h[1] ^= h[4] ^ h[7];
        h[2] ^= h[5];
    }

    unsigned long vBlake::bytesToLong(char bytes[8])
    {
        unsigned long result =
         (((static_cast<unsigned long>(bytes[0])) & 0xFF) << 56) ^
               (((static_cast<unsigned long>(bytes[1])) & 0xFF) << 48) ^
               (((static_cast<unsigned long>(bytes[2])) & 0xFF) << 40) ^
               (((static_cast<unsigned long>(bytes[3])) & 0xFF) << 32) ^
               (((static_cast<unsigned long>(bytes[4])) & 0xFF) << 24) ^
               (((static_cast<unsigned long>(bytes[5])) & 0xFF) << 16) ^
               (((static_cast<unsigned long>(bytes[6])) & 0xFF) << 8) ^
               (((static_cast<unsigned long>(bytes[7])) & 0xFF));

        return result;
    }

    vector<char> vBlake::recombineB2Bh(uint64_t h[])
    {
        vector<char> output(24);

        for (int i = 0; i < 3; i++) {
            output[i * 8 + 0] = static_cast<char>(h[i] >> 0);
            output[i * 8 + 1] = static_cast<char>(h[i] >> 8);
            output[i * 8 + 2] = static_cast<char>(h[i] >> 16);
            output[i * 8 + 3] = static_cast<char>(h[i] >> 24);
            output[i * 8 + 4] = static_cast<char>(h[i] >> 32);
            output[i * 8 + 5] = static_cast<char>(h[i] >> 40);
            output[i * 8 + 6] = static_cast<char>(h[i] >> 48);
            output[i * 8 + 7] = static_cast<char>(h[i] >> 56);
        }

        return output;
    }
}
