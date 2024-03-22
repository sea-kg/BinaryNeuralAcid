
function calcBinaryNeuralAcid(vIn) {
    var node0 = vIn[0];
    var node1 = vIn[1];
    var node2 = (node1 & node0) % 256;
    var node3 = (node1 | node0) % 256;
    var node4 = (node3 ^ node2) % 256;
    var node5 = ((node2 >> (node3 % 8)) | (node2 << (8 - (node3 % 8)))) % 256;
    var node6 = (node4 | node5) % 256;
    var vOut = [];
    vOut.push(node6); // out 0
    return vOut;
};

var success = 0;
var all = 0;
for (var x = 0; x < 256; x++) {
    for (var y = 0; y < 256; y++) {
        var expected = (x + y) % 256;
        var got = calcBinaryNeuralAcid([x,y])[0];
        if (expected == got) {
            success++;
        }
        all++;
    }
}
console.log(success, "/", all);

/*
var x = 222;
var y = 211;
var z = (x + y) % 256;
console.log("+ z= ", z);
var z = (x | y);
console.log("| z= ", z);
var z = (x ^ y);
console.log("^ z= ", z);
var z = ((x << (y % 8)) | (x >> (8 - (y % 8)))) % 256;
console.log("shl z= ", z);
var z = ((x >> (y % 8)) | (x << (8 - (y % 8)))) % 256;
console.log("shr z= ", z);
*/

/*
c++ :
#include <iostream>

int main()
{
    std::cout<<"Hello World\n";
    unsigned char x = 222;
    unsigned char y = 211;
    unsigned char z = (x + y);
    printf("+z = %d\n", int(z));
    z = (x | y);
    printf("| z = %d\n", int(z));
    z = (x ^ y);
    printf("^ z = %d\n", int(z));
    z = (x & y);
    printf("& z = %d\n", int(z));
    z = (x << (y % 8)) | (x >> (8 - (y % 8)));
    printf("sdl z = %d\n", int(z));
    z = (x >> (y % 8)) | (x << (8 - (y % 8)));
    printf("sdr z = %d\n", int(z));
    return 0;
}

in c++ expected:
+z = 177
| z = 223
^ z = 13
& z = 210
sdl z = 246
sdr z = 219
*/