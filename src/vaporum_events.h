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
#include "vaporum_defs.h"
#include "vaporum_structs.h"

void vaporum_eventadd_notarized(vaporum_state *sp, const char *symbol,int32_t height, vaporum::event_notarized& ntz);

void vaporum_eventadd_pubkeys(vaporum_state *sp, const char *symbol,int32_t height, vaporum::event_pubkeys& pk);

void vaporum_eventadd_pricefeed(vaporum_state *sp, const char *symbol,int32_t height, vaporum::event_pricefeed& pf);

void vaporum_eventadd_opreturn(vaporum_state *sp, const char *symbol,int32_t height, vaporum::event_opreturn& opret);

void vaporum_eventadd_kmdheight(vaporum_state *sp, const char *symbol,int32_t height, vaporum::event_kmdheight& kmd_ht);

void vaporum_event_rewind(vaporum_state *sp, const char *symbol,int32_t height);

void vaporum_setkmdheight(vaporum_state *sp,int32_t kmdheight,uint32_t timestamp);
