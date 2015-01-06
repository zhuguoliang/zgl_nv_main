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

#include "Ranks/SB_ON_RANK/SB_ON_RANK.h"
#include "src/EventQueue.h"
#include "Banks/BankFactory.h"
#include "NVM/nvmain.h"/*zhuguoliang*/
#include "include/NVMHelpers.h"/*zhuguoliang*/
#include "Ranks/RankFactory.h"/*zhuguoliang*/
#include "src/EventQueue.h"/*zhuguoliang*/
#include <iostream>
#include <sstream>
#include <cassert>
#include <algorithm>/*zhuguoliang for std::remove_if*/
using namespace NVM;
/* Command queue removal predicate. */
bool WasIssuedSB( NVMainRequest *request );
bool WasIssuedSB( NVMainRequest *request ) { return (request->flags & NVMainRequest::FLAG_ISSUED_BY_SB); }

std::string GetFilePath( std::string file );

SB_ON_RANK::SB_ON_RANK( )
{
    //no state
    buffer_draining= false;
    max_buffer_size= 32;
    memreq_buffer= NULL;
    memreq_buffer = new std::deque<NVMainRequest *> [max_buffer_size];
    syncValue = 0.0f;

}

SB_ON_RANK::~SB_ON_RANK( )
{
    delete [] memreq_buffer; 
}

void SB_ON_RANK::SetConfig( Config *c, bool createChildren )
{
    //std::cout<< "SB_ON_RANK named " << StatName()<< std::endl;
    conf = c;

    Params *params = new Params( );
    params->SetParams( c );
    SetParams( params );
    

    if( createChildren )
    {
        
        std::vector<NVMObject_hook *>::iterator it;


        //std::vector<NVMObject_hook *>& childNodes = GetChildren( );
        //childNodes.clear();

        ///////////////////////////////////////////////////////////////////////////////////////////
        /* Orphan the bank created by interconnect?? I don`t know if I should call it here */
        /*
        
        for( it = childNodes.begin(); it != childNodes.end(); it++ )
        {
            std::cout << "The children hook name is " << (*it)->StatName( ) << std::endl;
        }
        
        childNodes.clear();
        */
        ///////////////////////////////////////////////////////////////////////////////////////////       
        /* When selecting a child, use the bank field from the decoder. */
        
//should set below to set decoder but SB_ON_RANK is only one layer and has only one child


        /*
        AddressTranslator *rankAT = DecoderFactory::CreateDecoderNoWarn( conf->GetString( "Decoder" ) );
        rankAT->SetTranslationMethod( GetParent( )->GetTrampoline( )->GetDecoder( )->GetTranslationMethod( ) );
        rankAT->SetDefaultField( RANK_FIELD );
        rankAT->SetConfig( c, createChildren );
        SetDecoder( rankAT );
        */
        /* Initialize ranks in config file, named HRANK_CONFIG*/
        std::string configFile;
        Config *Heterogeneous_RankConfig;

        configFile  = NVM::GetFilePath( c->GetFileName( ) );
        configFile += c->GetString( "HRANK_CONFIG" );
        std::cout << "the configFile for the new rank is " << configFile << std::endl;
        Heterogeneous_RankConfig = new Config( );
        Heterogeneous_RankConfig->Read( configFile );

        hrank= RankFactory::CreateRankNoWarn( Heterogeneous_RankConfig->GetString( "RankType" ) );

        EventQueue *hrankEventQueue = new EventQueue( );
        hrank->SetParent( this );
        hrank->SetEventQueue( hrankEventQueue);
        /* Shoud add NVMain pointer here instead of hrank*/
        GetGlobalEventQueue( )->AddSystem( hrank,  Heterogeneous_RankConfig);

        std::stringstream formatter;
        
        formatter.str( "" );
        formatter << StatName( ) << ".hrank";
        hrank->StatName( formatter.str( ) );
        hrank->SetConfig( Heterogeneous_RankConfig,createChildren );
        
        

        //std::cout<< "SB_ON_RANK named " << hrank->StatName()<< std::endl;
        //hrank->SetParent( this );
        AddChild( hrank );
        //hrank->SetParent( this );
        
        hrank->RegisterStats( );
        /*
        std::vector<NVMObject_hook *>& childNodes = GetChildren( );
        for( it = childNodes.begin(); it != childNodes.end(); it++ )
        {
            std::cout << "The children hook name is " << (*it)->StatName( ) << std::endl;
        }
        */
            /* SetConfig recursively. */
            //hrank->SetConfig( conf, createChildren ); 
    }

    SetDebugName( "SB_ON_RANK", c );
}

void SB_ON_RANK::RegisterStats( )
{
    //already done in SB_ON_RANK::SETCONFIG. so don't need to do this here
    //GetChild()->GetTrampoline( )->RegisterStats( );
}

bool SB_ON_RANK::Idle( )
{
    return GetChild()->GetTrampoline( ) ->Idle();
}

/*

bool SB_ON_RANK::Activate( NVMainRequest *request )
{
   
}

bool SB_ON_RANK::Read( NVMainRequest *request )
{
    
}


bool SB_ON_RANK::Write( NVMainRequest *request )
{
   
}

bool SB_ON_RANK::Precharge( NVMainRequest *request )
{
   
}

bool SB_ON_RANK::CanPowerDown( NVMainRequest *request )
{
    
}

bool SB_ON_RANK::PowerDown( NVMainRequest *request )
{
    
}

bool SB_ON_RANK::CanPowerUp( NVMainRequest *request )
{
    
}

bool SB_ON_RANK::PowerUp( NVMainRequest *request )
{
   
}

*/

/*
 * refresh is issued to those banks that start from the bank specified by the
 * request.  
 */
/*
bool SB_ON_RANK::Refresh( NVMainRequest *request )
{
    
}
*/
ncycle_t SB_ON_RANK::NextIssuable( NVMainRequest *)
{
    
    //zhuguoliang
    std::cout<< "Warining!!!===========Calling SB_ON_RANK::NextIssuable I don't know how to deal with\n";
    NVMainRequest *queueHead = memreq_buffer->at( 0 );

            //std::cout << "0x" << std::hex << queueHead->address.GetPhysicalAddress( )
            //          << " issuable at " << std::dec 
            //          << GetChild( )->NextIssuable( queueHead ) << std::endl;
            
    return GetChild( )->NextIssuable( queueHead );
}


//the next req that is issuable
bool SB_ON_RANK::IsIssuable( NVMainRequest *req, FailReason *)
{
   bool rv = true;

    /* during a request drain, no req can enqueue */
    /* only have one child which is hrank ask it if the req is isissuable*/
   //if((memreq_buffer->size() >= max_buffer_size) || (buffer_draining ==true))
    if((memreq_buffer->size() >= max_buffer_size) || (buffer_draining ==true) || (!(GetChild( )->IsIssuable(req))))
    {
        rv = false;
    }

    if(rv==false){
        std::cout<<" NOT ISSUABLE from SB_ON_RANK \n";
        std::cout<<req->type<<std::endl;
    }


    return rv;
}



bool SB_ON_RANK::IssueCommand( NVMainRequest *req )
{
    bool rv = false;

    if( !IsIssuable( req ) )
    {
        std::cout<< "Sync buffer full or buffer_draining on going\n";
        return false;
    }
    else
    {
        rv = true;
        
        switch( req->type )
        {
            case ACTIVATE:
                memreq_buffer->push_back( req);
                break;
            
            case READ:
            case READ_PRECHARGE:
                memreq_buffer->push_back( req);
                break;
            
            case WRITE:
            case WRITE_PRECHARGE:
                memreq_buffer->push_back( req);
                break;
            
            case PRECHARGE:
            case PRECHARGE_ALL:
                memreq_buffer->push_back( req);
                break;

            case POWERDOWN_PDA:
            case POWERDOWN_PDPF: 
            case POWERDOWN_PDPS: 
                memreq_buffer->push_back( req);
                break;

            case POWERUP:
                memreq_buffer->push_back( req);
                break;
        
            case REFRESH:
                memreq_buffer->push_back( req);
                break;

            default:
                std::cout << "NVMain:unknown request from MC! " 
                    << req->type << std::endl;
                rv=false;
                break;  
        }
    }

    return rv;
}

/* 
 *  Other ranks should notify us when they read/write so we can ensure minimum 
 *  timings are met.
 */
void SB_ON_RANK::Notify( NVMainRequest *request )
{
            GetChild( )->Notify( request );
}

bool SB_ON_RANK::RequestComplete( NVMainRequest* req )
{
    
    //maybe should del req from queue?
    return GetParent( )->RequestComplete( req );
}




void SB_ON_RANK::SBCleanupCallback( void * /*data*/ )
{
    
    memreq_buffer->erase(
            std::remove_if( memreq_buffer->begin(),
                            memreq_buffer->end(),
                            WasIssuedSB),
            memreq_buffer->end()
        );
//    std::cout<< "SB_ON_RANK::SBCleanupCallback I am called on cycle "<<  GetEventQueue()->GetCurrentCycle() <<std::endl;
}

void SB_ON_RANK::CycleRequestBuffer( )
{
    FailReason fail;
    if( !memreq_buffer->empty( )
            && GetChild( )->IsIssuable( memreq_buffer->at( 0 ), &fail ) )
    {
        NVMainRequest *queueHead = memreq_buffer->at( 0 );
        GetChild( )->IssueCommand( queueHead );
        queueHead->flags |= NVMainRequest::FLAG_ISSUED_BY_SB;

        /* Get this cleaned this up. */
        ncycle_t cleanupCycle = GetEventQueue()->GetCurrentCycle() + 1;
        bool cleanupScheduled = GetEventQueue()->FindCallback( this, 
                                        (CallbackPtr)&SB_ON_RANK::SBCleanupCallback,
                                        cleanupCycle, NULL, SBcleanupPriority );

            if( !cleanupScheduled )
                GetEventQueue( )->InsertCallback( this, 
                                  (CallbackPtr)&SB_ON_RANK::SBCleanupCallback,
                                  cleanupCycle, NULL, SBcleanupPriority );
    }

}



void SB_ON_RANK::Cycle( ncycle_t steps )
{
    
    /* Sync the momory clock with the bus clock. */
    //double cpuFreq = static_cast<double>(p->CPUFreq);
    double busFreq = static_cast<double>(p->CLK);
    double memFreq = static_cast<double>(p->MEM_Freq);


    //std::cout<<"MEM_Freq is "<< p->MEM_Freq<<std::endl;


    syncValue += static_cast<double>( memFreq / busFreq );

    
    if( syncValue >= 1.0f )
    {
        syncValue -= 1.0f;
    }
    else
    {
        return;
    }


//    std::cout<<"SB_ON_RANK::Cycle current memreq_buffer->size() = "<<memreq_buffer->size()<<std::endl;

    /* Issue memory commands from the request buffer. */
    CycleRequestBuffer( );

    /* 
    *  zhuguoliang
    *  only have one child which is a hrank
    */
    assert(steps==1);
    for( ncounter_t childIdx = 0; childIdx < GetChildCount( ); childIdx++ )
        GetChild( childIdx )->Cycle( steps );//step always is 1

//let the clock run
    GetChild( )->GetTrampoline()->GetEventQueue()->Loop( steps );

}

void SB_ON_RANK::CalculateStats( )
{
    GetChild( )->CalculateStats( );
}

void SB_ON_RANK::ResetStats( )
{
    GetChild( )->ResetStats( );
}

