// Test 2: Division by Zero During Constant Evaluation
// The pass recursively evaluates expressions. When it substitutes a=10, denominator becomes 0.
// This should crash the pass if it doesn't check for division by zero during folding.
int test_div_by_zero_fold(int a, int b) {
    int res;
    if (a == 10) {
        int denom = 10 - a;  // Always 0 when a==10
        if (b == 5) {
            res = 100;
        } else {
            res = a / denom; // Division by zero!
        }
    } else {
        res = 100;
    }
    return res;
}
