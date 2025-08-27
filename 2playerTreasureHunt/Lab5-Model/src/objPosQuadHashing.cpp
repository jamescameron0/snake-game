#include "objPosQuadHashing.h"
#include "MacUILib.h"

#include <iostream>
using namespace std;

objPosQuadHashing::objPosQuadHashing()
{
    // Instantiate the objPos Hash Table of the default size TABLE_SIZE defined in the objPosHashTable abstract class
    myHashTable = new objPos[TABLE_SIZE];
    // Should also initialize tableSize field with the default size TABLE_SIZE
    tableSize = TABLE_SIZE;
}

objPosQuadHashing::objPosQuadHashing(int size)
{
    // Instantiate the objPos Hash Table of the specified size
    myHashTable = new objPos[size];
    // Should also initialize tableSize field with specified size
    tableSize = size;
}

objPosQuadHashing::~objPosQuadHashing()
{
    // delete the Hash Table
    delete[] myHashTable;
}

int objPosQuadHashing::calculateHashing(int prefix, int number) const  // hashing function
{    
    // Implementing the primary hashing function

    // Add all the decimal integer digits of the Prefix ASCII char and the Number together

    // For example, given objPos.prefix = 'A' (65) and objPos.number = 98, then...
    int prefixVal = prefix;

    int sum = 0;

    while(prefixVal > 0){
        sum += prefixVal % 10;
        prefixVal = prefixVal/10;
    }
    int num = number;
    while(num > 0){
        sum += num % 10;
        num = num/10;
    }
    return sum;
    //  h(x) = sum of all decimal digits of prefix and number = 6 + 5 + 9 + 8 = 28

    // return the sum as the hashing key.    
    
}

bool objPosQuadHashing::insert(const objPos &thisPos)
{
    // Algorithm similar to the one discussed in class
    //  1. Calculate the initial hashing key using calculateHashing() private helper function (i.e. the h1(x))
    int hashKey = calculateHashing(thisPos.getPF(), thisPos.getNum());
    //  2. Check symbol field to see whether the indexed slot is empty
    //      If yes, insert data, set symbol to 'v', then return true (indicating successful insertion)
    if (myHashTable[hashKey].getSym() == '\0') 
    {
        // If yes, insert data at the hash key and return true (indicating successful insertion)
        myHashTable[hashKey] = thisPos;
        myHashTable[hashKey].setSym('v');
        return true;
    }
    //      Otherwise, proceed to collision resolution
    //  3. Collision resolution strategy - Quadratic Hashing
    //      h(x) = (h1(x) + i^2) mod tableSize  (i is the probing count)
    //  4. For every newly calculated key using quadratic probing, check for empty slot for insertion
    int i = 1;
    while (i <= MAX_PROBING_COUNT) 
    {
        int newHashKey = (hashKey + i * i) % tableSize;
        
        if (myHashTable[newHashKey].getSym() == '\0') 
        {
            myHashTable[newHashKey] = thisPos;
            myHashTable[newHashKey].setSym('v');
            return true;
        }
        else if (myHashTable[newHashKey].getNum() == thisPos.getNum() && myHashTable[newHashKey].getPF() == thisPos.getPF()) 
        {
            return false;
        }
        
        i++;
    }
    //      If empty slot is found, insert data, set symbol to 'v', then return true (indicating successful insertion)
    //      If element is already inserted, return false (indicating failed insertion)
    //      If probing count exceeds MAX_PROBING_COUNT defined in objPosHash.h, return false
    return false;
    //          (too many probing attempts, may lead to integer overflow)


}

bool objPosQuadHashing::remove(const objPos &thisPos)  // lazy delete 
{
    // Algorithm similar to the one discussed in class
    //  1. Calculate the initial hashing key using calculateHashing() private helper function (i.e. the h1(x))
    int hashKey = calculateHashing(thisPos.getPF(), thisPos.getNum());
    
    //  2. Check if the data at the slot matches thisPos (prefix and number)
    //      If yes, perform lazy delete by setting symbol to 0, then return true (indicating successful removal)
    if (myHashTable[hashKey].getPF() == thisPos.getPF() 
    && myHashTable[hashKey].getNum() == thisPos.getNum()) 
    {
        myHashTable[hashKey].setSym('\0');
        return true;
        
    }
    //      Otherwise, proceed to collision resolution
    //  3. Collision resolution strategy - Quadratic Hashing
    int i = 1;
    while (i <= MAX_PROBING_COUNT) 
    {
        int newHashKey = (hashKey + i * i) % tableSize;

        if (myHashTable[newHashKey].getPF() == thisPos.getPF() 
        && myHashTable[newHashKey].getNum() == thisPos.getNum()) 
        {
            myHashTable[newHashKey].setSym('\0');
            return true;
        }
    //      h(x) = (h1(x) + i^2) mod tableSize  (i is the probing count)
    //  4. For every newly calculated key using quadratic probing, check for data matching (identical prefix and number)
    //      If match found, perform lazy delete by setting symbol to 0, then return true (indicating successful removal)
    //      If empty slot encountered, the probing chain reaches the end, hence return false (indicating failed remove
        if(newHashKey == hashKey){
            break;
        }
        i++;
    }
    //      If probing count exceeds MAX_PROBING_COUNT defined in objPosHash.h, return false
    //          (too many probing attempts, may lead to integer overflow)
    return false;

}

bool objPosQuadHashing::isInTable(const objPos &thisPos) const
{
    // Algorithm similar to the one discussed in class
    //  1. Calculate the initial hashing key using calculateHashing() private helper function (i.e. the h1(x))
    int hashKey = calculateHashing(thisPos.getPF(), thisPos.getNum());
    //  2. Check if the data at the slot matches thisPos (prefix and number)
    if (myHashTable[hashKey].getPF() == thisPos.getPF() && myHashTable[hashKey].getNum() == thisPos.getNum()) 
    {
    //      If matched, further chech whether symbol != 0 (data not deleted yet)
        if(myHashTable[hashKey].getSym() != '\0')
        {
            return true;
        }
    
    //          If yes, return true (indicating successfully found)
    //          Otherwise, return false (indicating failed to find)
        else
        {
            return false;
        }
    }

    int i = 1;

    while (i <= MAX_PROBING_COUNT) 
    {
        int newHashKey = (hashKey + i * i) % tableSize;
        
        if (myHashTable[newHashKey].getPF() == thisPos.getPF() 
        && myHashTable[newHashKey].getNum() == thisPos.getNum()) 
        {
            if (myHashTable[newHashKey].getSym() != '\0') 
            {
                return true; 
            }
            else 
            {
                return false; 
            }
        }
        
        i++;
    }
    
    // Data not found after probing
    return false;

    //     If no match is found, proceed to collision resolution
    //  3. Collision resolution strategy - Quadratic Hashing
    //      h(x) = (h1(x) + i^2) mod tableSize  (i is the probing count)
    //  4. For every newly calculated key using quadratic probing, check for data matching (identical prefix and number)
     //      If matched, further chech whether symbol != 0 (data not deleted yet)
    //          If yes, return true (indicating successfully found)
    //          Otherwise, return false (indicating failed to find)
    //      If probing count exceeds MAX_PROBING_COUNT defined in objPosHash.h, return false
    //          (too many probing attempts, may lead to integer overflow)
    
    
}

double objPosQuadHashing::getLambda() const
{
    // Calculate the Load Factor of the Hash Table
    // Check lecture code for examples

    int numElements = 0;

    for (int i = 0; i < tableSize; i++) 
    {
        if (myHashTable[i].getSym() == 'v') // element present
        { 
            numElements++;
        }
    }

    return (double) numElements/tableSize;
}

void objPosQuadHashing::printMe() const
{
    // Recommended Format:
    //  [Key] Prefix Number  Symbol
    //for(int i =0; i < tableSize; i++){
    //   cout << "[" << i << "]" << myHashTable[i].getPF() << myHashTable[i].getNum() << myHashTable[i].getSym() << endl;
    //}
    // e.g. ...
    //      [20] A 9  v
    //      [21] 0 
    //      [22] E 7  v
    //      ... 
 


    // Provided Code for Model Code Integration
    ////////////////////////////////////////////////

    MacUILib_printf("[ ");
    for(int i = 0; i < TABLE_SIZE; i++)
    {
        if(myHashTable[i].getSym() != 0)
             MacUILib_printf("%c%d ", myHashTable[i].getPF(), myHashTable[i].getNum());     
    }
    MacUILib_printf("] L=%.2f", getLambda());

    //You will also need to #include "MacUILib.h" when integrating with the model code.
}