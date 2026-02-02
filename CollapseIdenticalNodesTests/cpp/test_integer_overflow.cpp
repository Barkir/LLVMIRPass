// Test 1: Integer Overflow Mismatch
// The pass likely uses APInt or 64-bit math, but the actual IR uses 32-bit wraparound.
// If it calculates 2000000000 + 2000000000 = 4000000000 (64-bit) instead of -294967296 (32-bit),
// it will incorrectly think the phi node values differ.
int test_overflow_mismatch(int a, int b) {
    int res;
    if (a == 2000000000) {
        if (b == 2000000000) {
            res = -294967296;  // (s32)(2000000000 + 2000000000)
        } else {
            res = a + b;       // Computes to same value above if b=2000000000
        }
    } else {
        res = 0;
    }
    return res;
}
