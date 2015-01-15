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
*   Matt Poremba    ( Email: mrp5060 at psu dot edu 
*                     Website: http://www.cse.psu.edu/~poremba/ )
*******************************************************************************/

#ifndef __EASY_CTRL_H__
#define __EASY_CTRL_H__

#include "src/MemoryController.h"
#include <deque>

namespace NVM {

class EASY_CTRL : public MemoryController
{
  public:
    EASY_CTRL( );
    ~EASY_CTRL( );

    bool IssueCommand( NVMainRequest *req );
    bool IsIssuable( NVMainRequest *request, FailReason *fail = NULL );
    bool RequestComplete( NVMainRequest * request );

    void SetConfig( Config *conf, bool createChildren = true );

    void Cycle( ncycle_t steps );

    void RegisterStats( );
    void CalculateStats( );

  private:
    NVMTransactionQueue *memQueue;

    /* Cached Configuration Variables*/
    uint64_t queueSize;

    /* Stats */
    uint64_t measuredLatencies, measuredQueueLatencies, measuredTotalLatencies;
    double averageLatency, averageQueueLatency, averageTotalLatency;
    uint64_t mem_reads, mem_writes;
    uint64_t rb_hits;
    uint64_t rb_miss;
    uint64_t starvation_precharges;
    uint64_t cpu_insts;
    uint64_t write_pauses;
};

};

#endif
