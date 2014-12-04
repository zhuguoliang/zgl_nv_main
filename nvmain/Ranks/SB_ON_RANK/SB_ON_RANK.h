/*******************************************************************************
* Copyright (c) 2012-2014, The Microsystems Design Labratory (MDL)
* Department of Computer Science and Engineering, The Pennsylvania State University
* All rights reserved.
* 
* This source code is part of NVMain - A cycle accurate timing, bit accurate
* energy simulator for both volatile (e.g., DRAM) and non-volatile memory
* (e.g., PCRAM). The source code is free and you can redistribute and/or
* modify it by providing that the following conditions are met:
* 
*  1) Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
* 
*  2) Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer in the documentation
*     and/or other materials provided with the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* Author list: 
*   zhuguoliang     Email:zhuguoliangcs@163.com
*******************************************************************************/

#ifndef __SB_ON_RANK_H__
#define __SB_ON_RANK_H__

#include "src/Rank.h"
#include "src/Bank.h"

#include <cstdint>
#include <list>
#include <iostream>

namespace NVM {


const int SBcleanupPriority = -10;
/*
 *  We use six rank states because our timing and energy parameters
 *  only tell us the delay of the entire read/write cycle to one bank.
 *  Even though all banks should be powered down in lockstep, we use three
 *  bank states to indicate different PowerDown modes. In addition, as all
 *  banks are powered up, some banks may be active directly according to
 *  different PowerDown states.
 *
 *  In the case of non-volatile memory, consecutive reads and writes do
 *  not need to consider the case when reads occur before tRAS, since
 *  data is not destroyed during read, and thus does not need to be
 *  written back to the row.
 */
enum SB_ON_RANK_State 
{ 
    SB_ON_RANK_UNKNOWN,   /***< Unknown state. Uh oh. */
    SB_ON_RANK_OPEN,      /***< Rank has at least one open bank  */
    SB_ON_RANK_CLOSED,    /***< all banks in the rank are closed (standby) */
    SB_ON_RANK_REFRESHING,/***< some banks in the rank are refreshing */
    SB_ON_RANK_PDPF,      /***< Rank is in precharge powered down, fast exit mode */
    SB_ON_RANK_PDA,       /***< Rank is in active powered down mode */
    SB_ON_RANK_PDPS       /***< Rank is in precharge powered down, slow exit mode */
};

class SB_ON_RANK : public Rank
{
  public:
    SB_ON_RANK( );
    ~SB_ON_RANK( );

    void SetConfig( Config *c, bool createChildren = true );

    bool IssueCommand( NVMainRequest *request );
    bool IsIssuable( NVMainRequest *request, FailReason *reason = NULL );
    void Notify( NVMainRequest *request );
    bool RequestComplete( NVMainRequest* );
    ncycle_t NextIssuable( NVMainRequest *request );

    void SetName( std::string name );

    bool Idle( );

    void Cycle( ncycle_t steps );

    void RegisterStats( );
    void CalculateStats( );
    void ResetStats( );


    void SBCleanupCallback( void *data );

  protected:
    Config *conf;
/*
    ncounter_t stateTimeout;
    uint64_t psInterval;
    SB_ON_RANK_State state;

    ncounter_t bankCount;
    ncounter_t deviceWidth;
    ncounter_t deviceCount;
    ncounter_t busWidth;
    ncycle_t* lastActivate;
    ncounter_t RAWindex;
    ncounter_t rawNum;
    ncounter_t banksPerRefresh;


    ncycle_t nextRead;
    ncycle_t nextWrite;
    ncycle_t nextActivate;
    ncycle_t nextPrecharge;

    ncounter_t activeCycles;
    ncounter_t standbyCycles;
    ncounter_t fastExitActiveCycles;
    ncounter_t fastExitPrechargeCycles;
    ncounter_t slowExitCycles;
    ncycle_t lastReset;

    ncounter_t rrdWaits;
    ncounter_t rrdWaitTotal;
    double rrdWaitAverage;
    ncounter_t fawWaits;
    ncounter_t fawWaitTotal;
    double fawWaitAverage;
    ncounter_t actWaits;
    ncounter_t actWaitTotal;
    double actWaitAverage;

    ncounter_t reads, writes;

    double totalEnergy, backgroundEnergy, activateEnergy, burstEnergy, refreshEnergy;
    double totalPower, backgroundPower, activatePower, burstPower, refreshPower;

    bool Activate( NVMainRequest *request );
    bool Read( NVMainRequest *request );
    bool Write( NVMainRequest *request );
    bool Precharge( NVMainRequest *request );
    bool Refresh( NVMainRequest *request );
    bool PowerDown( NVMainRequest *request );
    bool PowerUp( NVMainRequest *request );
    bool CanPowerDown( NVMainRequest *request );
    bool CanPowerUp( NVMainRequest *request );
*/


    void CycleRequestBuffer( );
/* 
*  zhuguoliang 
*  add these for Sync Buffer
*  SB is acting like memory controller here ! 
*  every req has < address data memcmd > in it
*  Simply buffering req can fufill our need
*
*/
  private:
    Rank * hrank;
    std::deque<NVMainRequest *> *memreq_buffer;
    double syncValue;
    //std::list<NVMainRequest *> *transactionQueues;
    //std::deque<NVMainRequest *> *commandQueues;
    ncounter_t max_buffer_size;
    bool buffer_draining;
};

};

#endif
