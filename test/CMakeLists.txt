add_executable(unicode_conv_test test.cpp ../UnicodeConv.cpp ../UnicodeConv.h)

add_test(
    NAME "unicode_conv_test"
    COMMAND $<TARGET_FILE:unicode_conv_test>
)