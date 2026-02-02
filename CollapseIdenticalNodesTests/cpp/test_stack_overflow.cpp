// Test 3: Cyclic PHI Dependence (Fibonacci in loop)
// The pass uses CountValueRecursively. With cyclic phis (mutual recursion),
// it will either infinite recurse or use unvisited tracking incorrectly.
int test_cyclic_phi(int n, int init) {
    int result;
    if (init == 1) {
        int x = 0, y = 1;
        for (int i = 0; i < n; i++) {
            int temp = x;
            x = y;
            y = temp + y;  // Creates cyclic dependency through PHIs
        }
        result = x;
    } else {
        result = 0;
    }
    return result;
}
