#include "HexByte.h"
#include "math.h"

int HexToByte(char* hex){
    int result = 0;
    for(int i = 0; i < 4; i++){
        if((hex[i] >= 48) && (hex[i] <= 57)){
            result += (hex[i] - 48) * pow(16, (3-i));
        }
        else if((hex[i] >= 65) && (hex[i] <= 70)){
            result += (hex[i] - 65 + 10) * pow(16, (3-i));
        }
        else if((hex[i] >= 97) && (hex[i] <= 102)){
            result += (hex[i] - 97 + 10) * pow(16, (3-i));
        }
    }

    return result;
}

std::string ByteToHex(uint8_t input){
    std::string HexTable[] = {
        "0", "1", "2", "3",
        "4", "5", "6", "7",
        "8", "9", "A", "B", 
        "C", "D", "E", "F"
    };
    
    return HexTable[input >> 4] + HexTable[input & 0x0F];
}