// Test 5: Undef Value Propagation
// The pass likely assumes all instructions have defined values.
// When it tries to constant-fold 'x + 5' where x is undef, it may crash or produce wrong results.
int test_undef_propagation(int a, int b) {
    int x;  // Undef
    int res;
    if (a == 5) {
        int y = x + 5;  // Poison/undef value
        if (b == y) {   // Comparison with undef
            res = 10;
        } else {
            res = 10;
        }
    } else {
        res = 10;
    }
    return res;
}
