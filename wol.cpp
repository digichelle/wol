/* *****************************************************************************

wol.cpp - Simple Wake-On-LAN utility
Author:  Michelle Catherine Marcó (digichelle)
Source:  https://github.com/digichelle/wol

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
#include <string>           // String handling
#include <cstring>          // Provides memset()
#include <regex>            // Provides regular expression support
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
        "    -q, --quiet    Quiet mode\n";
    exit(0);
}

// Function to display version information
void PrintVersion(const string& progname) {
    cout <<
        progname << " - Simple Wake-ON-LAN utility\n" <<
        "Version 1.0β4\n" <<
        "Copyright © 2026 Michelle Catherine Marcó\n" <<
        "MIT License\n";
    exit(0);
}

// Function to validate IP address
bool ValidateIPAddress(const string& ip) {

    // Convert const string to const char * for use in regex_match()
    const char *ipaddr = ip.c_str();

    // Define regular expression
    const auto ipv4 = regex(R"(^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$)");

    // Check if supplied IP address matches regular expression
    if (regex_match(ipaddr, ipv4)) {
        return true;
    }

    return false;
}

// Function to validate port
bool ValidatePort(unsigned port) {

    // Valid port number must be an integer from 0 to 65535
    if (port < 0 || port > 65535) {
        return false;
    }

    return true;
}

// Function to turn MAC address (like "00:11:22:33:44:55") into bytes
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

    // Convert const string to const char * for use in inet_addr() or inet_pton()
    const char *ipaddr = ip.c_str();

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
    addr.sin_addr.s_addr = inet_addr(ipaddr);

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

        // Set defaults
        bool quiet = false;
        string mac = argv[argc - 1];
        string IPAddress = "255.255.255.255";
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
                    port = strtol(optarg, NULL, 0);
                    break;
                case '?': // Unrecognized option
                    cerr << "Unknown option '-" << static_cast<char>(optopt) << "'" << endl;
                default:
                    cerr << "Internal error" << endl;
            }
        }

        // Do the thing!
        if (optind == argc - 1) {

            // Parse the MAC address into bytes and validate
            auto MACBytes = ParseMAC(mac);
            if (MACBytes.size() != 6) {
                cerr << "Error: Invalid MAC address (Note: format must be XX:XX:XX:XX:XX:XX)" << endl;
            }

            // Validate IP address
            if (!ValidateIPAddress(IPAddress)) {
                cerr << "Error: Invalid IP address" << endl;
            }

            // Validate port number
            if (!ValidatePort(port)) {
                cerr << "Error: Invalid port number" << endl;
            }

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
