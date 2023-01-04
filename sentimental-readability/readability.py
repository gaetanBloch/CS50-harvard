import cs50
import sys


def main():
    text = cs50.get_string("Text: ")
    letters = count_letters(text)
    # print(f"l: {letters}")
    words = count_words(text)
    # print(f"w: {words}")
    sentences = count_sentences(text)
    # print(f"s: {sentences}")
    index = compute_index(letters, words, sentences)

    if (index < 1):
        print("Before Grade 1")
        sys.exit(0)

    if (index > 16):
        print("Grade 16+")
        sys.exit(0)

    print(f"Grade {index}")


def count_letters(text):
    count = 0
    for i in range(len(text)):
        if text[i].isalpha():
            count += 1
    return count


def count_words(text):
    count = 0
    for i in range(len(text)):
        c = text[i]
        if c == " ":
            count += 1
    count += 1
    return count


def count_sentences(text):
    count = 0
    for i in range(len(text)):
        c = text[i]
        if c == "." or c == "?" or c == "!":
            count += 1
    return count


def compute_index(letters, words, sentences):
    """Calculate Coleman-Liau index"""

    L = (letters * 100) / words
    S = (sentences * 100) / words
    index = 0.0588 * L - 0.296 * S - 15.8
    return round(index)


if __name__ == "__main__":
    main()