import cs50


def main():
    while (True):
        height = cs50.get_int("Height: ")
        if (height > 0 and height < 9):
            break
    print_pyramid(height)


def print_pyramid(height):
    for i in range(0, height):
        for j in range(1, height + 1):
            if (j < height - i):
                print(" ", end="")
            else:
                print("#", end="")
        print("  ", end="")
        for k in range(height - i, height + 1):
            print("#", end="")
        print("")


if __name__ == "__main__":
    main()