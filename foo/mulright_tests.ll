define dso_local noundef i32 @_Z3muliiii(i32 noundef %a, i32 noundef %b, i32 noundef %c, i32 noundef %d) #0 {
entry:
  %cmp = icmp eq i32 %a, 500
  %cmp1 = icmp eq i32 %d, 700
  %or.cond = and i1 %cmp, %cmp1
  %add = add nsw i32 %a, 5
  %cmp3 = icmp eq i32 %c, %add
  %or.cond9 = select i1 %or.cond, i1 %cmp3, i1 false
  br i1 %or.cond9, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %div = mul i32 %a, %b
  %mul = mul nsw i32 %c, %d
  %add4 = add nsw i32 %div, %mul
  br label %return

if.end:                                           ; preds = %entry
  %div5 = mul i32 %a, %b
  %mul6 = mul nsw i32 %c, %d
  %add7 = add nsw i32 %div5, %mul6
  br label %return

return:                                           ; preds = %if.end, %if.then
  %retval.0 = phi i32 [ %add4, %if.then ], [ %add7, %if.end ]
  ret i32 %retval.0
}
