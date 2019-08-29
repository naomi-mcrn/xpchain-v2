// Copyright (c) 2019 barrystyle
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <validation.h>

int GetStakeInputAge(uint256 stakeInputHash, int nBlockTime);
CAmount GetProofOfStakeReward(int64_t nCoinAge);
