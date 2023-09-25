# Prompt user for pyramid height
while True:
    try:
        height = int(input("Height: "))
        if height < 1 or height > 8:
            print("Please enter a positive number between 1 and 8")
            continue
        break
    except ValueError:
        print("Please enter a number.")

# Print pyramid
for i in range(height):
    print(" " * (height - (1 + i)), end="")
    print("#" * (1 + i), end="")
    print("  ", end="")
    print("#" * (1 + i))