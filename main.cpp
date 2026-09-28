#include <iostream>
#include <string>

bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort) {
    outAddress = 0;
    outPort = -1;

    const std::size_t n = str.size();
    std::size_t i = 0;

    while (i < n) {
        // Skip true garbage. Digits, '.' and ':' all belong to candidate runs,
        // even when a run begins with punctuation and will later be rejected.
        while (i < n) {
            char c = str[i];
            if ((c >= '0' && c <= '9') || c == '.' || c == ':') {
                break;
            }
            ++i;
        }

        if (i >= n) {
            break;
        }

        // A candidate is the entire consecutive run made only of digits, '.' and ':'.
        const std::size_t start = i;
        std::size_t end = i;

        while (end < n) {
            char c = str[end];
            if ((c >= '0' && c <= '9') || c == '.' || c == ':') {
                ++end;
            } else {
                break;
            }
        }

        // Parse the complete candidate run. We never truncate a malformed run
        // to rescue a valid-looking IPv4 prefix from inside it.
        unsigned long octets[4] = {0, 0, 0, 0};
        std::size_t pos = start;
        bool valid = true;

        for (int octetIndex = 0; octetIndex < 4 && valid; ++octetIndex) {
            if (pos >= end || str[pos] < '0' || str[pos] > '9') {
                valid = false;
                break;
            }

            // Leading zero is allowed only when the entire field is exactly "0".
            if (str[pos] == '0') {
                octets[octetIndex] = 0;
                ++pos;

                if (pos < end && str[pos] >= '0' && str[pos] <= '9') {
                    valid = false;
                    break;
                }
            } else {
                unsigned long value = 0;
                int digitCount = 0;

                while (pos < end && str[pos] >= '0' && str[pos] <= '9') {
                    if (digitCount == 3) {
                        valid = false;
                        break;
                    }

                    value = value * 10 + static_cast<unsigned long>(str[pos] - '0');
                    ++digitCount;
                    ++pos;
                }

                if (!valid || value > 255) {
                    valid = false;
                    break;
                }

                octets[octetIndex] = value;
            }

            if (octetIndex < 3) {
                if (pos >= end || str[pos] != '.') {
                    valid = false;
                    break;
                }
                ++pos;
            }
        }

        int port = -1;

        if (valid && pos < end && str[pos] == ':') {
            ++pos;

            if (pos >= end || str[pos] < '0' || str[pos] > '9') {
                valid = false;
            } else if (str[pos] == '0') {
                port = 0;
                ++pos;

                if (pos < end && str[pos] >= '0' && str[pos] <= '9') {
                    valid = false;
                }
            } else {
                unsigned long portValue = 0;
                int digitCount = 0;

                while (valid && pos < end && str[pos] >= '0' && str[pos] <= '9') {
                    if (digitCount == 5) {
                        valid = false;
                        break;
                    }

                    portValue = portValue * 10 + static_cast<unsigned long>(str[pos] - '0');
                    ++digitCount;
                    ++pos;
                }

                if (valid && portValue > 65535) {
                    valid = false;
                }

                if (valid) {
                    port = static_cast<int>(portValue);
                }
            }
        }

        // The candidate must be consumed in full. Any leftover '.', ':', or digit
        // makes the whole candidate invalid.
        if (valid && pos == end) {
            outAddress =
                (octets[0] << 24) |
                (octets[1] << 16) |
                (octets[2] << 8)  |
                octets[3];

            outPort = port;
            return true;
        }

        // Continue after the entire failed candidate run.
        i = end;
    }

    outAddress = 0;
    outPort = -1;
    return false;
}

int main() {
    std::string input;

    while (true) {
        std::cout << "Enter a string (or 'END' to quit): ";
        std::getline(std::cin, input);

        if (!std::cin) {
            break;
        }

        if (input == "END") {
            std::cout << "Program terminated." << std::endl;
            break;
        }

        unsigned long address = 0;
        int port = -1;

        if (extractIPv4(input, address, port)) {
            unsigned long a = (address >> 24) & 255UL;
            unsigned long b = (address >> 16) & 255UL;
            unsigned long c = (address >> 8) & 255UL;
            unsigned long d = address & 255UL;

            std::cout << "Extracted IPv4 address: "
                      << a << "." << b << "." << c << "." << d
                      << " (decimal value: " << address << ", port: ";

            if (port == -1) {
                std::cout << "none";
            } else {
                std::cout << port;
            }

            std::cout << ")" << std::endl;
        } else {
            std::cout << "Invalid input: no valid IPv4 address found" << std::endl;
        }
    }

    return 0;
}
