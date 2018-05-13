#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include <unistd.h>

#include "hash.h"

static const size_t TRANSACTIONS_PRE_BLOCK = 5;
static const unsigned INIT_BALACE = 100;

struct Transaction
{
	Hash256 hash;
	std::string from;
	std::string to;
	double value;
};

struct Block
{
	bool valid;
	Hash256 block_hash;
	Hash256 parent_hash;
	size_t block_height;
    std::vector<Transaction> transactions;
	Block(size_t height);
};

class AccountManager
{
public:
	static AccountManager* Instance()
	{
		static AccountManager instance;
		return &instance;
	}
	bool VerifyAndProcessTransaction(const Transaction& t);
private:
	AccountManager();
	unsigned& CheckBalance(const std::string& account);

	std::unordered_map<std::string, unsigned> _balance;
};

class BlockChain
{
public:
	static BlockChain* Instance()
	{
		static BlockChain instance;
		return &instance;
	}

	bool AppendTransaction(const Transaction& t);
	void PrintChain() const;

private:
	BlockChain();
	std::list<Block> _chain;
};
