def calc(w, b, step):
    return ((((w * calc(w - 1, b + 1, step - 1)) if w > 0 else 0) + ((b * calc(w + 1, b - 1, step - 1)) if b > 0 else 0)) / (w + b)) if step > 1 else w / (w + b)


def checkio(marbles, step):
    w = b = 0
    for x in marbles:
        if x == 'w':
            w += 1
        else:
            b += 1
    return round(calc(w, b, step), 2)

#These "asserts" using only for self-checking and not necessary for auto-testing
if __name__ == '__main__':
    print(checkio('www', 3))
    assert checkio('bbw', 3) == 0.48, "1st example"
    assert checkio('wwb', 3) == 0.52, "2nd example"
    assert checkio('www', 3) == 0.56, "3rd example"
    assert checkio('bbbb', 1) == 0, "4th example"
    assert checkio('wwbb', 4) == 0.5, "5th example"
    assert checkio('bwbwbwb', 5) == 0.48, "6th example"