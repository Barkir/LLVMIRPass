// Test 9: Sign Extension vs Zero Extension Confusion
// The trace shows 'sextVal' (sign extension). Testing unsigned values near max.
// If the pass sign-extends 0xFFFFFFFFU to -1 (64-bit), arithmetic will be wrong.
unsigned test_unsigned_wrap(unsigned a) {
    unsigned res;
    if (a == 0xFFFFFFFFU) {
        res = 0;  // (0xFFFFFFFFU + 1) wraps to 0
    } else {
        res = a + 1;
    }
    return res;
}
