define dso_local noundef i32 @_Z5test1iii(i32 noundef %a, i32 noundef %b, i32 noundef %c) #0 {
entry:
  %cmp = icmp eq i32 %a, 46346
  %cmp1 = icmp eq i32 %b, 5745
  %or.cond = select i1 %cmp, i1 %cmp1, i1 false
  %cmp3 = icmp eq i32 %c, 5
  %or.cond1 = select i1 %or.cond, i1 %cmp3, i1 false
  br i1 %or.cond1, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %mul = mul nsw i32 %a, %b
  %div = sdiv i32 %mul, %c
  br label %return

if.end:                                           ; preds = %entry
  %mul4 = mul nsw i32 %a, %b
  %div5 = sdiv i32 %mul4, %c
  br label %return

return:                                           ; preds = %if.end, %if.then
  %retval.0 = phi i32 [ %div, %if.then ], [ %div5, %if.end ]
  ret i32 %retval.0
}
