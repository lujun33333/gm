#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

// MD5�㷨�еĳ���
const unsigned int T[] = {
    0x00000000, 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613,
    0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e,
    0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681,
    0xe7d3fbc8, 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9,
    0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60,
    0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8,
    0xc4ac5665, 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d,
    0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb,
    0xeb86d391 };

// MD5�㷨�е���ѭ����λ��
const unsigned int s[] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };

// F����
unsigned int F(unsigned int b, unsigned int c, unsigned int d) {
    return (b & c) | ((~b) & d);
}

// G����
unsigned int G(unsigned int b, unsigned int c, unsigned int d) {
    return (b & d) | (c & (~d));
}

// H����
unsigned int H(unsigned int b, unsigned int c, unsigned int d) {
    return b ^ c ^ d;
}

// I����
unsigned int I(unsigned int b, unsigned int c, unsigned int d) {
    return c ^ (b | (~d));
}

// ��ѭ����λ
unsigned int leftRotate(unsigned int x, unsigned int c) {
    return (x << c) | (x >> (32 - c));
}

// ����ַ���
vector<unsigned int> padding(const string& src) {
    vector<unsigned int> padded;
    size_t original_len = src.length();
    size_t new_len = original_len + 1 + ((56 - (original_len + 1) % 64) % 64);
    padded.resize(new_len + 8);

    // ��ԭʼ���ݰ���little-endian��䵽padded������
    for (size_t i = 0; i < original_len; ++i) {
        padded[i >> 2] |= static_cast<unsigned int>(src[i]) << ((i % 4) * 8);
    }
    padded[original_len >> 2] |= 0x80 << ((original_len % 4) * 8);
    padded[new_len >> 2] = static_cast<unsigned int>(original_len) * 8;

    return padded;
}

// MD5�㷨���Ĵ�����
string md5(const string& src) {
    vector<unsigned int> padded = padding(src);

    // ��ʼHashֵ
    unsigned int A = 0x67452301;
    unsigned int B = 0xefcdab89;
    unsigned int C = 0x98badcfe;
    unsigned int D = 0x10325476;

    unsigned int a, b, c, d, f, g, temp;
    for (size_t i = 0; i < padded.size(); i += 16) {
        a = A;
        b = B;
        c = C;
        d = D;

        for (int j = 0; j < 64; ++j) {
            if (j < 16) {
                f = F(b, c, d);
                g = j;
            }
            else if (j < 32) {
                f = G(b, c, d);
                g = (5 * j + 1) % 16;
            }
            else if (j < 48) {
                f = H(b, c, d);
                g = (3 * j + 5) % 16;
            }
            else {
                f = I(b, c, d);
                g = (7 * j) % 16;
            }

            temp = d;
            d = c;
            c = b;
            b = b + leftRotate(a + f + T[j] + padded[i + g], s[j]);
            a = temp;
        }

        A += a;
        B += b;
        C += c;
        D += d;
    }

    // ��A��B��C��D��little-endian��ʽ���MD5ֵ
    ostringstream result;
    result << hex << setw(8) << setfill('0') << A
        << hex << setw(8) << setfill('0') << B
        << hex << setw(8) << setfill('0') << C
        << hex << setw(8) << setfill('0') << D;

    return result.str();
}


