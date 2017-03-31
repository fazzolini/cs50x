import cs50
from math import floor

def press_lever(value, cents, result):
    while cents >= value:
        cents -= value
        result += 1
    return cents, result

def check_coins(change):
    result = 0
    cents = int(floor(change * 100)) # truncates if values smaller than cents are provided $4.1111 -> 411 cents
    for value in [25, 10, 5, 1]:
        cents, result = press_lever(value, cents, result)
    return result

def main():
    print("O hai! How much change is owed?")
    while True:
        change = cs50.get_float()
        if change >= 0:
            break
        print("How much change is owed?")

    result = check_coins(change)
    print(result)
    return 1
    
if __name__ == "__main__":
    main()