/* *****************************************************************************

wol.cpp - Simple Wake-On-LAN utility
Author: Michelle Catherine Marcó (digichelle)
Source: https://github.com/digichelle/wol

MIT LICENSE
Copyright © 2026 Michelle Catherine Marcó

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

***************************************************************************** */

#include <iostream>         // Basic I/O
#include <algorithm>        // Provides all_of()
#include <cstring>          // Provides memset()
#include <regex>            // Provides regular expression support
#include <string>           // String handling
#include <vector>           // Byte storage
#include <getopt.h>         // Provides getopt_long()
#include <arpa/inet.h>      // Provides inet_addr()
#include <netinet/in.h>     // Internet address structures
#include <sys/socket.h>     // Socket programming (BSD/Linux/macOS)
#include <unistd.h>         // Provides close()

using namespace std;

// Function to display help
void PrintUsage(const string& progname) {
    cout <<
        progname << " - Simple Wake-On-LAN utility\n" <<
        "Usage: " << progname << " [OPTIONS] ... MAC-ADDRESS\n" <<
        "    -h, --help     Display this help and exit\n" <<
        "    -v, --version  Display version and license information\n" <<
        "    -i, --ip       Specify broadcast IP address\n" <<
        "    -p, --port     Specify port number\n" <<
        "    -q, --quiet    Quiet mode\n" <<
        "\n" <<
        "MAC address must be in the form x:x:x:x:x:x, where x is a zero-padded\n" <<
        "hexadecimal number from 00 to ff.\n" <<
        "\n" <<
        "IP address, if specified, must be in the form x.x.x.x, where x is a\n" <<
        "decimal number from 0 to 255.\n\n";
    exit(0);
}

// Function to display version information
void PrintVersion(const string& progname) {
    cout <<
        progname << " - Simple Wake-ON-LAN utility\n" <<
        "Version 1.0β6\n" <<
        "Copyright © 2026 Michelle Catherine Marcó\n" <<
        "MIT License: https://opensource.org/license/MIT\n\n";
    exit(0);
}

// Function to validate MAC address
bool ValidateMACAddress(const string& mac) {

    // Make sure MAC address is not greater than 17 bytes
    if (mac.size() > 17) {
        return false;
    }

    // Define regular expression
    const auto macaddr = regex(R"(([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2})");

    // Check if supplied MAC address matches regular expression
    if (regex_match(mac.c_str(), macaddr)) {
        return true;
    }

    return false;
}

// Function to validate IP address
bool ValidateIPAddress(const string& ip) {

    // Make sure IP address is not greater than 15 bytes
    if (ip.size() > 15) {
        return false;
    }

    // Define regular expression
    const auto ipv4 = regex(R"(^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$)");

    // Check if supplied IP address matches regular expression
    if (regex_match(ip.c_str(), ipv4)) {
        return true;
    }

    return false;
}

// Function to validate port
bool ValidatePort(const string& portstr) {

    // Make sure port is not greater than 5 bytes
    if (portstr.size() > 5) {
        return false;
    }

    // Valid port number must be all digits
    if (!all_of(portstr.begin(), portstr.end(), ::isdigit)) {
        return false;
    }

    // Convert string to integer
    unsigned port = strtol(portstr.c_str(), NULL, 0);

    // Valid port number must be an integer from 1 to 65535
    if (port < 1 || port > 65535) {
        return false;
    }

    return true;
}

// Function to turn MAC address (like "aa:bb:cc:dd:ee:ff") into bytes
vector<unsigned char> ParseMAC(const string& mac) {
    vector<unsigned char> bytes;
    size_t start = 0;
    size_t end = mac.find(':');
    int count = 0;

    // Split the string by ':' and convert each part to a byte
    while (end != string::npos && count < 5) {
        string byteStr = mac.substr(start, end - start);
        try {
            unsigned char byte = static_cast<unsigned char>(stoi(byteStr, nullptr, 16));
            bytes.push_back(byte);
        } catch (...) {
            return {}; // Return empty if conversion fails
        }
        start = end + 1;
        end = mac.find(':', start);
        count++;
    }

    // Check if we have exactly 5 colons
    if (count != 5) {
        return {};
    }

    // Get the last part
    string byteStr = mac.substr(start);
    try {
        unsigned char byte = static_cast<unsigned char>(stoi(byteStr, nullptr, 16));
        bytes.push_back(byte);
    } catch (...) {
        return {};
    }

    // MAC address must be 6 bytes
    if (bytes.size() != 6) {
        return {};
    }

    return bytes;
}

// Function to build the magic packet
vector<unsigned char> CreateMagicPacket(const vector<unsigned char>& mac) {
    vector<unsigned char> packet;

    // Step 1: Add 6 bytes of FF (255 in hex)
    for (int i = 0; i < 6; i++) {
        packet.push_back(0xFF);
    }

    // Step 2: Add the MAC address 16 times
    for (int i = 0; i < 16; i++) {
        for (auto byte : mac) {
            packet.push_back(byte);
        }
    }

    return packet;
}

// Function to send the magic packet over the network
bool SendPacket(const vector<unsigned char>& packet, const string& ip, unsigned port) {

    // Create a UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        cerr << "Error: Could not create socket" << endl;
        return false;
    }

    // Allow the socket to send broadcast messages
    int broadcast = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
        cerr << "Error: Could not set broadcast option" << endl;
        close(sock);
        return false;
    }

    // Set up the destination
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    // Send the packet
    ssize_t sent = sendto(sock, packet.data(), packet.size(), 0, (struct sockaddr*)&addr, sizeof(addr));
    if (sent < 0) {
        cerr << "Error: Failed to send packet" << endl;
        close(sock);
        return false;
    }

    // Clean up
    close(sock);
    return true;

}

int main(int argc, char * const argv[]) {
    try {

        // Initialize variables with defaults
        bool quiet = false;
        string mac = argv[argc - 1];
        string IPAddress = "255.255.255.255";
        string portstr = "9";
        unsigned port = 9;

        // Parse command line arguments using getopt_long
        // (this allows both short and long forms)
        const char* const short_opts = "hvqi:p:";
        const option long_opts[] {
            { "help", no_argument, NULL, 'h' },
            { "version", no_argument, NULL, 'v' },
            { "quiet", no_argument, NULL, 'q' },
            { "ip", required_argument, NULL, 'i' },
            { "port", required_argument, NULL, 'p' },
            { NULL, no_argument, NULL, 0 }
        };

        while (true) {
            const auto opt = getopt_long(argc, argv, short_opts, long_opts, NULL);
            if (-1 == opt)
                break;
            switch (opt) {
                case 'h': // -h, --help
                    PrintUsage(argv[0]);
                    return 1;
                case 'v': // -v, --version
                    PrintVersion(argv[0]);
                    return 1;
                case 'q': // -q, --quiet
                    quiet = true;
                    break;
                case 'i': // -i, --ip
                    IPAddress = optarg;
                    break;
                case 'p': // -p, --port
                    portstr = optarg;
                    port = strtol(optarg, NULL, 0);
                    break;
                case '?': // Unrecognized option
                    cerr << "Unknown option '-" << static_cast<char>(optopt) << "'" << endl;
                    exit(1);
                default:
                    cerr << "Internal error" << endl;
                    exit(1);
            }
        }

        // Do the thing!
        if (optind == argc - 1) {

            // Initalize error handling variables
            bool ValidMAC = true;
            bool ValidIPAddress = true;
            bool ValidPort = true;
            int errors = 0;

            // Validate MAC address and convert to bytes if valid
            if (!ValidateMACAddress(mac)) {
                ValidMAC = false;
                errors++;
            }

            // Validate IP address
            if (!ValidateIPAddress(IPAddress)) {
                ValidIPAddress = false;
                errors++;
            }

            // Validate port number
            if (!ValidatePort(portstr)) {
                ValidPort = false;
                errors++;
            }

            // Display errors if errors > 0
            if (errors > 0) {
                if (errors == 1) {
                    cerr << "1 error:\n";
                } else {
                    cerr << errors << " errors:\n";
                }
                if (!ValidMAC) {
                    cerr << "    Invalid MAC address\n";
                }
                if (!ValidIPAddress) {
                    cerr << "    Invalid IP address\n";
                }
                if (!ValidPort) {
                    cerr << "    Invalid port\n";
                }
                exit(1);
            }

            // Convert MAC address to bytes
            auto MACBytes = ParseMAC(mac);

            // Create the magic packet
            auto packet = CreateMagicPacket(MACBytes);

            // Send the packet and check for errors
            if (!SendPacket(packet, IPAddress, port)) {
                return 1; // Exit with error if sending fails
            }

            if (!quiet) {
                cout <<
                    "Wake-on-LAN magic packet sent!\n" <<
                    "    MAC Address: " << mac << "\n" <<
                    "    IP Address:  " << IPAddress << "\n" <<
                    "    Port:        " << dec << port << "\n";
            }

        } else {
            PrintUsage(argv[0]);
            return 1;
        }
    }

    catch (const exception& e) {
        cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}
