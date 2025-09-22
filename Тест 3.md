## Тест 3
```cpp
int test1(int a, int b, int c) {
if (a == 46346 && b == 5745 && c == 5)
	return a * b;

return a * b;

}
```

#### IR до изменений в пайплайне
```cpp
define dso_local i32 @test1(i32 noundef %a, i32 noundef %b, i32 noundef %c) local_unnamed_addr {
entry:
	%cmp = icmp eq i32 %a, 46346
	%cmp1 = icmp eq i32 %b, 5745
	%or.cond = and i1 %cmp, %cmp1
	%cmp3 = icmp eq i32 %c, 5
	%or.cond5 = and i1 %or.cond, %cmp3
	%mul4 = mul nsw i32 %b, %a
	%retval.0 = select i1 %or.cond5, i32 266257770, i32 %mul4
	ret i32 %retval.0
}
```

#### IR после изменений в пайплайне
```cpp
define dso_local noundef i32 @_Z5test1iii(i32 noundef %a, i32 noundef %b, i32 noundef %c) local_unnamed_addr #0 {
entry:
  %mul = mul nsw i32 %b, %a
  ret i32 %mul
}
```
