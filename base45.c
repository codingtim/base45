#include <stdio.h>
#include <string.h>
const char* charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";
void main() {
    char* line = NULL;
    ssize_t len = 0;
    getline(&line, &len, stdin);
    
    //line ends with \n, so - 1 on length and ofcourse \0 which is not counted by strlen
    char values[strlen(line) - 1];
    //convert characters in the base45 string to their encoded number value
    for(int i = 0; i < sizeof(values); i++) {
        char* ptr = strchr(charset, line[i]);
        values[i] = ptr - charset;
    }
    //base45 outputs 3 bytes for every 2 bytes of encoded data
    //a trailing data byte is encoded in 2 bytes
    char resultBytes[sizeof(values) / 3 * 2 + (sizeof(values) % 3 != 0 ? 1 : 0)];
    //decode 3 bytes to 2 data bytes
	for(int i = 0; i < sizeof(resultBytes) / 2; i++) {
        unsigned short tripleValue = values[i*3] + values[i*3+1] * 45 + values[i*3+2] *45 *45;
        char firstByte = (tripleValue >> 0);
        char secondByte = (tripleValue >> 8);   
        resultBytes[i*2] = secondByte;
        resultBytes[i*2+1] = firstByte;
    }
    //if present decode the 2 last bytes to single data byte
    if(sizeof(values) % 3 != 0) {
        char value = values[sizeof(values)-2] + values[sizeof(values)-1] * 45;
        resultBytes[sizeof(resultBytes)-1] = value;
    
    }
    fwrite(resultBytes, 1, sizeof(resultBytes), stdout);
}
