// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_NET_PROCESSING_H
#define BITCOIN_NET_PROCESSING_H

#include "net.h"
#include "validationinterface.h"

class Config;

/** Default for -maxorphantx, maximum number of orphan transactions kept in
 * memory */
static const unsigned int DEFAULT_MAX_ORPHAN_TRANSACTIONS = 100;
/** Expiration time for orphan transactions in seconds */
static const int64_t ORPHAN_TX_EXPIRE_TIME = 20 * 60;
/** Minimum time between orphan transactions expire time checks in seconds */
static const int64_t ORPHAN_TX_EXPIRE_INTERVAL = 5 * 60;
/** Default number of orphan+recently-replaced txn to keep around for block
 * reconstruction */
static const unsigned int DEFAULT_BLOCK_RECONSTRUCTION_EXTRA_TXN = 100;
/** Max broadcast delay duration in milliseconds */
static const int64_t MAX_INV_BROADCAST_DELAY = 50 * 1000;

/** Register with a network node to receive its signals */
void RegisterNodeSignals(CNodeSignals &nodeSignals);
/** Unregister a network node */
void UnregisterNodeSignals(CNodeSignals &nodeSignals);
/** Set inventory broadcasting delay time in seconds*/
bool SetInvBroadcastDelay(const int64_t& nDelayMillisecs);

class PeerLogicValidation final : public CValidationInterface {
private:
    CConnman *connman;

public:
    PeerLogicValidation(CConnman *connmanIn);

    void
    BlockConnected(const std::shared_ptr<const CBlock> &pblock,
                   const CBlockIndex *pindexConnected,
                   const std::vector<CTransactionRef> &vtxConflicted) override;
    void UpdatedBlockTip(const CBlockIndex *pindexNew,
                         const CBlockIndex *pindexFork,
                         bool fInitialDownload) override;
    void BlockChecked(const CBlock &block,
                      const CValidationState &state) override;
    void NewPoWValidBlock(const CBlockIndex *pindex,
                          const std::shared_ptr<const CBlock> &pblock) override;
};

struct CNodeStateStats {
    int nMisbehavior;
    int nSyncHeight;
    int nCommonHeight;
    std::vector<int> vHeightInFlight;
};

/** Get statistics from node state */
bool GetNodeStateStats(NodeId nodeid, CNodeStateStats &stats);
/** Increase a node's misbehavior score. */
void Misbehaving(NodeId nodeid, int howmuch, const std::string &reason);

/** Process protocol messages received from a given node */
bool ProcessMessages(const Config &config, const CNodePtr& pfrom, CConnman &connman,
                     const std::atomic<bool> &interrupt);
/**
 * Send queued protocol messages to be sent to a give node.
 *
 * @param[in]   pto             The node which we are sending messages to.
 * @param[in]   connman         The connection manager for that node.
 * @param[in]   interrupt       Interrupt condition for processing threads
 * @return                      True if there is more work to be done
 */
bool SendMessages(const Config &config, const CNodePtr& pto, CConnman &connman,
                  const std::atomic<bool> &interrupt);

#endif // BITCOIN_NET_PROCESSING_H
