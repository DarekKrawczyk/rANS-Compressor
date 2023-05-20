#include <iostream>
#include <math.h>

/* Links:
    - https://www.youtube.com/watch?v=6vNBLGKsssA&ab_channel=StanfordResearchTalks
    - https://kedartatwawadi.github.io/post--ANS/
*/

//Alphabet -> tablica wszystkich mo¿liwych do zakodowania symboli.
//M -> mianownik prawdopodobieñstwa.
const int M = 8;
int Alphabet[M] = { 0, 1, 2 };
//Prob - probability -> tablica przechowuj¹ca prawdopodobieñstwa wyst¹pienia danego symbolu.
float Prob[M] = { 3 / M, 3 / M, 2 / M };
//Freq - frequencies -> tablica zawieraj¹ca liczniki z prawdopodobieñstwa wyst¹pienia danego symbolu.
int Freq[M] = { 3, 3, 2 };
//Cumul -> tablica przechowuj¹ca sumê czêstotliwoœci poprzednio zdefiniowanych symboli.
int Cumul[M] = { 0, 3, 6 };

//This is a function to be called on every step (element of array) of input value.
//It takes as argument previous value of the encoder, and element of an array to be compressed.
int encodeStep(int x, int symbol) {
    
    int blockID = floor(x / Freq[symbol]);
    int slot = Cumul[symbol] + (x%Freq[symbol]);
    int nextState = blockID * M + slot;
    std::cout << "Next state: " << nextState << std::endl;
    return nextState;
}

//Encoder takes an input array.
int encode(int* inputTable, size_t size) {
    //Initial state of the encoder. Always starts with 0.
    int x = 0;
    for (int i = 0; i < size; i++) {
        x = encodeStep(x, inputTable[i]);
    }

    //One feature missing here is converting result to binary form.

    return x;
}

int main()
{
    std::cout << "rANS encoder input = { 1, 0, 2, 1 }\n";
    const size_t inputSize = 4;
    int input[inputSize] = { 1, 0, 2, 1 };
    int result = encode(input, inputSize);
    std::cout << "Result: " << result << std::endl;
}
