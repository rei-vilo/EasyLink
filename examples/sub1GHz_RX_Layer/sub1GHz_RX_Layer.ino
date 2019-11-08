///
/// @mainpage	sub1GHz_RX_Layer
///
/// @details	Additional layer for EasyLink
/// @n
/// @n
/// @n @a		Developed with [embedXcode+](https://embedXcode.weebly.com)
///
/// @author		Rei Vilo
/// @author		https://embeddedcomputing.weebly.com
/// @date		05 Nov 2019 10:56
/// @version	103
///
/// @copyright	(c) Rei Vilo, 2019
/// @copyright	CC = BY SA NC
///
/// @see		ReadMe.txt for references
///


///
/// @file		sub1GHz_RX_Layer.ino
/// @brief		Main sketch
///
/// @details    RX example for EasyLink additional layer
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
//EasyLinkLayer myLink;
#if (ADDRESS_FILTERING == WITH_ADDRESS_FILTERING)
EasyLinkLayer myLink(true);
#else
EasyLinkLayer myLink;
#endif

// Hub address
AddressIEEE_t addressHub = { 0x00, 0x12, 0x4B, 0x00, 0x0B, 0xCA, 0x27, 0x82 };
AddressIEEE_t addressNode = { 0x00, 0x12, 0x4B, 0x00, 0x0A, 0x27, 0xCD, 0x6A }; // Node address

AddressIEEE_t addressLocal;
uint16_t value;

// Prototypes
// !!! Help: http://bit.ly/2l0ZhTa


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

///
/// @brief    Print the addresses of the filter
/// @param    title default="Filter"
///
void printFilter(String title = "Filter")
{
    AddressIEEE_t work;
    Serial.println(title);

    for (uint8_t i = 0; i < EASYLINK_MAX_ADDR_FILTERS; i++)
    {
        // QuickDebug("Slot %i: ", i);

        Serial.print("Slot ");
        Serial.print(i);
        Serial.print("= ");

        myLink.getAddressFilter(i, &work);

        printAddress(work, true);
    }
}


// Functions

// Add setup code
// !!! Help: http://bit.ly/2BmKRzI
void setup()
{
    Serial.begin(115200);
    delay(500);
    Serial.println("=== sub1GHz_RX_Layer");

    // Begin defaults to EasyLink_Phy_50kbps2gfsk
    myLink.begin();
    Serial.println(myLink.version());

#if (ADDRESS_FILTERING == WITH_ADDRESS_FILTERING)
    // Get local address
    myLink.getAddressLocal(&addressLocal);
    //    // Other options
    //    // . as ui8[8] 00.12.4B.00.0A.27.CD.6A
    //    // . as ui64   6ACD270A.004B1200
    //    uint64_t * address64 = (uint64_t*)&addressLocal;
    //    Serial.print((uint32_t)(*address64 >> 32), HEX);
    //    Serial.print(".");
    //    Serial.print((uint32_t)(*address64 & 0xffffffff), HEX);
    Serial.print("addressLocal: ");
    printAddress(addressLocal, true);
    Serial.println("!!! Add this address as destination on sub1GHz_TX_Layer");

    // Add local address to filters
    myLink.setAddressFilter(1, addressLocal);
#endif

    // List the addresses of the filter
    printFilter();

    myLink.setAddressFilter(1, addressLocal);

    Serial.println();
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
}

void loop()
{
    uint8_t buffer[EASYLINK_MAX_DATA_LENGTH];
    size_t size;
    AddressIEEE_t work;

    // Read the RX message, if any within 2 seconds
    EasyLink_Status status = myLink.receive(&buffer, size, 2000);

    if (status == EasyLink_Status_Success)
    {
        memcpy(&value, &buffer, sizeof(uint16_t));

        // QuickDebug("+ Packet received len=%i value=%i RSSI=%i sent to ", size, value, myLink.getRSSIRX());
        Serial.print("+ Packet received len=");
        Serial.print(size);
        Serial.print(" value=");
        Serial.print(value);
        Serial.print(" RSSI=");
        Serial.print(myLink.getRSSIRX());
        Serial.print(" sent to ");
        Serial.print(value);
        myLink.getAddressDestinationRX(&work);
        printAddress(work);

        digitalWrite(RED_LED, value);
    }
    else
    {
        Serial.print("! Error ");
        Serial.print(status);
        Serial.print(" (");
        Serial.print(myLink.getStatusString(status));
        Serial.println(")");
    }
}
