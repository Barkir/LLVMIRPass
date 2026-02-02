// Test 4: Contradictory Path Constraints via Select
// The pass collects icmp eq constraints. If it tracks both a==5 and a==6 as true
// (because of select chain), it will have contradictory values for 'a' in the hashtable.
int test_contradictory_constraints(int a) {
    bool c1 = (a == 5);
    bool c2 = (a == 6);
    bool both = c1 ? c2 : false;  // Always false, but pass might track both constraints

    if (both) {
        return 10;  // Unreachable, but if pass thinks a==5 && a==6, math breaks
    }
    return 10;
}
