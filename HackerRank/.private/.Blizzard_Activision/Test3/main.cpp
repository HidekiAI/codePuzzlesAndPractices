#include <bits/stdc++.h>

using namespace std;

class Lock
{
public:
    Lock() = default;
    ~Lock() = default;
    void Acquire();
    void Release();
private:
    std::recursive_mutex m_mutex;
};

void Lock::Acquire()
{
    m_mutex.lock();
}

void Lock::Release()
{
    m_mutex.unlock();
}

const unsigned int GOLD_LIMIT = 1000;


class Avatar
{
    private:
    // hide default ctor() just in case...  We want to make sure m_id is valid...
   Avatar(){
       // invalid ID, probably better to make m_id const, in which
       // only gets initialized in ctor()
       m_id=0;
       m_gold=0;} 
public:
    Avatar(unsigned int id){m_id = id; m_gold=0;};
    ~Avatar(){};
    
    unsigned int GetId() const { return m_id; }
    unsigned int GetGold() const { return m_gold; }

    void AddGold(unsigned int gold)
    {
        m_Lock.Acquire();
        m_gold += gold;
        m_Lock.Release();
    }
    void RemoveGold(unsigned int gold)
    {
        if ((m_gold <= 0) ||  ((m_gold - gold) < 0))
        {
            // ideally, for multi-threads, it's best to just throw rather than to
            // futz around with some ways to fix itself (i.e. make sure the
            // calling system first checks prior to removing)
            // At least with transfer() method, we first test enough $$$,
            // so it must be some other system that is abusing this...
            throw runtime_error("Make sure to verify gold amount first before removing");
        }
        
        m_Lock.Acquire();
        m_gold -= gold;
        m_Lock.Release();
        // once lock has been released, do some testing
    }
   
   // true if transfer succeeded 
    static bool Transfer(Avatar& from, Avatar& to, unsigned int gold)
    {
        // fail if gold_from < gold
        if (from.GetGold() < gold)  // though it's more performant to bypass GetGold() and use from.m_gold, in case I need to track who/what is (mainly race-condition tracking) accessing m_gold at the time of transfer, we'll use this method
        {
            // not enough $$$ in pocket
            return false;
        }
        // fail if gold_to has "too much gold"
        if (to.GetGold() > gold)
        {
            // have too much gold, disallow this potential cheater...
            return false;
        }

        // Notes:
        // m_Lock is per Avatar, so it makes NO SENSE to lock
        // here where to and from (different objects)
        // compete for the resources...  Also, thankfully, the
        // architect made this method static, hence there's no
        // way to access m_lock here anyways...
        // Hence, locking should/will-be done at 
        // Add/Remove gold level...
        // The common issue is that if either-or has some issues
        // such as, even though we've prechecked that Avatar
        // has enough gold (above), by the time we got here,
        // we may have less gold...
        // The most simplest solution is the "Starbuck transaction"
        // system, where you'd go ahead and buy that coffee, even if
        // the coffee may not be possible to be made due to lack of
        // ingredients or something.  We then complete the paying
        // transaction, and then if some issue occurs, we'd refund
        // the money as next/separate transaction...

        // also, note that we should be able to look at thread-id but we'll deal with that after
        // all my thought prcoess is complete/clear... (really hate these time-based tests
        // where I cannot sit-down and design first and have to just start coding on the fly
        // in hopes that the approach sticks to the wall...)
        // first, pay for the coffee: 
        try {
            from.RemoveGold(gold);  // blocks until resource is ready...  throws if wasn't able to remove
        } catch (exception) {
            // Assume all throws are done prior to be withdrawn, hence no need to
            // provide refund 
            return false;   // just mark as failed transaction...
        }
        
        try {
        to.AddGold(gold);
            
        }
        catch (exception) {
            // in this case, we've failed to add to the dest,
            // so refund is needed
            from.AddGold(gold); // refund
            return false;   // fail the transaction
        }
        return true;
    }

private:
    unsigned int m_id;
    unsigned int m_gold;
    
    Lock m_Lock;    // reentrent MUTEX so as long as it's locked again from same thread id
};
enum TransferDirection
{
	TO_PLAYER2,
	FROM_PLAYER2,
};

struct Transfer
{
	unsigned int gold1;
	unsigned int gold2;
	unsigned int transferAmount;
	TransferDirection transferDirection;
};

void doTransfer(
	Avatar& from,
	Avatar& to,
	unsigned int gold1,
	unsigned int gold2,
	unsigned int transferAmount)
{
	from.AddGold(gold1);
	to.AddGold(gold2);
	Avatar::Transfer(from, to, transferAmount);
}

void transferThread(
	Avatar& player1,
	Avatar& player2,
	const std::vector<Transfer>& transfers)
{
	for (const Transfer& transfer : transfers)
	{
		if (transfer.transferDirection == TO_PLAYER2)
		{
			doTransfer(player1, player2, transfer.gold1, transfer.gold2, transfer.transferAmount);
		}
		else
		{
			doTransfer(player2, player1, transfer.gold1, transfer.gold2, transfer.transferAmount);
		}
	}
}

int main() {
    ofstream fout(getenv("OUTPUT_PATH"));

    int res;
    int v_size = 0;
    cin >> v_size;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Avatar player1(101), player2(202);
    
    std::vector<Transfer> transfers1, transfers2;
	std::vector<std::thread> threads;

	for (int i = 0; i < v_size; i++) {
		unsigned int gold1, gold2, transferAmount, transferDirection;
		cin >> gold1 >> gold2 >> transferAmount >> transferDirection;

		if (i % 2 == 0)
		{
			transfers1.push_back({ gold1, gold2, transferAmount, static_cast<TransferDirection>(transferDirection) });
		}
		else
		{
			transfers2.push_back({ gold1, gold2, transferAmount, static_cast<TransferDirection>(transferDirection) });
		}
	}

	threads.emplace_back(transferThread, std::ref(player1), std::ref(player2), std::ref(transfers1));
	threads.emplace_back(transferThread, std::ref(player1), std::ref(player2), std::ref(transfers2));

    for (std::thread& thread : threads)
    {
        try 
        {
            thread.join();
        } 
        catch(const std::system_error& e) 
        {
            fout << "Caught system_error with code " << e.code() 
                  << " meaning " << e.what() << endl;
            return 1;
        }
    }
    
    fout << "Player 1 gold: " << player1.GetGold() << ". Player 2 gold: " << player2.GetGold();
    
    fout.close();
    return 0;
}