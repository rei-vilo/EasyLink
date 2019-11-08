///
/// @mainpage	sub1GHz_TX_Layer
///
/// @details	Additional layer for EasyLink
/// @n
/// @n
/// @n @a		Developed with [embedXcode+](https://embedXcode.weebly.com)
///
/// @author		Rei Vilo
/// @author     https://embeddedcomputing.weebly.com
/// @date		05 Nov 2019 10:56
/// @version	103
///
/// @copyright	(c) Rei Vilo, 2019
/// @copyright	CC = BY SA NC
///
/// @see		ReadMe.txt for references
///


///
/// @file		sub1GHz_TX_Layer.ino
/// @brief		Main sketch
///
/// @details    TX example for EasyLink additional layer
/// @n @a		Developed with [embedXcode+](https://embedXcode.weebly.com)
///
/// @author		Rei Vilo
/// @author     https://embeddedcomputing.weebly.com
/// @date		05 Nov 2019 10:56
/// @version    103
///
/// @copyright	(c) Rei Vilo, 2019
/// @copyright	CC = BY SA NC
///
/// @see		ReadMe.txt for references
/// @n
///


// Core library for code-sense - IDE-based
// !!! Help: http://bit.ly/2AdU7cu
#if defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#else // error
#error Platform not defined
#endif // end IDE

// Set parameters
///
/// @brief    Address filtering
/// @{
#define WITH_ADDRESS_FILTERING 1 ///< activated
#define WITHOUT_ADDRESS_FILTERING 0 ///< desacticated
#define ADDRESS_FILTERING WITH_ADDRESS_FILTERING ///< mode
/// @}

// Include application, user and local libraries
#include "EasyLinkLayer.h"
//#include "QuickDebug.h"


// Define structures and classes

// Define variables and constants
// EasyLinkLayer myLink;
#if (ADDRESS_FILTERING == WITH_ADDRESS_FILTERING)
EasyLinkLayer myLink(true);
#else
EasyLinkLayer myLink;
#endif

// Node address
AddressIEEE_t addressNode = { 0x00, 0x12, 0x4B, 0x00, 0x0A, 0x27, 0xCD, 0x6A };
AddressIEEE_t addressHub  = { 0x00, 0x12, 0x4B, 0x00, 0x0B, 0xC9, 0xDB, 0x04 };
AddressIEEE_t addressPhony = { 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa };
AddressIEEE_t addressZero = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
AddressIEEE_t addressLocal;

// Utilities

///
/// @brief    Print IEEE address
/// @param    address IEEE address
/// @param    prefix default=false, true=add 0x
///
void printAddress(AddressIEEE_t address, bool prefix = false)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (prefix)
        {
            Serial.print("0x");
        }
        if (address[i] < 0x10)
        {
            Serial.print("0");
        }
        Serial.print(address[i], HEX);
        if (i < 7)
        {
            if (prefix)
            {
                Serial.print(", ");
            }
            else
            {
                Serial.print(".");
            }
        }
    }
    Serial.println();
}

// Add setup code
// !!! Help: http://bit.ly/2BmKRzI
void setup()
{
    Serial.begin(115200);
    delay(500);
    Serial.println("=== sub1GHz_TX_Layer");
    
    // Begin defaults to EasyLink_Phy_50kbps2gfsk
    myLink.begin();
    Serial.println(myLink.version());
    
#if (ADDRESS_FILTERING == WITH_ADDRESS_FILTERING)
    // Get local address
    myLink.getAddressLocal(&addressLocal);
    Serial.print("addressLocal ");
    printAddress(addressLocal, true);
    
    // Add local address to filters
    myLink.setAddressFilter(1, addressLocal);
#endif
    
    pinMode(PUSH1, INPUT_PULLUP);
    pinMode(PUSH2, INPUT_PULLUP);
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
}

void loop()
{
    uint16_t value = 1 - digitalRead(PUSH1);
    uint16_t address = 1 - digitalRead(PUSH2);
    
    // QuickDebugln("address= %i, value= %i", address, value);
    Serial.print("address= ");
    Serial.print(address);
    Serial.print(", value= ");
    Serial.println(value);

#if (ADDRESS_FILTERING == WITH_ADDRESS_FILTERING)
    // Set destination address for TX message
    AddressIEEE_t * w;
    if (address > 0)
    {
        w = &addressHub;
    }
    else
    {
        w = &addressPhony;
        // w = &addressZero;
    }
    myLink.setAddressDestinationTX(*w);
#endif
    
    // Send the TX message
    EasyLink_Status status = myLink.transmit(&value, sizeof(uint16_t));
    
    if (status == EasyLink_Status_Success)
    {
        Serial.print("+ Packet sent to ");
        printAddress(*w, true);
        digitalWrite(GREEN_LED, HIGH);
        delay(100);
        digitalWrite(GREEN_LED, LOW);
    }
    else
    {
        Serial.print("! Error ");
        Serial.print(status);
        Serial.print(" (");
        Serial.print(myLink.getStatusString(status));
        Serial.println(")");
    }
    
    delay(1000);
}
