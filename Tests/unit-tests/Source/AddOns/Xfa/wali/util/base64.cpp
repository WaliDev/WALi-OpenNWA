#include "gtest/gtest.h"

#include <sstream>

#include <wali/util/base64.hpp>

#define NUM_ELEMENTS(array)  (sizeof(array)/sizeof((array)[0]))

static std::string idempotent_tests[] = {
    "",
    "a",
    "hello world",
    "\x10\x20\x30\x40\x50\x60\x70\x80\x90\xA0\xB0\xC0\xD0\xE0\xF0\xFF"
};

static const unsigned int num_idempotent_tests = NUM_ELEMENTS(idempotent_tests);


unsigned char const red_dot_png[] =
    "\x50\x89\x47\x4e\x0a\x0d\x0a\x1a\x00\x00\x0d\x00\x48\x49\x52\x44"
    "\x00\x00\x05\x00\x00\x00\x05\x00\x06\x08\x00\x00\x8d\x00\x26\x6f"
    "\x00\xe5\x00\x00\x49\x1c\x41\x44\x08\x54\x63\xd7\xff\xf8\x3f\xff"
    "\x7f\xc3\x20\x06\xc3\x05\x12\x20\xd0\x84\xf1\x31\x58\x82\x04\xcd"
    "\x0e\x00\x35\xf5\xd1\xcb\x0e\x8e\x00\x1f\x00\x00\x49\x00\x4e\x45"
    "\xae\x44\x60\x42\x00\x82";


namespace wali {
    namespace util {

        std::string
        encode(std::string const & s)
        {
            return base64_encode(reinterpret_cast<unsigned char const *>(s.c_str()),
                                 s.size());
        }
        

        TEST(wali$util$$base64_encode$$base64_decode, batteryOfIdempotency)
        {
            for (unsigned word = 0; word < num_idempotent_tests; ++word) {
                std::stringstream ss;
                ss << "Current test number " << word;
                SCOPED_TRACE(ss.str());

                std::string
                    original = idempotent_tests[word],
                    encoded  = encode(original),
                    decoded  = base64_decode(encoded);

                if (original != "") {
                    EXPECT_NE(original, encoded);
                }
                EXPECT_EQ(original, decoded);
            }
        }

        TEST(wali$util$$base64_encode, websiteExample)
        {
            std::string
                original = "Man is distinguished, not only by his reason, but ...",
                actual = encode(original),                
                expected = "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCAuLi4=";
            
            EXPECT_EQ(expected, actual);
        }


    }
}


