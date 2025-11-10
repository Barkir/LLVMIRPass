; ModuleID = 'mul_smb.ll'
source_filename = "mul_smb.ll"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define dso_local noundef i32 @_Z3muliiii(i32 noundef %a, i32 noundef %b, i32 noundef %c, i32 noundef %d) local_unnamed_addr #0 {
entry:
  %cmp = icmp eq i32 %a, 500
  %cmp1 = icmp eq i32 %d, 700
  %or.cond = and i1 %cmp, %cmp1
  %add = add nuw nsw i32 %a, 5
  %cmp3 = icmp eq i32 %c, %add
  %or.cond7 = select i1 %or.cond, i1 %cmp3, i1 false
  %mul6 = shl nsw i32 %a, 1
  %mul6.sink = select i1 %or.cond7, i32 1000, i32 %mul6
  %mul7 = mul nsw i32 %c, %b
  %add8 = add nsw i32 %mul7, %mul6.sink
  ret i32 %add8
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) }
