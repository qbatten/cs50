# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

An invented word! It refers to silicosis.

## According to its man page, what does `getrusage` do?

It returns research usage statistics, such as system time used and operations performed, for "who",
which is the process referenced in the input.

## Per that same man page, how many members are in a variable of type `struct rusage`?

Sixteen.

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

Because they're structs and we want to access multiple fields within the struct. So, better to pass
the pointer and pick out the fields we need within the function than to do that work outside of it.

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from
## a file. In other words, convince us that you indeed understand how that function's `for` loop works.

First, we open the file to be checked, printf the header for the 'misspelled words' section, and
initialize some variables. The variables init'ed are: 'index', which is the place we are at within
the word we're currently reading in, used to exclude strings too long to be words; misspellings
and words, to track the number of misspelled words and total words read in, respectively; and
word[], which is a char array to hold the word currently being read in.
Next, we begin the for loop. The loop reads in one char at a time, and breaks if it reads in an
end-of-file. Within the loop is a group of if-else statements, which determine whether the current
char is: an alphabetic char or apostrophe, a digit, or if we've reached the end of a word. If it's
an alphabetic char / apostrophe, we append it to word[], then we check if the word is too long
(longer than the longest word), in which case we throw that string out. If there's a digit in the
string, we throw it out. If neither of the previous conditions are true and index > 0, then we
just reached the end of a word; in which case we add a terminating character, increment words, and
call check(word) to see if it's in our dictionary. If it was misspelled, we print it and increment
misspelled; otherwise we just reset index to prepare for the next word. In addition to these
explicitly-written conditions, there is an unspoken 'else', which occurs when index <= 0 and the
current char is not alphabetical or a digit. This condition occurs when we have completed a word
and are consuming the spaces, punctuation, etc. between the end of the last word and the beginning
of the next.
This loop breaks once we've reached the end of file, so we will have read in all words in the
document at that point.


## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?

It is not a good idea to use fscanf to read directly from any input, for several reasons. If it fails,
it can leave the file pointer in an unknown state, for one. It also will cause problems if a word ends
in something other than whitespace (e.g. a period), because fscanf() will include those in the word
string (not what we want). Finally, fscanf does not enable us to track the number of chars read in, so
we can't protect ourselves from strings longer than our 'word' array. That is not okay!

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?

We do not want those functions to alter those pointers or their pointees! Declaring them as const is
good practice because it clarifies that intent and assures us that the function is not doing that.
