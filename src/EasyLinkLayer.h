///
/// @file		EasyLinkLayer.h
/// @brief		Library header
/// @details	Additional layer for EasyLink
/// @n
/// @n @b		Project sub1GHz_TX_Layer
/// @n @a		Developed with embedXcode+: https://embedXcode.weebly.com
///
/// @author		Rei Vilo
/// @author		Rei Vilo
///
/// @date		05 Nov 2019 11:00
/// @version	<#version#>
///
/// @copyright	(c) Rei Vilo, 2019
/// @copyright	<#licence#>
///
/// @see		ReadMe.txt for references
///


// Core library for code-sense - IDE-based
// !!! Help: http://bit.ly/2AdU7cu
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(ROBOTIS) // Robotis specific
#include "libpandora_types.h"
#include "pandora.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#elif defined(LITTLEROBOTFRIENDS) // LittleRobotFriends specific
#include "LRF.h"
#elif defined(MICRODUINO) // Microduino specific
#include "Arduino.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(REDBEARLAB) // RedBearLab specific
#include "Arduino.h"
#elif defined(RFDUINO) // RFduino specific
#include "Arduino.h"
#elif defined(SPARK) // Spark specific
#include "application.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif // end IDE

#ifndef EASYLINKLAYER_RELEASE
///
/// @brief	Release
///
#define EASYLINKLAYER_RELEASE

#include "EasyLink.h"

///
/// @brief      IEEE address
/// @details    uint8_t[8]
/// @note       CC13xx is little endian
///             + (uint8_t[8])00.12.4B.00.0A.27.CD.6A
///             + but (uint64_t)6ACD270A004B1200
/// @see        http://www.ti.com/lit/ug/swcu117h/swcu117h.pdf
///
typedef uint8_t AddressIEEE_t[8];

///
/// @brief      Addresses filter array
/// @details    EASYLINK_MAX_ADDR_SIZE set to 3
///
typedef AddressIEEE_t AddressFilter_t[EASYLINK_MAX_ADDR_SIZE];

//extern AddressFilter_t addressFilter;

///
/// @brief      EasyLinkLayer
/// @details    Additional layer with address filtering for EasyLink
/// @note       Basic usage: begin(), transmit() and receive()
///
class EasyLinkLayer : public EasyLink
{
  public:
    ///
    /// @brief    Constructor
    /// @param    flagAddressFiltering Enable address filtering, default=false
    /// @note     Basic usage: no parameter, no filtering
    /// @warning  Both RX and TX need to have the same configuration
    ///
    EasyLinkLayer(bool flagAddressFiltering = false);

    ///
    /// @brief    Initialise and start the radio
    /// @note     Basic usage: begin()
    ///
    EasyLink_Status begin();

    ///
    /// @brief    Send a message
    /// @param    payload pointer to the payload
    /// @param    size size of the payload
    /// @return   EasyLink_Status_Success or EasyLink_Status_Param_Error
    /// @note     Maximum payload size is EASYLINK_MAX_DATA_LENGTH=128
    /// @note     Basic usage: transmit(payload, size)
    ///
    EasyLink_Status transmit(const void * payload, size_t size);
    
    ///
    /// @brief    Receive a message
    /// @param    payload:out pointer to the payload
    /// @param    size:out size of the payload
    /// @param    ms period to receive, default=2 seconds, time-out after
    /// @return   EasyLink_Status_Success or EasyLink_Status_Param_Error
    /// @note     Maximum payload size is EASYLINK_MAX_DATA_LENGTH=128
    /// @note     Basic usage: receive(&payload, &size)
    ///
    EasyLink_Status receive(void * payload, size_t size, uint32_t ms = 2000);

    ///
    /// @brief    Set address to addresses filter
    /// @param    slot 0, 1 or 2
    /// @param    address IEEE address to add to filters
    /// @return   EasyLink_Status_Success or EasyLink_Status_Param_Error
    /// @note     Up to three addresses can be added to the filter.
    /// @n        Recommended allocation of addresses
    /// * @b Hub
    ///     * not used
    ///     * specific IEEE address of the hub
    ///     * generic address for commissioning nodes to hub
    /// * @b Node
    ///     * general broadcast from hub to all nodes
    ///     * specific IEEE address of the node
    ///     * not used
    /// @warning     Not part of basic usage
    ///
    EasyLink_Status setAddressFilter(uint8_t slot, AddressIEEE_t address);

    ///
    /// @brief      Get address to addresses filter
    /// @param      slot:in 0, 1 or 2
    /// @param      address:out IEEE address read from filters
    /// @return     EasyLink_Status_Success or EasyLink_Status_Param_Error
    /// @note       The filter contains up to three addresses.
    /// @warning     Not part of basic usage
    ///
    EasyLink_Status getAddressFilter(uint8_t slot, AddressIEEE_t * address);

    ///
    /// @brief      Get local address
    /// @details    IEEE address uint8_t[8]
    /// @param:out  ieeeAddress pointer to uint8_t[8]
    /// @return     EasyLink_Status
    ///
    EasyLink_Status getAddressLocal(AddressIEEE_t * ieeeAddress);

    ///
    /// @brief      Set destination for TX message
    /// @param      ieeeAddress destination address, uint8_t[8]
    /// @warning    Not part of basic usage
    ///
    void setAddressDestinationTX(AddressIEEE_t ieeeAddress);

    ///
    /// @brief      Get destination from RX message
    /// @param:out  ieeeAddress destination address, uint8_t[8]
    /// @warning    Not part of basic usage
    ///
    void getAddressDestinationRX(AddressIEEE_t * ieeeAddress);

    ///
    /// @brief      Get RSSI from RX message
    /// @return     RSSI level
    ///
    uint8_t getRSSIRX();

  private:
    bool _flagAddressFiltering;
    AddressFilter_t _addressFilter;
    EasyLink_TxPacket txPacket;
    EasyLink_RxPacket rxPacket;
};


#endif // EASYLINKLAYER_RELEASE
