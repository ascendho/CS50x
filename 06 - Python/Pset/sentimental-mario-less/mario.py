def main():

    while True:
        try:
            height = int(input("Height: "))
            if 1 <= height <= 8:
                break
            else:
                print("Height must be between 1 and 8")
        except ValueError:
            print("Please enter a valid integer")

    for i in range(height):

        spaces = " " * (height - i - 1)

        hashes = "#" * (i + 1)
        print(spaces + hashes)


if __name__ == "__main__":
    main()
