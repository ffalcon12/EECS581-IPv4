# A1: Extracting IPv4 Addresses from Noisy Text

EECS 581 — Software Engineering II  
Fall 2026

## Description

This C++ program scans a line of noisy text and extracts one valid IPv4 address, optionally followed by a port number.

The parser is implemented manually, character by character. It does not use:

- `atoi`, `strtol`, `stoi`, or other string-to-number conversion functions
- `sscanf` or numeric `scanf` conversions
- regular expressions
- `inet_aton`, `inet_pton`, `inet_addr`, or other address-parsing libraries

## IPv4 rules

A valid address contains four octets:

```text
A.B.C.D
```

Each octet:

- contains 1–3 digits
- has a value from 0 through 255
- cannot have a leading zero unless the octet is exactly `0`

An optional port may follow:

```text
A.B.C.D:PORT
```

The port:

- contains 1–5 digits
- has a value from 0 through 65535
- cannot have a leading zero unless the port is exactly `0`

If a colon is present and the port is invalid, the complete candidate is rejected.

## Candidate-token rule

Only digits, periods, and colons can belong to a candidate token.

The parser validates the complete contiguous candidate run. It does not truncate malformed candidates to recover a valid-looking prefix.

For example:

```text
192.168.1.1.
```

is rejected as a complete malformed candidate.

However, garbage characters separate candidates. Therefore:

```text
192a168.1.1.1
```

allows the parser to reject `192`, skip `a`, and then extract `168.1.1.1`.

## Build

Using g++:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o ipv4_parser
```

## Run

```bash
./ipv4_parser
```

The program repeatedly prompts:

```text
Enter a string (or 'END' to quit):
```

Enter `END` to terminate.

## Example

```text
Enter a string (or 'END' to quit): connecting to 192.168.1.1 now
Extracted IPv4 address: 192.168.1.1 (decimal value: 3232235777, port: none)
Enter a string (or 'END' to quit): END
Program terminated.
```

## Files

- `main.cpp` — implementation
- `TESTS.md` — test cases and expected results
- `AI_DISCLOSURE.pdf` — generative-AI usage documentation
