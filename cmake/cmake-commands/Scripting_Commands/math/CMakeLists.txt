math(EXPR value "100 * 0xA" OUTPUT_FORMAT DECIMAL)      # value is set to "1000"
math(EXPR value1 "100 * 0xA" OUTPUT_FORMAT HEXADECIMAL)  # value is set to "0x3e8"
message("100 * 0xA = ${value}")
message("100 * 0xA = ${value1}")
