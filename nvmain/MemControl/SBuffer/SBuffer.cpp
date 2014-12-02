
#include "MemControl/SBuffer/SBuffer.h"
#include "MemControl/MemoryControllerFactory.h"
#include "Interconnect/InterconnectFactory.h"
#include "include/NVMHelpers.h"
#include "NVM/nvmain.h"
#include "Decoders/DRCDecoder/DRCDecoder.h"
#include "src/EventQueue.h"

#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdlib>


using namespace NVM;
SBuffer::SBuffer()
{
	std::cout << "Created a SBuffer!" << std::endl;
}

SBuffer::~SBuffer( )
{

}

void SBuffer::SetConfig( Config *conf, bool createChildren )
{
	 if( createChildren )
	 {
	 	 /* When selecting a child, use the bank field from the decoder. */
        AddressTranslator *rankAT = DecoderFactory::CreateDecoderNoWarn( conf->GetString( "Decoder" ) );
        rankAT->SetTranslationMethod( GetParent( )->GetTrampoline( )->GetDecoder( )->GetTranslationMethod( ) );
        rankAT->SetDefaultField( BANK_FIELD );
        rankAT->SetConfig( c, createChildren );
        SetDecoder( rankAT );

        std::cout << "Creating " << bankCount << " banks in all " 
            << deviceCount << " devices.\n";

        for( ncounter_t i = 0; i < bankCount; i++ )
        {
            std::stringstream formatter;

            Bank *nextBank = BankFactory::CreateBankNoWarn( conf->GetString( "BankType" ) );

            formatter << i;
            nextBank->SetId( i );
            formatter.str( "" );

            formatter << StatName( ) << ".bank" << i;
            nextBank->StatName( formatter.str( ) );

            nextBank->SetParent( this );
            AddChild( nextBank );

            /* SetConfig recursively. */
            nextBank->SetConfig( c, createChildren );
            nextBank->RegisterStats( );
        }

	 }

}