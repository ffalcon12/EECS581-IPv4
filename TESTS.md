# Test Cases

The program was tested against the instructor examples and additional edge cases.

| # | Input | Expected result |
|---|---|---|
| 1 | `connecting to 192.168.1.1 now` | Valid: `192.168.1.1`, decimal `3232235777`, no port |
| 2 | `server=10.0.0.255:8080end` | Valid: `10.0.0.255`, decimal `167772415`, port `8080` |
| 3 | `192a168.1.1.1` | Valid: `168.1.1.1`, decimal `2818638081`, no port |
| 4 | `192.168.1.1.` | Invalid |
| 5 | `Connection from 192.168.1.1 refused` | Valid: `192.168.1.1` |
| 6 | `192.168.01.1` | Invalid: leading zero |
| 7 | `1.2.3.4:99999` | Invalid: port out of range |
| 8 | `12.34.56` | Invalid: only three octets |
| 9 | `no number here` | Invalid |
| 10 | `0.0.0.0` | Valid: decimal `0`, no port |
| 11 | `255.255.255.255` | Valid: decimal `4294967295`, no port |
| 12 | `256.1.1.1` | Invalid: octet out of range |
| 13 | `1.2.3.4:65535` | Valid: maximum port |
| 14 | `1.2.3.4:65536` | Invalid: port out of range |
| 15 | `1.2.3.4:0` | Valid: port `0` |
| 16 | `1.2.3.4:00` | Invalid: leading zero in port |
| 17 | `1.2.3.4:` | Invalid: missing port digits |
| 18 | `1.2.3.4:80:90` | Invalid: second colon |
| 19 | `.1.2.3.4` | Invalid: stray adjacent period |
| 20 | `1.2.3.4.5` | Invalid: fifth octet / adjacent period |
| 21 | `1..2.3.4` | Invalid: empty octet |
| 22 | `1.2.3..4` | Invalid: empty octet |
| 23 | `01.2.3.4` | Invalid: leading zero |
| 24 | `000.2.3.4` | Invalid: leading zero |
| 25 | `abc1.2.3.4xyz` | Valid: `1.2.3.4` |
| 26 | `abc 999.999.999.999 xyz 1.2.3.4 ok` | Valid: skips failed candidate and finds `1.2.3.4` |
| 27 | `1.2.3.4:123456` | Invalid: six-digit port |
| 28 | `1234.2.3.4` | Invalid: four-digit octet |
| 29 | `1.2.3.4::80` | Invalid: double colon |
| 30 | `END` | Terminates program and prints `Program terminated.` |

## Important interpretation tested

A candidate token is treated as the entire contiguous run consisting only of digits, periods, and colons. Therefore a malformed run is not shortened to recover a valid prefix.

Examples:

- `192.168.1.1.` is invalid rather than extracting `192.168.1.1`.
- `1.2.3.4:99999` is invalid rather than extracting `1.2.3.4`.
- `1.2.3.4.5` is invalid rather than extracting `1.2.3.4`.

Garbage characters terminate a failed candidate and allow scanning to continue afterward. For example, `192a168.1.1.1` extracts `168.1.1.1`.
