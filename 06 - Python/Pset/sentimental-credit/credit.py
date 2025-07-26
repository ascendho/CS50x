def luhn_check(card_number):
    total = 0
    reverse_digits = card_number[::-1]
    for i, digit in enumerate(reverse_digits):
        n = int(digit)
        if i % 2 == 1:
            n *= 2
            if n > 9:
                n -= 9
        total += n
    return total % 10 == 0


def get_card_type(card_number):
    length = len(card_number)
    if length == 15 and card_number.startswith(('34', '37')):
        return "AMEX"
    elif length == 16 and card_number.startswith(tuple(str(i) for i in range(51, 56))):
        return "MASTERCARD"
    elif length in [13, 16] and card_number.startswith('4'):
        return "VISA"
    else:
        return "INVALID"


def main():
    card_number = input("Number: ").strip()
    if not card_number.isdigit():
        print("INVALID")
        return
    if luhn_check(card_number):
        print(get_card_type(card_number))
    else:
        print("INVALID")


if __name__ == "__main__":
    main()
