for f in no_pass/ll_opt/*.ll; do
    target="CollapseIdenticalNodesTests/ll_opt/$(basename "$f")"
    if [ -f "$target" ]; then
        echo "$target"
        diff "$f" "$target"
    else
        echo "Нет пары для $f"
    fi
done
