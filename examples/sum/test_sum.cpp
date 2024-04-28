#include <iostream>
#include <vector>

std::vector<unsigned char> calcBinaryNeuralAcid(const std::vector<unsigned char> &vIn) {
    // 16029 / 65536
    unsigned char node0 = vIn[0];
    unsigned char node1 = vIn[1];
    unsigned char node2 = node0 ^ node1;
    unsigned char node3 = node2 | node1;
    unsigned char node4 = node0 ^ node1;
    unsigned char node5 = (node3 << int(node3 % 8)) | (node3 >> (8 - int(node3 % 8)));
    unsigned char node6 = node4 ^ node3;
    unsigned char node7 = node6 & node3;
    unsigned char node8 = node5 | node3;
    unsigned char node9 = (node7 >> int(node8 % 8)) | (node7 << (8 - int(node8 % 8)));
    unsigned char node10 = node9 | node4;
    std::vector<unsigned char> vOut;
    vOut.push_back(node10); // out 0
    return vOut;
};

int main()
{
    int success = 0;
    int all = 0;
    for (int  x = 0; x < 256; x++) {
        for (int  y = 0; y < 256; y++) {
            unsigned char cX = x;
            unsigned char cY = y;
            unsigned char expected = x + y;
            if (calcBinaryNeuralAcid({cX, cY})[0] == expected) {
                success++;
            }
            all++;
        }
    }
    printf("%d / %d\n", success, all);
    return 0;
}