#include "../UnicodeConv.h"

#include <codecvt>
#include <iostream>
#include <locale>

#define U8TEXT(text) u8##text
#define U16TEXT(text) u##text
#define U32TEXT(text) U##text
#define WTEXT(text) L##text

template <typename T0, typename T1, typename Conv>
bool TestConv(const T0* text0, const T1* text1, Conv conv) {
    std::basic_string<T1> result;
    if (!conv(text0, result)) {
        return false;
    }
    if (std::basic_string<T1>(text1) != result) {
        return false;
    }
    return true;
}

#define TestConvXToY(X, Y, text)                                       \
    if (!TestConv(X##TEXT(text), Y##TEXT(text), Conv##X##To##Y)) {     \
        std::cout << "Faild to Convert " << #X << "To" << #Y << " : (" \
                  << __LINE__ << ") \"" << text << "\"\n";             \
        return 1;                                                      \
    }

#define TestConvAll(text)         \
    TestConvXToY(U8, U16, text);  \
    TestConvXToY(U8, U32, text);  \
    TestConvXToY(U16, U8, text);  \
    TestConvXToY(U16, U32, text); \
    TestConvXToY(U32, U8, text);  \
    TestConvXToY(U32, U16, text);

#define TestConvChU32ToU8(u32Ch, checkText)                              \
    {                                                                    \
        bool result = true;                                              \
        std::array<char, 4> u8Ch;                                        \
        if (!ConvChU32ToU8(u32Ch, u8Ch)) {                               \
            result = false;                                              \
        }                                                                \
        std::string u8Str(checkText);                                    \
        u8Str.resize(4, '\0');                                           \
        if (u8Ch[0] != u8Str[0]) {                                       \
            result = false;                                              \
        }                                                                \
        if (u8Ch[1] != u8Str[1]) {                                       \
            result = false;                                              \
        }                                                                \
        if (u8Ch[2] != u8Str[2]) {                                       \
            result = false;                                              \
        }                                                                \
        if (u8Ch[3] != u8Str[3]) {                                       \
            result = false;                                              \
        }                                                                \
        if (!result) {                                                   \
            std::cout << "Faild to Convert Char U32ToU8 : (" << __LINE__ \
                      << ") \"" << int(u32Ch) << "\"\n";                 \
            return 1;                                                    \
        }                                                                \
    }

#define TestConvChU32ToU16(u32Ch, checkText)                              \
    {                                                                     \
        bool result = true;                                               \
        std::array<char16_t, 2> u16Ch;                                    \
        if (!ConvChU32ToU16(u32Ch, u16Ch)) {                              \
            result = false;                                               \
        }                                                                 \
        std::u16string u16Str(checkText);                                 \
        u16Str.resize(2, '\0');                                           \
        if (u16Ch[0] != u16Str[0]) {                                      \
            result = false;                                               \
        }                                                                 \
        if (u16Ch[1] != u16Str[1]) {                                      \
            result = false;                                               \
        }                                                                 \
        if (!result) {                                                    \
            std::cout << "Faild to Convert Char U32ToU16 : (" << __LINE__ \
                      << ") \"" << int(u32Ch) << "\"\n";                  \
            return 1;                                                     \
        }                                                                 \
    }

#define TestConvChU8ToU32(checkText, u32Ch)                              \
    {                                                                    \
        bool result = true;                                              \
        std::string text = checkText;                                    \
        text.resize(4, '\0');                                            \
        std::array<char, 4> u8Ch;                                        \
        u8Ch[0] = text[0];                                               \
        u8Ch[1] = text[1];                                               \
        u8Ch[2] = text[2];                                               \
        u8Ch[3] = text[3];                                               \
        char32_t ch;                                                     \
        if (!ConvChU8ToU32(u8Ch, ch)) {                                  \
            result = false;                                              \
        }                                                                \
        if (u32Ch != ch) {                                               \
            result = false;                                              \
        }                                                                \
        if (!result) {                                                   \
            std::cout << "Faild to Convert Char U8ToU16 : (" << __LINE__ \
                      << ") \"" << int(u32Ch) << "\"\n";                 \
            return 1;                                                    \
        }                                                                \
    }

#define TestConvChU16ToU32(checkText, u32Ch)                              \
    {                                                                     \
        bool result = true;                                               \
        std::u16string text = checkText;                                  \
        text.resize(2, '\0');                                             \
        std::array<char16_t, 2> u16Ch;                                    \
        u16Ch[0] = text[0];                                               \
        u16Ch[1] = text[1];                                               \
        char32_t ch;                                                      \
        if (!ConvChU16ToU32(u16Ch, ch)) {                                 \
            result = false;                                               \
        }                                                                 \
        if (u32Ch != ch) {                                                \
            result = false;                                               \
        }                                                                 \
        if (!result) {                                                    \
            std::cout << "Faild to Convert Char U16ToU32 : (" << __LINE__ \
                      << ") \"" << int(u32Ch) << "\"\n";                  \
            return 1;                                                     \
        }                                                                 \
    }

int TestMain() {
    TestConvChU32ToU8(U'a', u8"a");
    TestConvChU32ToU8(U'ā', u8"ā");
    TestConvChU32ToU8(U'😃', u8"😃");

    TestConvChU8ToU32(u8"a", U'a');
    TestConvChU8ToU32(u8"ā", U'ā');
    TestConvChU8ToU32(u8"😃", U'😃');

    TestConvChU32ToU16(U'a', u"a");
    TestConvChU32ToU16(U'ā', u"ā");
    TestConvChU32ToU16(U'😃', u"😃");

    TestConvChU16ToU32(u"a", U'a');
    TestConvChU16ToU32(u"ā", U'ā');
    TestConvChU16ToU32(u"😃", U'😃');

    TestConvAll("😃😥🙀");
    TestConvAll("abc");
    TestConvAll("日本語");
    TestConvAll("ΦΩε");

    std::wstring_convert<std::codecvt_utf8_utf16<uint16_t>, uint16_t>
        u16convert;
    std::wstring_convert<std::codecvt_utf8<uint32_t>, uint32_t> u32convert;

    // Conv check
    for (char32_t ch = 0; ch < 0x10FFFF; ch++) {
        char32_t u32Ch = ch;
        std::array<char, 4> u8Ch;
        if (!ConvChU32ToU8(u32Ch, u8Ch)) {
            std::cout << "Faild to convert char (u32 to u8) [" << u32Ch
                      << "]\n";
            return 1;
        }
        std::array<char16_t, 2> u16Ch;
        if (!ConvChU32ToU16(u32Ch, u16Ch)) {
            std::cout << "Faild to convert char (u32 to u16) [" << u32Ch
                      << "]\n";
            return 1;
        }
        std::string u8correct = u32convert.to_bytes(u32Ch);
        u8correct.resize(4, '\0');
        if (u8correct[0] != u8Ch[0] || u8correct[1] != u8Ch[1] ||
            u8correct[2] != u8Ch[2] || u8correct[3] != u8Ch[3]) {
            std::cout << "Faild to compare char (u32 to u8) [" << u32Ch
                      << "]\n";
            return 1;
        }

        auto u16correct = u16convert.from_bytes(u8correct.c_str());
        u16correct.resize(2, '\0');
        if (u16correct[0] != u16Ch[0] || u16correct[1] != u16Ch[1]) {
            std::cout << "Faild to compare char (u32 to u16) [" << u32Ch
                      << "]\n";
            return 1;
        }

        // u8 to u32
        {
            char32_t testCh;
            if (!ConvChU8ToU32(u8Ch, testCh) || testCh != u32Ch) {
                std::cout << "Faild to convert char (u8 to u32) [" << u32Ch
                          << "]\n";
                return 1;
            }
        }
        // u8 to u16
        {
            std::array<char16_t, 2> testCh;
            if (!ConvChU8ToU16(u8Ch, testCh) || testCh != u16Ch) {
                std::cout << "Faild to convert char (u8 to u16) [" << u32Ch
                          << "]\n";
                return 1;
            }
        }

        // u16 to u32
        {
            char32_t testCh;
            if (!ConvChU16ToU32(u16Ch, testCh) || testCh != u32Ch) {
                std::cout << "Faild to convert char (u8 to u32) [" << u32Ch
                          << "]\n";
                return 1;
            }
        }
        // u16 to u8
        {
            std::array<char, 4> testCh;
            if (!ConvChU16ToU8(u16Ch, testCh) || testCh != u8Ch) {
                std::cout << "Faild to convert char (u8 to u16) [" << u32Ch
                          << "]\n";
                return 1;
            }
        }
    }

    return 0;
}


int main() {
    auto ret = TestMain();
    if (ret == 0) {
        std::cout << "Succeed.\n";
    }
    return ret;
}
