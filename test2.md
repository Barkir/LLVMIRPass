## Тест 2
```cpp
int mul_intV1(int a, int b, int c, int d) {
	int res;
	if (a == b + d && c == a && d == 700) {
	res = a * c * b + d;
	} else {
		res = a * b * c + d;
	}
	return res;
}
```

#### IR до изменений в пайплайне
```cpp
define dso_local i32 @mul_intV1(i32 noundef %a, i32 noundef %b, i32 noundef %c, i32 noundef %d) local_unnamed_addr {
entry:
	%add = add nsw i32 %d, %b
	%cmp = icmp eq i32 %a, %add
	br i1 %cmp, label %land.lhs.true, label %if.else
	land.lhs.true:
	%cmp1 = icmp eq i32 %c, %a
	%cmp3 = icmp eq i32 %d, 700
	%or.cond = and i1 %cmp1, %cmp3
	br i1 %or.cond, label %if.end, label %if.else

if.else:
	br label %if.end

if.end:
	%c.sink = phi i32 [ %c, %if.else ], [ %a, %land.lhs.true ]
	%d.sink = phi i32 [ %d, %if.else ], [ 700, %land.lhs.true ]
	%mul6 = mul i32 %b, %a
	%mul7 = mul i32 %mul6, %c.sink
	%add8 = add nsw i32 %mul7, %d.sink
	ret i32 %add8
}
```

#### IR после изменений в пайплайне
```cpp
define dso_local noundef i32 @_Z9mul_intV1iiii(i32 noundef %a, i32 noundef %b, i32 noundef %c, i32 noundef %d) local_unnamed_addr #0 {
entry:
  %mul4.pn.v = mul i32 %b, %a
  %mul4.pn = mul i32 %mul4.pn.v, %c
  %res.0 = add nsw i32 %mul4.pn, %d
  ret i32 %res.0
}

```
