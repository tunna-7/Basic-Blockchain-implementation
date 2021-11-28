#include <ctime>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
#include <string>

// Transaction Data
struct TransactionData
{
    double amount;
    string senderKey;
    string receiverKey;
    time_t timestamp;
};

// Block Class
class Block
{
    private:
        int index;
        size_t blockHash;
        size_t previousHash;
        size_t generateHash();

    public:
        //Constructor
        Block(int idx, TransactionData d, size_t prevHash);

        // Get Original Hash
        size_t getHash();

        // Get Previous Hash
        size_t getPreviousHash();

        // Transaction Data
        TransactionData data;

        // Validate Hash
        bool isHashValid();

};

// Constructor with params
Block::Block(int idx, TransactionData d, size_t prevHash)
{
    index = idx;
    data = d;
    previousHash = prevHash;
    blockHash = generateHash();

}

// private functions
size_t Block::generateHash()
{
    hash<string> hash1;
    hash<size_t> hash2;
    hash<size_t> finalHash;
    string toHash = to_string(data.amount) + data.receiverKey + data.senderKey + to_string(data.timestamp);

    return finalHash(hash1(toHash) + hash2(previousHash));
}

// public functions
size_t Block::getHash()
{
    return blockHash;
}

size_t Block::getPreviousHash()
{
    return previousHash;
}

bool Block::isHashValid()
{
    return generateHash() == blockHash;
    // If someone had modified or entered some new data or
    // try to change it forcefully then this will tell us
    // by comparing the generated Hash value and block Hash 
    // from the get Hash Function which means the string 
    // toHash will not be the same anymore
}

//BlockCahin Class
class Blockchain
{
    private:
        Block createGenesisBlock();

    public:
        // Public Chain
        vector<Block> chain;

        // Constructor
        Blockchain();

        // Public Functions
        void addBlock(TransactionData data);
        bool isChainValid();

        // Sample Example
        Block *getLatestBlock();
};

// BlockChain COnstructor - For implementing blockchain class memebers
Blockchain::Blockchain()
{
    // first block in a blockchain is called Genesis Block
    Block genesis = createGenesisBlock();
    chain.push_back(genesis);

}

Block Blockchain::createGenesisBlock()
{
    // Now we'll create what the Block parameters take in;
    // Since its a creation of genesis block i.e the first block
    // it doesn't really matter what data I'm giving 
    time_t current;
    TransactionData d;
    d.amount = 0;
    d.receiverKey = "None";
    d.senderKey = "None";
    d.timestamp = time(&current);

    hash<int> hash1;
    Block genesis(0,d,hash1(0));
    return genesis;
}

//  Sample example function which uses pointer - hypothetical
//  Following is used so that memory address can directly be 
//  changed or implemented
Block *Blockchain::getLatestBlock()
{
    return &chain.back();
}

void Blockchain::addBlock(TransactionData d)
{
    int index = (int)chain.size() - 1;
    Block newBlock(index, d , getLatestBlock()->getHash());
}

bool Blockchain::isChainValid()
{
    //iterator will iterate over the vector if type Block
    vector<Block>::iterator it;
    int chainLen = (int)chain.size();

    //Simple for loop which will begin with the first value of chain
    //as in the blockchain if the chain is the longest then its consi-
    //dered correct and valid version of Blockchain
    for(it = chain.begin(); it !=chain.end(); ++it )
    {
        Block currentBlock = *it;
        if(!currentBlock.isHashValid())
        {
            return false;
            //if chain is not valid then exit
        }

        if(chainLen > 1)
        {
            //checking previous block in the current block
            //as every block contains its prev chain (fundamental of
            //Blockcahin)
            Block previousBlock = *(it - 1);

            if(currentBlock.getPreviousHash() != previousBlock.getHash())
            {
               return false; 
            }
        }
    }
    return true;
}

int main()
{
    // Start Blockchain
    Blockchain Ayushereum;

    // Data for first added block
    TransactionData data1;
    time_t data1Time;
    data1.amount = 2; //Sending 2 Ayushereum
    data1.receiverKey = "Messi";
    data1.senderKey = "Ronaldo";
    data1.timestamp = time(&data1Time);

    Ayushereum.addBlock(data1);

    cout<<"Is chain valid?" << endl
        << Ayushereum.isChainValid() << endl;

    TransactionData data2;
    time_t data2Time;
    data2.amount = 0.0768; //Sending that amount of Ayushereum
    data2.receiverKey = "Bruno";
    data2.senderKey = "Rashy";
    data2.timestamp = time(&data2Time);

    Ayushereum.addBlock(data2);

    cout<<"Is the current chain valid?" << endl
        << Ayushereum.isChainValid() << endl;

    //Some hacker wannabe wants to disrupt the chain
    Block *hackBlock = Ayushereum.getLatestBlock();
    hackBlock->data.amount = 100; //hacking 100 Ayushereums
    hackBlock->data.receiverKey = "Suarez";

    cout<<"Is the current chain still valid?"<< endl
        <<Ayushereum.isChainValid() << endl;

    //refer isHashValid() fuction as to why it'll give 0 as a output
}