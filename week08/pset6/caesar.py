import cs50
import sys

LOWER_START = 97
LOWER_END = 122
UPPER_START = 65
UPPER_END = 90

def is_lower_case(c):
    if ord(c) < LOWER_START or ord(c) > LOWER_END:
        return False
    else:
        return True
        
def is_upper_case(c):
    if ord(c) < UPPER_START or ord(c) > UPPER_END:
        return False
    else:
        return True

def is_letter(c):
    if is_lower_case(c) or is_upper_case(c):
        return True
    else:
        return False

def shift_caesar(c, shift):
    if is_letter(c):
        if is_lower_case(c):
            lower_range = LOWER_START
        else:
            lower_range = UPPER_START
            
        # perform the shift
        new_char_ix = lower_range + (ord(c) - lower_range + shift) % 26
        new_char = chr(new_char_ix)
    else:
        # non-alphabetical should be unchanged
        return c
            
    return new_char

def main():
    # handle keyword arguments or lack thereof
    if len(sys.argv) != 2:
        print("There must be one command-line argument")
        return 1

    # get the caesar shift
    shift = int(sys.argv[1]) % 26
    print("key is {}".format(shift))

    # get input text    
    print("plaintext: ", end="")
    word = cs50.get_string()

    # start output
    print("ciphertext: ", end="")
    
    # iterate through all chars in word
    for l in word:
        print("{}".format( shift_caesar(l, shift) ), end="" )
    # new line at end of word
    print()
    
    return 0
    
    
if __name__ == "__main__":
    main()