
| Имя функции       | Причина по, которой пасс сработал          | Ссылка на исходный код функции                                         |     |
| ----------------- | ------------------------------------------ | ---------------------------------------------------------------------- | --- |
| getNearestDivider | Деление неизвестных аргументов функции     | [getNearestDivider](#getNearestDivider) |     |
| normMilliSecond   | Деление неизвестных компилятору переменных | [normMilliSecond](#normMilliSecond)   |     |
| getDuration       | Деление неизвестных компилятору переменных | [getDuration](#getDuration)       |     |
| div               | Деление неизвестных компилятору переменных | [div](#div)               |     |
|                   |                                            |                                                                        |     |
|                   |                                            |                                                                        |     |
|                   |                                            |                                                                        |     |




# getNearestDivider

```cpp
private getNearestDivider(divident: BigInt, divisor: BigInt): [Long, BigInt] {
        let sub = divident - divisor
        if (sub.negative()) {
            // i.e. divisor is greater than divident, so the latter is a remainder
            return [0, divident]
        }
        if (divident < MAX_REGULAR_INTEGER) {
            let dividentLong: long = divident.getLong()
            let divisorLong: long = divisor.getLong()
            let quotent: long = dividentLong / divisorLong
            let remainder: long = dividentLong % divisorLong
            return [quotent, new BigInt(remainder)]
        }
        let from: long = 1
        let to: long = BigInt.baseSys - (1 as long)
        let cnt: long = BigInt.getCnt(from, to)
        let res2: BigInt

        let done = false
        do {
            let res1 = divident - ((new BigInt(cnt)) * divisor)
            let b1: boolean = res1.negative()
            if (res1.isZero())
            {
                return [cnt, res1]
            }
            res2 = divident - ((new BigInt(cnt - 1)) * divisor)
            let b2: boolean = res2.positive()
            if (res2.isZero())
            {
                cnt--
                done = true
                return [cnt, res2]
            }

            if (b1 && b2)
            {
                cnt--
                done = true
            }
            if (!b1) {
                from = cnt
                cnt = BigInt.getCnt(from, to)

            }
            if (!b2) {
                to = cnt
                cnt = BigInt.getCnt(from, to)
            }
        } while (!done)

        return [cnt, res2]
    }
```

## куда вносит изменение пасс
`let quotent: long = dividentLong / divisorLong`
- здесь происходит деление неизвестных компилятору аргументов функции, из-за чего наш пасс здесь отлично работает


# normMilliSecond
```
  static normMilliSecond(sec: DateUnit): int {
    const len = sec.getLength();
    const value = sec.getValue();
    if (len === 3) {
      return value;
    }
    if (len === 2) {
      return value * 10;
    }
    if (len === 1) {
      return value * 100;
    }
    let divisor = 1;
    let remainingLen = len;
    while (remainingLen > 3) {
      divisor *= 10;
      remainingLen--;
    }
    return Math.floor(value / divisor).toInt();
  }
```

## куда вносит изменение пасс
```
return Math.floor(value / divisor).toInt();
```
- деление неизвестных компилятору констант


# getDuration
```
internal getDuration(): int {
            return ((Chrono.nanoNow() - this.startTime) / Chrono.NS_PER_MS).toInt();
        }
```
## куда вносит изменение пасс
- очев лол
- деление неизвестных компилятору констант


# div
```
    public div(other: Byte): Byte {
        return (this.value / other.toByte()).toByte()
    }
```
# куда вносит изменение пасс
- очев
- деление неизвестных компилятору переменных
