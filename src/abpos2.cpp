// Copyright (c) 2019 barrystyle
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <validation.h>
#include <util.h>

int GetStakeInputAge(uint256 stakeInputHash, int nBlockTime)
{
    int nInputTime = 0;

    // if this isnt provided, assume we want the current age
    if (nBlockTime == 0)
        nBlockTime = GetTime();

    // we will fail if this is a unspent coinbasetxn
    if (stakeInputHash == uint256()) return 0;

    if (nInputTime == 0)
    {
        // retrieve tx from disk..
        uint256 blockHashFrom;
        CTransactionRef stakeInputTx;
        if (!GetTransaction(stakeInputHash, stakeInputTx, Params().GetConsensus(), blockHashFrom))
            return 0;

        // ..then the block it was used in..
        CBlock originBlock;
        CBlockIndex* pindex = NULL;
        BlockMap::iterator it2 = mapBlockIndex.find(blockHashFrom);
        if (it2 != mapBlockIndex.end())
            pindex = it2->second;
        else
            return error("GetStakeInputAge() : read block failed");
        if (!ReadBlockFromDisk(originBlock, pindex, Params().GetConsensus()))
            return 0;

        // ..then we're done
        nInputTime = originBlock.nTime;
    }

    // if nInputTime is still 0 here, something went wrong
    if (nInputTime == 0) return 0;

    // return time in whole hours
    int stakeInputAge = nBlockTime - nInputTime;

    LogPrintf("GetStakeInputAge()::Inputhash %s Age %llu\n", stakeInputHash.ToString().c_str(), stakeInputAge);
    return stakeInputAge * 10000;
}

CAmount GetProofOfStakeReward(int64_t nCoinAge)
{
    static int64_t nRewardCoinYear = CENT;
    CAmount nSubsidy = nCoinAge * 33 / (365 * 33 + 8) * nRewardCoinYear;
    LogPrintf("%s: create=%llu nCoinAge=%llu\n", __func__, nSubsidy, nCoinAge);
    return nSubsidy;
}
