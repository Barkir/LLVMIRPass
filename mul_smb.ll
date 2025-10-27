define dso_local noundef i32 @_Z3muliiii(i32 noundef %a, i32 noundef %b, i32 noundef %c, i32 noundef %d) #0 {
entry:
  %cmp = icmp eq i32 %a, 500
  %cmp1 = icmp eq i32 %d, 700
  %or.cond = and i1 %cmp, %cmp1
  %add = add nsw i32 %a, 5
  %cmp3 = icmp eq i32 %c, %add
  %or.cond7 = select i1 %or.cond, i1 %cmp3, i1 false
  br i1 %or.cond7, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %mul4 = mul nsw i32 %b, %c
  %add5 = add nsw i32 %mul4, 1000
  br label %return

if.end:                                           ; preds = %entry
  %mul6 = shl nsw i32 %a, 1
  %mul7 = mul nsw i32 %b, %c
  %add8 = add nsw i32 %mul6, %mul7
  br label %return

return:                                           ; preds = %if.end, %if.then
  %retval.0 = phi i32 [ %add5, %if.then ], [ %add8, %if.end ]
  ret i32 %retval.0
}
