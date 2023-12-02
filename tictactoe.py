from time import sleep
from os import system
from random import randint

def menu():
    system("cls")
    print("Tic-tac-toe -> by adobko")
    print("Menu:\n1) Singleplayer\n2) Multiplayer\n3) Exit")
    match input("Choose how to proceed: "):
        case "1": 
            while True:
                match input("Want to begin first? (y/n): "):
                    case "y": playerFirst = True
                    case "n": playerFirst = False
                    case _:
                        print("Please input (y/n)!")
                        continue
                break
            main("single", playerFirst)
        case "2": main()
        case "3": exit()
        case _:
            print("Please input a vallid choice!")
            sleep(1)
            menu()

def playerInput(player: int, field: list[int]) -> int:
    while True:
        coords = [*input(f"Player {player}: (xy): ")]
        try:
            x, y = coords
            y = int(y)-1
            match x.lower():
                case "a": x = 0
                case "b": x = 1
                case "c": x = 2
                case _:
                    print("X has to be a, b or c!")
                    continue
        except:
            print("Invalid input! Please input coordinates in (xy) format.")
            continue
        if not 0 <= y <= 2:
            print("Y is out of range <1,3>")
            continue
        elif field[y][x] != 0:
            print("Please enter coordinates to a not yet used field!")
            continue
        return x, y

def botInput(bid: int, pid: int, field: list[int]):
    print(f"(BOT) player {bid} turn ")
    rowIndex = -1
    for row in field:
        rowIndex += 1
        if (row.count(bid) == 2 or row.count(pid) == 2) and 0 in row:
            return row.index(0), rowIndex
    for x in range(3):
        column = []
        for y in range(3):
            column.append(field[y][x])
        if (column.count(bid) == 2 or column.count(pid) == 2) and 0 in column:
            return x, column.index(0)
    diagonal1 = []
    diagonal2 = []
    for xy in range(3):
        diagonal1.append(field[xy][xy])
        diagonal2.append(field[xy][-xy-1])
    if (diagonal1.count(bid) == 2 or diagonal1.count(pid) == 2) and 0 in diagonal1:
        return diagonal1.index(0), diagonal1.index(0)
    if (diagonal2.count(bid) == 2 or diagonal2.count(pid) == 2) and 0 in diagonal2:
        return (-diagonal2.index(0)-1), diagonal2.index(0)
    x, y = 1, 1
    while field[y][x] != 0:
        x, y = randint(0,2), randint(0,2)
    return x, y
        
def displayField(field: list[int]):
    display = "  A B C\n"
    y = 0
    for row in field:
        y += 1
        items = str(y)
        for item in row:
            match item:
                case 0: item = "-"
                case 1: item = "X"
                case 2: item = "O"
            items += f" {item}"
        display += f"{items}\n"
    print(display)

def lineCheck(line: list[int]) -> int:
    if line == [1, 1, 1]:
        return 1
    elif line == [2, 2, 2]:
        return 2
    return 0

def winCheck(field: list[int]):
    for row in field:
        if wid := lineCheck(row):
            end(field, "win", wid)
    
    for x in range(3):
        column = []
        for y in range (3):
            column.append(field[y][x])
        if wid := lineCheck(column):
            end(field, "win", wid)

    diagonal = []
    diagonal2 = []
    for xy in range(3):
        diagonal.append(field[xy][xy])
        diagonal2.append(field[xy][-xy-1])
    if wid := lineCheck(diagonal):
        end(field, "win", wid)
    if wid := lineCheck(diagonal2):
        end(field, "win", wid)

def drawCheck(field: list[int]):
    fills = 0
    for row in field:
        if 0 not in row:
            fills += 1
    if fills >= 3:
        end(field, "draw")

def newFrame(field: list[int]):
    system("cls")
    displayField(field)

def main(mode: str="multi", playerFirst: bool=False):
    system("cls")
    field = [
        [0,0,0],
        [0,0,0],
        [0,0,0]
    ]
    while True:
        # player 1
        newFrame(field)
        if mode == "multi" or playerFirst: x1, y1 = playerInput(1, field)
        else: x1, y1 = botInput(1, 2, field)
        field[y1][x1] = 1
        winCheck(field)
        
        drawCheck(field)

        # player 2
        newFrame(field)
        if mode == "multi" or not playerFirst: x2, y2 = playerInput(2, field)
        else: x2, y2 = botInput(2, 1, field)
        field[y2][x2] = 2
        winCheck(field)

def end(field: list[int], ending: str, player: int=None):
    newFrame(field)
    if ending == "win":
        print("       _      _                   \n      (_)    | |                  \n__   ___  ___| |_ ___  _ __ _   _ \n\ \ / / |/ __| __/ _ \| '__| | | |\n \ V /| | (__| || (_) | |  | |_| |\n  \_/ |_|\___|\__\___/|_|   \__, |\n                             __/ |\n                            |___/ ")
        print(f"Plyer {player} has WON!")
    elif ending == "draw":
        print("Draw!")
        print("The field was filled yet noone has won...")
    system("pause")
    menu()

system("color 70")
menu()
