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
#include "vaporum_events.h"
#include "vaporum_globals.h"
#include "vaporum_bitcoind.h" // vaporum_verifynotarization
#include "vaporum_notary.h" // vaporum_notarized_update
#include "vaporum_utils.h" // portable_mutex_lock / unlock
#include "vaporum_kv.h"

#define VAPORUM_EVENT_RATIFY 'P'
#define VAPORUM_EVENT_NOTARIZED 'N'
#define VAPORUM_EVENT_KMDHEIGHT 'K'
#define VAPORUM_EVENT_REWIND 'B'
#define VAPORUM_EVENT_PRICEFEED 'V'
#define VAPORUM_EVENT_OPRETURN 'R'

/*****
 * Add a notarized event to the collection
 * @param sp the state to add to
 * @param symbol
 * @param height
 * @param ntz the event
 */
void vaporum_eventadd_notarized( vaporum_state *sp, const char *symbol, int32_t height, vaporum::event_notarized& ntz)
{
    if (IS_VAPORUM_NOTARY)   {
        int32_t ntz_verify = vaporum_verifynotarization(symbol, ntz.dest, height, ntz.notarizedheight, ntz.blockhash, ntz.desttxid);
        LogPrint("notarisation", "vaporum_verifynotarization result %d\n", ntz_verify);

        if (ntz_verify < 0)    {
            static uint32_t counter;
            if ( counter++ < 100 )
                LogPrintf("[%s] error validating notarization ht.%d notarized_height.%d, if on a pruned %s node this can be ignored\n",
                        chainName.symbol().c_str(), height, ntz.notarizedheight, ntz.dest);
            return;
        }
    }
    
    if (chainName.isSymbol(symbol) || chainName.isKMD() && std::string(symbol) == "KMD" /*special case for KMD*/)
    {
        if (sp != nullptr)
        {
            sp->add_event(symbol, height, ntz);
            vaporum_notarized_update(sp, height, ntz.notarizedheight, ntz.blockhash, ntz.desttxid, ntz.MoM, ntz.MoMdepth);
        } else {
            LogPrintf("could not update notarisation event: vaporum_state is null");
        }
    } else {
        LogPrintf("could not update notarisation event: invalid symbol %s", symbol);
    }
}

/*****
 * Add a pubkeys event to the collection
 * @param sp where to add
 * @param symbol
 * @param height
 * @param pk the event
 */
void vaporum_eventadd_pubkeys(vaporum_state *sp, const char *symbol, int32_t height, vaporum::event_pubkeys& pk)
{
    if (sp != nullptr)
    {
        sp->add_event(symbol, height, pk);
        vaporum_notarysinit(height, pk.pubkeys, pk.num);
    }
}

/********
 * Add a pricefeed event to the collection
 * @note was for PAX, deprecated
 * @param sp where to add
 * @param symbol
 * @param height
 * @param pf the event
 */
void vaporum_eventadd_pricefeed( vaporum_state *sp, const char *symbol, int32_t height, vaporum::event_pricefeed& pf)
{
    if (sp != nullptr)
    {
        sp->add_event(symbol, height, pf);
    }
}

/*****
 * Add an opreturn event to the collection
 * @param sp where to add
 * @param symbol
 * @param height
 * @param opret the event
 */
void vaporum_eventadd_opreturn( vaporum_state *sp, const char *symbol, int32_t height, vaporum::event_opreturn& opret)
{
    if ( sp != nullptr && !chainName.isKMD() )
    {
        sp->add_event(symbol, height, opret);
        //vaporum_opreturn(height, opret->value, opret->opret.data(), opret->opret.size(), opret->txid, opret->vout, symbol);
        if ( opret.opret.data()[0] == 'K' && opret.opret.size() != 40 )
        {
            vaporum_kvupdate(opret.opret.data(), opret.opret.size(), opret.value);
        }
    }
}

/*****
 * @brief Undo an event
 * @note seems to only work for KMD height events
 * @param sp the state object
 * @param ev the event to undo
 */
template<class T>
void vaporum_event_undo(vaporum_state *sp, T& ev)
{
}

template<>
void vaporum_event_undo(vaporum_state* sp, vaporum::event_kmdheight& ev)
    {
    if ( ev.height <= sp->SAVEDHEIGHT )
        sp->SAVEDHEIGHT = ev.height;
    }
 


void vaporum_event_rewind(vaporum_state *sp, const char *symbol, int32_t height)
{
    if ( sp != nullptr )
    {
        if ( chainName.isKMD() && height <= VAPORUM_LASTMINED && prevVAPORUM_LASTMINED != 0 )
        {
            LogPrintf("undo VAPORUM_LASTMINED %d <- %d\n",VAPORUM_LASTMINED,prevVAPORUM_LASTMINED);
            VAPORUM_LASTMINED = prevVAPORUM_LASTMINED;
            prevVAPORUM_LASTMINED = 0;
        }
        while ( sp->events.size() > 0)
        {
            auto ev = sp->events.back();
            if (ev-> height < height)
                    break;
            vaporum_event_undo(sp, *ev);
            sp->events.pop_back();
        }
    }
}

void vaporum_setkmdheight(struct vaporum_state *sp,int32_t kmdheight,uint32_t timestamp)
{
    if ( sp != nullptr )
    {
        if ( kmdheight > sp->SAVEDHEIGHT )
        {
            sp->SAVEDHEIGHT = kmdheight;
            sp->SAVEDTIMESTAMP = timestamp;
        }
        if ( kmdheight > sp->CURRENT_HEIGHT )
            sp->CURRENT_HEIGHT = kmdheight;
    }
}

/******
 * @brief handle a height change event (forward or rewind)
 * @param sp
 * @param symbol
 * @param height
 * @param kmdht the event
 */
void vaporum_eventadd_kmdheight(struct vaporum_state *sp, const char *symbol,int32_t height,
        vaporum::event_kmdheight& kmdht)
{
    if (sp != nullptr)
    {
        if ( kmdht.kheight > 0 ) // height is advancing
        {

            sp->add_event(symbol, height, kmdht);
            vaporum_setkmdheight(sp, kmdht.kheight, kmdht.timestamp);
        }
        else // rewinding
        {
            vaporum::event_rewind e(height);
            sp->add_event(symbol, height, e);
            vaporum_event_rewind(sp,symbol,height);
        }
    }
}
