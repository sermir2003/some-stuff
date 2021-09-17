#include "utf8.h"
#include <cstddef>
#include <vector>
#include <string>


const int32_t kCntBitsForByte[5][4] = {
    {},
    {7},
    {5, 6},
    {4, 6, 6},
    {3, 6, 6, 6}
};

const int32_t kCntAllBitsForByte[5] = {
    0,
    kCntBitsForByte[1][0],
    kCntBitsForByte[2][0] + kCntBitsForByte[2][1],
    kCntBitsForByte[3][0] + kCntBitsForByte[3][1] + kCntBitsForByte[3][2],
    kCntBitsForByte[4][0] + kCntBitsForByte[4][1] + kCntBitsForByte[4][2] + kCntBitsForByte[4][3]
};

size_t CountUtf8Symbols(const std::string& str) {
    size_t count_symbols = 0;
    for (size_t i = 0; i < str.size(); ) {
        int32_t cnt1_prefix = 0;
        while (((str[i] >> (7 - cnt1_prefix)) & 1) == 1) {
            ++cnt1_prefix;
        }
        int32_t cnt_bytes = cnt1_prefix;
        if (cnt1_prefix == 0) {
            cnt_bytes = 1;
        }
        i += cnt_bytes;
        ++count_symbols;
    }
    return count_symbols;
}

void EncodeUtf8(const std::vector<uint32_t>& codepoints, std::string& str) {
    for (size_t i = 0; i < codepoints.size(); ++i) {
        uint32_t current_codepoint = codepoints[i];
        int32_t used_bits = 32;
        while (used_bits > 0 && ((codepoints[i] >> (used_bits - 1)) & 1) == 0) {
            --used_bits;
        }
        int32_t cnt_bytes = 0;
        while (used_bits > kCntAllBitsForByte[cnt_bytes]) {
            ++cnt_bytes;
        }
        std::string buffer(cnt_bytes, 0);
        for (int32_t j = cnt_bytes - 1; j >= 0; --j) {
            int32_t cnt_bits = kCntBitsForByte[cnt_bytes][j];
            int32_t prefix_of1 = 8 - cnt_bits - 1;
            buffer[j] = (1 << prefix_of1) - 1;
            buffer[j] <<= cnt_bits + 1;
            uint32_t mask = (1u << cnt_bits) - 1;
            buffer[j] += current_codepoint & mask;
            current_codepoint >>= cnt_bits;
        }
        str += buffer;
    }
}

void DecodeUtf8(const std::string& str, std::vector<uint32_t>& codepoints) {
    codepoints.reserve(str.size());
    for (size_t i = 0; i < str.size(); ) {
        int32_t cnt1_prefix = 0;
        while (((str[i] >> (7 - cnt1_prefix)) & 1) == 1) {
            ++cnt1_prefix;
        }
        int32_t cnt_bytes = cnt1_prefix;
        if (cnt1_prefix == 0) {
            cnt_bytes = 1;
        }
        uint32_t current_codepoint = 0;
        for (int32_t j = 0; j < cnt_bytes; ++j) {
            int32_t cnt_bits = kCntBitsForByte[cnt_bytes][j];
            current_codepoint <<= cnt_bits;
            int32_t mask = (1u << cnt_bits) - 1;
            current_codepoint += str[i + j] & mask;
        }
        codepoints.push_back(current_codepoint);
        i += cnt_bytes;
    }
}
