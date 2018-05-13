#include "blockchain.h"
#include <fstream>
#include <iostream>

Block::Block(size_t height)
	: valid(false)
	, block_height(height)
{
}

AccountManager::AccountManager()
{
}

unsigned& AccountManager::CheckBalance(const std::string& account)
{
	auto found = _balance.find(account);
	if (found == _balance.end())
	{
		found = _balance.emplace(account, INIT_BALACE).first;
	}
	return found->second;
}

bool AccountManager::VerifyAndProcessTransaction(const Transaction& t)
{
	unsigned& from_balance = CheckBalance(t.from);
	unsigned& to_balance = CheckBalance(t.to);

	if (from_balance < t.value) // insufficient balance
	{
		return false;
	}
	from_balance -= t.value;
	to_balance += t.value;
	return true;
}

BlockChain::BlockChain()
{
	_chain.push_back(Block(0));
}

bool BlockChain::AppendTransaction(const Transaction& t)
{
	_chain.back().transactions.push_back(t);
	if (_chain.back().transactions.size() >= TRANSACTIONS_PRE_BLOCK)
	{
		std::cout << "create block" << std::endl;
		std::vector<uint8_t> hashes((TRANSACTIONS_PRE_BLOCK + 1) * Hash256::size);
		Hash256& previous_hash = _chain.back().parent_hash;
		std::copy(previous_hash.bytes, previous_hash.bytes + Hash256::size, std::back_inserter(hashes));
		for (auto &t : _chain.back().transactions)
		{
			std::copy(t.hash.bytes, t.hash.bytes + Hash256::size, std::back_inserter(hashes));
		}
		Hash256 hash;
		hash.HashFromByteStreams(hashes);
		_chain.back().block_hash = hash;
		_chain.back().valid = true;
		_chain.push_back(Block(_chain.size()));
		_chain.back().parent_hash = hash;
		return true;
	}
	return false;
}

void BlockChain::PrintChain() const
{
	bool first_block = true;
	for (auto& block : _chain)
	{
		if (block.valid)
		{
			std::cout << "--------------------- Block #" << block.block_height << " -------------------------" << std::endl;
			std::cout << "Block Hash:  " << block.block_hash.ToString() << std::endl;
			std::cout << "Parent Hash: " << (first_block ? "00000000" : block.parent_hash.ToString()) << std::endl;
			for (auto& transaction : block.transactions)
			{
				std::cout << "Transaction Hash: " << transaction.hash.ToString() << " ";
				std::cout << transaction.from << "=>" << transaction.to << ":" << transaction.value << std::endl;
			}
		}
		first_block = false;
	}
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " [transaction file]" << std::endl;
		exit(1);
	}

	std::ifstream myfile(argv[1], std::ifstream::in);
	while (true)
	{
		Transaction t;
		myfile >> t.from;	
		myfile >> t.to;
		myfile >> t.value;
		if (!myfile.good() || myfile.eof())
		{
			break;
		}
		std::cout << "Read transaction: " << t.from << "->" << t.to << ":" << t.value;
		if (AccountManager::Instance()->VerifyAndProcessTransaction(t))
		{
			std::cout << " accepted" << std::endl;
			t.hash.HashFromString(t.from + t.to + std::to_string(t.value));
			bool is_new_block = BlockChain::Instance()->AppendTransaction(t);
			if (is_new_block)
			{
				sleep(10);
			}
		}
		else
		{
			std::cout << " rejected" << std::endl;
		}
	}
	BlockChain::Instance()->PrintChain();
	
	myfile.close();
	return 0;
}
