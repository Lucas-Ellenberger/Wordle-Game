#!/usr/bin/env python3

"""
A quick approximation of wordle for you to mess with to get a sense about how
the rules work.
"""

import random

def main():
    solution_words = set()
    with open("vocabulary.txt") as infile:
        for line in infile:
            word = line.strip()
            if line:
                solution_words.add(word)


    secret_word = random.choice(list(solution_words))

    tries = 0
    while True:
        guess = input("enter your guess: ")
        if guess not in solution_words:
            print("invalid guess")
            continue

        tries += 1
        if guess == secret_word:
            print("you win! got it in {} tries".format(tries))
            break
        else:
            response = []
            for (gc, sc) in zip(guess, secret_word):
                if gc == sc:
                    response.append("g")
                elif gc in secret_word:
                    response.append("y")
                else:
                    response.append("x")
            print("".join(response))

if __name__ == "__main__": main()
