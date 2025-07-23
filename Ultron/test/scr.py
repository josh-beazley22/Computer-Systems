# Remove carriage returns from the input
def scr():
    try:
        while True:
            line = input()
            for c in line:
                if c != '\r':
                    print(c, end="")
            print()
    except:
        pass


if __name__ == "__main__":
    scr()
