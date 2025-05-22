# Regrep - Pattern Matching Tool

## Description

`regrep` is a simple tool for searching lines in text matching regular expressions, supporting logical operators, highlighting, replacement, and context lines.

## Usage

```bash
regrep "<pattern>" [options] < input.txt
```
### Patterns

Combine patterns with logical operators:

AND: foo&&bar — line must contain all patterns

OR: foo||bar — line contains at least one pattern

Single pattern: foo

### Options

```--invert``` — outputs lines not matching the pattern

```--highlight``` — highlights matched parts (red color)

```--replace <text>``` — replaces matched parts with <text>

```--count``` — outputs only the number of matched lines

```--before <n>``` — outputs n lines before matched line

```--after <n>``` — outputs n lines after matched line

```--showline``` — shows line numbers before the line content

Example
```bash
cat logs.txt | regrep "error||fail" --highlight --before 2 --after 2 --showline < logfile.txt
```
Finds lines containing error or fail, highlights matches, shows 2 lines of context before and after, and prints line numbers.

### Notes

Patterns use ECMAScript regular expression syntax.

The --benchmark flag is recognized but currently has no implementation.