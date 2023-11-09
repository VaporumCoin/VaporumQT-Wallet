/******************************************************************************
 * Copyright © 2014-2019 The SuperNET Developers.                             *
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
#pragma once
#include "uint256.h"
#include "chain.h"
#include "vaporum_structs.h"
#include "vaporum_utils.h"
#include "vaporum_curve25519.h"

//#include "vaporum_cJSON.h"
//#include "vaporum_bitcoind.h"
//#include "vaporum_interest.h"

// Todo:
// verify: reorgs


//#include "vaporum_kv.h"
//#include "vaporum_gateway.h"
//#include "vaporum_events.h"
//#include "vaporum_ccdata.h"
#include <cstdint>

const char VAPORUM_STATE_FILENAME[] = "vaporumevents";

int32_t vaporum_parsestatefile(struct vaporum_state *sp,FILE *fp,char *symbol, const char *dest);

void vaporum_currentheight_set(int32_t height);

int32_t vaporum_currentheight();

int32_t vaporum_parsestatefiledata(struct vaporum_state *sp,uint8_t *filedata,long *fposp,long datalen,const char *symbol, const char *dest);

void vaporum_stateupdate(int32_t height,uint8_t notarypubs[][33],uint8_t numnotaries,uint8_t notaryid,
        uint256 txhash,uint32_t *pvals,uint8_t numpvals,int32_t KMDheight,uint32_t KMDtimestamp,
        uint64_t opretvalue,uint8_t *opretbuf,uint16_t opretlen,uint16_t vout,uint256 MoM,int32_t MoMdepth);

int32_t vaporum_voutupdate(bool fJustCheck,int32_t *isratificationp,int32_t notaryid,
        uint8_t *scriptbuf,int32_t scriptlen,int32_t height,uint256 txhash,int32_t i,
        int32_t j,uint64_t *voutmaskp,int32_t *specialtxp,int32_t *notarizedheightp,
        uint64_t value,int32_t notarized,uint64_t signedmask,uint32_t timestamp);

int32_t vaporum_connectblock(bool fJustCheck, CBlockIndex *pindex,CBlock& block);
