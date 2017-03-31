import cs50

def print_pyramid(h):
    if h == 0:
        return 1
    else:
        # width = height + 1
        w = h + 1
        # iterate through each row
        for r in range(h):
            # in each row, print (h - r) spaces
            for s in range(h - r):
                print(" ", end="")
            # then print (r + 1) # signs
            for p in range(r + 2):
                print("#", end="")
            # new line
            print("\n", end="")
    
def main():
    while True:
        print("Height: ", end="")
        h = cs50.get_int()
        if h >= 0 and h <= 23:
            break
    
    print_pyramid(h)
    
    return 0
    
if __name__ == "__main__":
    main()