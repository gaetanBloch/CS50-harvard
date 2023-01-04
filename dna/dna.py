import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # Read database file into a variable
    sequences = get_sequences()
    data = get_dna_data(sequences)
    # print(data)

    # Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as seq_file:
        full_sequence = seq_file.read()
    # print(sequence)

    longest_matches = get_longest_matches(sequences, full_sequence)
    # print(longest_matches)

    # Check database for matching profiles
    found = {}
    for match_sequence, match in longest_matches.items():
        id = find_match(data, match_sequence, match, found)
        if id != "No match":
            break

    print(id)
    sys.exit(0)


def find_match(data, match_sequence, match, found):
    id = "No match"
    for person, sequences in data.items():
        search_sequences(sequences, person, match_sequence, match, found)
        if len(sequences) == found[person]:
            id = person
            break
    return id


def search_sequences(sequences, person, match_sequence, match, found):
    if found.get(person) is None:
        # Initialize dic entry
        found[person] = 0
    for sequence, count in sequences.items():
        if match_sequence == sequence and match == int(count):
            found[person] += 1


def get_sequences():
    """Get the sequences identifiers"""
    str_sequences = []
    with open(sys.argv[1], "r") as data_file:
        reader = csv.reader(data_file)
        for row in reader:
            str_sequences += row[1:]
            # Only take the first row (header)
            break
    return str_sequences


def get_dna_data(str_sequences):
    """Get the DNA data"""
    data = {}
    with open(sys.argv[1], "r") as data_file:
        reader = csv.DictReader(data_file)
        for row in reader:
            data[row["name"]] = merge_sequences(str_sequences, row)
    return data


def merge_sequences(str_sequences, row):
    strs = {}
    for i in range(len(str_sequences)):
        sequence = str_sequences[i]
        strs[sequence] = row[sequence]
    return strs


def get_longest_matches(sequences, sequence):
    """Find longest match of each STR in DNA sequence"""
    longest_matches = {}
    for seq in sequences:
        longest_matches[seq] = longest_match(sequence, seq)
    return longest_matches


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in sequence, return longest run found
    return longest_run


if __name__ == "__main__":
    main()
