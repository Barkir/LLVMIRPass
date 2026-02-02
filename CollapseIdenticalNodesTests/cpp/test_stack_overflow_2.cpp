// Test 10: Deep Expression Recursion (Stack Overflow)
// CountValueRecursively traverses the def-use chain. With a long chain of adds,
// it will blow the stack if no depth limit is implemented.
int test_deep_recursion(int a) {
    int res;
    if (a == 1) {
        int x = a + a + a + a + a + a + a + a +
                a + a + a + a + a + a + a + a +
                a + a + a + a + a + a + a + a;  // 24 adds, deeply nested in LLVM IR
        if (x == 24) {
            res = 100;
        } else {
            res = 100;
        }
    } else {
        res = 100;
    }
    return res;
}
