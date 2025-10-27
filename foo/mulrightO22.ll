; ModuleID = 'mulright_tests.ll'
source_filename = "mulright_tests.ll"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define dso_local noundef i32 @_Z3muliiii(i32 noundef %a, i32 noundef %b, i32 noundef %c, i32 noundef %d) local_unnamed_addr #0 {
entry:
  %cmp = icmp eq i32 %a, 500
  %cmp1 = icmp eq i32 %d, 700
  %or.cond = and i1 %cmp, %cmp1
  %add = add nuw nsw i32 %a, 5
  %cmp3 = icmp eq i32 %c, %add
  %or.cond9 = select i1 %or.cond, i1 %cmp3, i1 false
  br i1 %or.cond9, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %div = mul i32 %b, 500
  %mul = mul nsw i32 %c, 700
  %add4 = add nsw i32 %mul, %div
  br label %return

if.end:                                           ; preds = %entry
  %div5 = mul i32 %b, %a
  %mul6 = mul nsw i32 %d, %c
  %add7 = add nsw i32 %mul6, %div5
  br label %return

return:                                           ; preds = %if.end, %if.then
  %retval.0 = phi i32 [ %add4, %if.then ], [ %add7, %if.end ]
  ret i32 %retval.0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }
