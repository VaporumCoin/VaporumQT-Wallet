#pragma once
/******************************************************************************
 * Copyright © 2021 Vaporum Core Developers                                    *
 *                                                                            *
 * See the AUTHORS, DEVELOPER-AGREEMENT and LICENSE files at                  *
 * the top-level directory of this distribution for the individual copyright  *
 * holder information and the developer policies on copyright and licensing.  *
 *                                                                            *
 * Unless otherwise agreed in a custom licensing agreement, no part of the    *
 * SuperNET software, including this file may be copied, modified, propagated *
 * or distributed except according to the terms contained in the LICENSE file *
 *                                                                            *
 * Removal or modification of this copyright notice is prohibited.            *
 *                                                                            *
 ******************************************************************************/
/****
 * This file provides extern access to variables in vaporum_globals.h
 * Please think twice before adding to this list. Can it be done with a better scope?
 */
#include "vaporum_defs.h"
#include <mutex>
#include <cstdint>

extern bool IS_VAPORUM_NOTARY;
extern bool IS_VAPORUM_DEALERNODE;
extern char KMDUSERPASS[8192+512+1];
extern char BTCUSERPASS[8192]; 
extern char ASSETCHAINS_USERPASS[4096];
extern uint8_t NOTARY_PUBKEY33[33];
extern uint8_t ASSETCHAINS_OVERRIDE_PUBKEY33[33];
extern uint8_t ASSETCHAINS_OVERRIDE_PUBKEYHASH[20];
extern uint8_t ASSETCHAINS_PUBLIC;
extern uint8_t ASSETCHAINS_PRIVATE;
extern uint8_t ASSETCHAINS_TXPOW;
extern uint16_t KMD_PORT;
extern uint16_t BITCOIND_RPCPORT;
extern uint16_t DEST_PORT;
extern uint16_t ASSETCHAINS_P2PPORT;
extern uint16_t ASSETCHAINS_RPCPORT;
extern uint16_t ASSETCHAINS_BEAMPORT;
extern uint16_t ASSETCHAINS_CODAPORT;
extern int32_t VAPORUM_INSYNC;
extern int32_t VAPORUM_LASTMINED;
extern int32_t prevVAPORUM_LASTMINED;
extern int32_t VAPORUM_CCACTIVATE;
extern int32_t JUMBLR_PAUSE;
extern int32_t VAPORUM_MININGTHREADS;
extern int32_t STAKED_NOTARY_ID;
extern int32_t USE_EXTERNAL_PUBKEY;
//extern int32_t ASSETCHAINS_SEED;
extern int32_t VAPORUM_ON_DEMAND;
extern int32_t VAPORUM_EXTERNAL_NOTARIES;
extern int32_t VAPORUM_REWIND;
extern int32_t STAKED_ERA;
extern int32_t VAPORUM_CONNECTING;
extern int32_t VAPORUM_EXTRASATOSHI;
extern int32_t ASSETCHAINS_FOUNDERS;
extern int32_t ASSETCHAINS_CBMATURITY;
extern int32_t VAPORUM_NSPV;
extern bool VAPORUM_LOADINGBLOCKS;
extern uint32_t ASSETCHAINS_CC;
extern uint32_t VAPORUM_STOPAT;
extern uint32_t VAPORUM_DPOWCONFS;
extern uint32_t STAKING_MIN_DIFF;
extern uint32_t ASSETCHAIN_INIT;
extern uint32_t ASSETCHAINS_NUMALGOS;
extern uint32_t ASSETCHAINS_MINDIFF[];
extern uint64_t PENDING_VAPORUM_TX;
extern uint64_t ASSETCHAINS_TIMELOCKGTE;
extern uint64_t ASSETCHAINS_ENDSUBSIDY[ASSETCHAINS_MAX_ERAS+1];
extern uint64_t ASSETCHAINS_REWARD[ASSETCHAINS_MAX_ERAS+1];
extern uint64_t ASSETCHAINS_HALVING[ASSETCHAINS_MAX_ERAS+1];
extern uint64_t ASSETCHAINS_DECAY[ASSETCHAINS_MAX_ERAS+1];
extern uint64_t ASSETCHAINS_NOTARY_PAY[ASSETCHAINS_MAX_ERAS+1];
extern uint64_t ASSETCHAINS_TIMEUNLOCKFROM;
extern uint64_t ASSETCHAINS_TIMEUNLOCKTO;

extern std::mutex vaporum_mutex;
//extern pthread_mutex_t vaporum_mutex;
extern pthread_mutex_t VAPORUM_CC_mutex;

/**
 * @brief Given a currency name, return the index in the VAPORUM_STATES array
 * 
 * @param origbase the currency name to look for
 * @return the index in the array, or -1
 */
int32_t vaporum_baseid(const char *origbase);

uint64_t vaporum_current_supply(uint32_t nHeight);
