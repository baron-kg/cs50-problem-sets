from cs50 import get_string
from re import findall
from math import ceil

# Prompt for text
text = get_string("Please enter your text: ")

# Read all words from text into list
words = findall(r"\w+", text)

# Count number of words in text list
numOfWords = len(words)

# Count number of letters in text
numOfLetters = 0
for i in text:
    if i.isalpha():
        numOfLetters += 1

# Count number of sentences in text
numOfSentences = 0
for i in text:
    if i in [".", "!", "?"]:
        numOfSentences += 1

# Calculate l and s values
l = round(numOfLetters / numOfWords * 100)
s = round(numOfSentences / numOfWords * 100)

# Calculate the Coleman-Liau index
colemanLiau = round(0.0588 * l - 0.296 * s - 15.8)

# Print grade level
if colemanLiau < 1:
    print("Before Grade 1")
elif colemanLiau > 16:
    print("Grade 16+")
else:
    print(f"Grade {colemanLiau}")