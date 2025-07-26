def main():

    while True:
        try:
            change = float(input("Change owed: "))
            if change >= 0:
                break
            else:
                print("Please enter a non-negative number.")
        except ValueError:
            print("Please enter a valid number.")

    cents = round(change * 100)

    quarters = 25
    dimes = 10
    nickels = 5
    pennies = 1

    coins = 0

    coins += cents // quarters
    cents %= quarters

    coins += cents // dimes
    cents %= dimes

    coins += cents // nickels
    cents %= nickels

    coins += cents // pennies

    print(coins)


if __name__ == "__main__":
    main()
