## Тест 1
```cpp
int mul_intV1(int a, int b, int c, int d) {
	int res;
	if (a == 500 && d == 700 && c == a + 5) {
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
	%cmp = icmp eq i32 %a, 500
	%cmp1 = icmp eq i32 %d, 700
	%or.cond = and i1 %cmp, %cmp1
	%cmp3 = icmp eq i32 %c, 505
	%or.cond17 = and i1 %cmp3, %or.cond
	br i1 %or.cond17, label %if.then, label %if.else

if.then:
	%mul4 = mul nsw i32 %b, 252500
	%add5 = add nsw i32 %mul4, 700
	br label %if.end
	
if.else:
	%mul6 = mul nsw i32 %b, %a
	%mul7 = mul nsw i32 %mul6, %c
	%add8 = add nsw i32 %mul7, %d
	br label %if.end

if.end:
	%res.0 = phi i32 [ %add5, %if.then ], [ %add8, %if.else ]
	ret i32 %res.0
}
```

#### IR после изменений в пайплайне
```cpp
define dso_local noundef i32 @_Z9mul_intV1iiii(i32 noundef %a, i32 noundef %b, i32 noundef %c, i32 noundef %d) local_unnamed_addr #0 {
entry:
  %mul7.pn.v = mul i32 %b, %a
  %mul7.pn = mul i32 %mul7.pn.v, %c
  %res.0 = add nsw i32 %mul7.pn, %d
  ret i32 %res.0
}

```
