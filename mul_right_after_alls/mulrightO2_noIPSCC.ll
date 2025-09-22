; ModuleID = 'mul_right.ll'
source_filename = "mul_right.ll"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define dso_local noundef i32 @_Z5test1iii(i32 noundef %a, i32 noundef %b, i32 noundef %c) local_unnamed_addr #0 {
entry:
  %cmp = icmp eq i32 %a, 46346
  %cmp1 = icmp eq i32 %b, 5745
  %or.cond = and i1 %cmp, %cmp1
  %cmp3 = icmp eq i32 %c, 5
  %or.cond1 = and i1 %or.cond, %cmp3
  br i1 %or.cond1, label %return, label %if.end

if.end:                                           ; preds = %entry
  %mul4 = mul nsw i32 %b, %a
  %div5 = sdiv i32 %mul4, %c
  br label %return

return:                                           ; preds = %entry, %if.end
  %retval.0 = phi i32 [ %div5, %if.end ], [ 53251554, %entry ]
  ret i32 %retval.0
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }
