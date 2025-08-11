
| Имя функции            | Причина, по которой пасс не сработал | Ссылка на функцию                                                 |
| ---------------------- | ------------------------------------ | ----------------------------------------------------------------- |
| reverse                | Деление на константу                 | [reverse](#reverse)                |
| map                    | Деление на константу                 | [map](#map)                    |
| updateIndicesAndResult | Деление на константу                 | [updateIndicesAndResult](#updateIndicesAndResult) |
| heapSort               | Деление на константу                 | [heapSort](#heapSort)               |
| milliNow               | Деление на константу                 | [milliNow](#milliNow)               |
|                        |                                      |                                                                   |
|                        |                                      |                                                                   |
|                        |                                      |                                                                   |

[](#)

# reverse
```cpp
    public reverse(): this {
        for (let i = 0; i < this.actualLength / 2; i++) {
            const tmp = this.$_get_unsafe(i);
            const idx_r = this.actualLength - 1 - i;
            const val_r = this.$_get_unsafe(idx_r);
            this.$_set_unsafe(i, val_r);
            this.$_set_unsafe(idx_r, tmp);
        }
        return this;
    }
```
# почему пасс не вносит изменение
```
this.actualLength / 2
```
- деление на константу

# map
```cpp
    public map(fn: (val: BigInt, index: number, array: BigInt64Array) => BigInt): BigInt64Array {
        let resBuf = new ArrayBuffer(this.lengthInt * BigInt64Array.BYTES_PER_ELEMENT)
        let res = new BigInt64Array(resBuf, 0, (resBuf.getByteLength() / BigInt64Array.BYTES_PER_ELEMENT).toInt())
        for (let i = 0; i < this.lengthInt; ++i) {
            res.set(i, fn(new BigInt(this.getUnsafe(i)), i.toDouble(), this).getLong())
        }
        return res
    }
```
## почему пасс не вносит изменение
`        let res = new BigInt64Array(resBuf, 0, (resBuf.getByteLength() / BigInt64Array.BYTES_PER_ELEMENT).toInt())`
- деление на константу

# updateIndicesAndResult
```cpp
internal updateIndicesAndResult(hasIndices: boolean): void {
        let data = this.resultRaw_.split(',')
        this.result_ = new Array<String>(data.length / 2)
        this.isUndefined = new Array<boolean>(data.length / 2)
        if (hasIndices) {
            this.indices_ = new Array<Array<number>>(data.length / 2)
            for (let i = 0; i < data.length; i+= 2) {
                const index = Number.parseInt(data[i])
                const endIndex = Number.parseInt(data[i + 1])
                if (index == -1 || endIndex == -1) {
                    this.indices_[i / 2] = [0, 0]
                    this.isUndefined[i / 2] = true
                } else {
                    this.indices_[i / 2] = [index, endIndex]
                    this.result_[i / 2] = this.input_.substring(index, endIndex)
                    this.processStaticFields(i / 2, this.result_[i / 2])
                    this.isUndefined[i / 2] = false
                }
            }
```

## почему пасс не вносит изменение
- здесь много делений на константу `2`


# heapSort
```cpp
export function heapSort(arr: FixedArray<short>, startIndex: int, endIndex: int): void {
    let len = endIndex - startIndex
    for (let i = len / 2 - 1; i >= 0; i--) {
        heapSortDown(arr, i, startIndex, endIndex)
    }

    for (let i = endIndex - 1; i > startIndex; i--) {
        // move max element to the end of range
        swap(arr, i, startIndex)
        heapSortDown(arr, 0, startIndex, i)
    }
}
```
## почему пасс не вносит изменение
- деление на константу

# milliNow
```cpp
    public static milliNow(): number {
        return Chrono.nanoNow() / 1000000;
    }
```
## почему пасс не вносит изменение
- деление на константу

# toString
```
    public override toString(): String {
        // Check for zero, log10 would go inf otherwise
        if (this.value == 0) {
            return '0';
        }
        let val = this.value;
        let negative: boolean = (val < 0);
        let digitsNum = Double.toInt(log10(abs(this.value)) + 1);
        if (negative) {
            ++digitsNum;
        }
        let data : FixedArray<char> = new char[digitsNum];
        let curPlace = digitsNum - 1;
        while (val != 0) {
            let remainder = val % 10;
            if (negative) {
                remainder = -remainder;
            }
            data[curPlace] = (remainder + c'0').toChar();
            val /= 10;
            curPlace -= 1;
        }
        if (negative) {
            data[0] = c'-';
        }
        return new String(data);
    }
```
## почему пасс не вносит изменение
- деление на константу
