# Questions

## What's `stdint.h`?

It's a library that gives us a set of defined variable types (for integers) that have specific length requirements.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

This allows for greater specificity in each variable. There are several benefits. Firstly, it helps with error-catching,
because these variable types demand specific lengths. If we used less-specific variable types, there would be more room
for these variables to end up with a weird, unexpected value. Secondly, it helps with portability across systems where
less-specific variable types might have differing meanings.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

A `BYTE` is 8 bits, `DWORD` is 32 bits, `LONG` is 32 bits, and `WORD` is 16 bits.

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

42 4D (https://en.wikipedia.org/wiki/List_of_file_signatures)

## What's the difference between `bfSize` and `biSize`?

`biSize` is the bytes required by the structure `BITMAPINFOHEADER`, whereas `bfSize` is the size of the entire file.

## What does it mean if `biHeight` is negative?

The RGB Triples represent, in order, the pixels in the image from the top left corner across and down (as opposed to
from the reverse, from bottom right up).

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

`biBitCount`.

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

`fopen` would return `NULL` if there were a problem opening the file.

## Why is the third argument to `fread` always `1` in our code?

Because, throughout this code, we are reading in one unit at a time. That is, we have
several types of structs and each time we read from the file, our code deals with an
individual instance of that struct before moving on. This makes sense as a design choice
because, if we did otherwise, we'd be lumping several distinct pieces of data together
and then we'd have to do the work to separate them again in a different place, anyway!
That'd be a bit silly, in pretty much all the cases I can think of.

## What value does line 63 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

1.

## What does `fseek` do?

It moves the file pointer (first input) the specified number of bytes (second input) from
the specified point (third input)

## What is `SEEK_CUR`?

It's an option for the third inputted variable in fseek. It tells fseek to offset from the
current position of the file pointer.
