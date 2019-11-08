//
// EasyLinkLayer.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode+
// https://embedXcode.weebly.com
//
// Details	    Additional layer for EasyLink
// Project 		sub1GHz_TX_Layer
//
// Created by 	Rei Vilo, 05 Nov 2019 11:00
// 				Rei Vilo
//
// Copyright 	(c) Rei Vilo, 2019
// Licence		CC = BY SA NC
//
// See 			EasyLinkLayer.h and ReadMe.txt for references
//


// Library header
#include "EasyLinkLayer.h"

/////
///// @brief    Addresses of the filter for incoming messages
///// @details  Up to three addresses, 0..2
///// @{
/////
//AddressFilter_t _addressFilter =
//{
//    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /// < Broadcast address
//    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, /// < Wrong hub address
//    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff  /// < Not used
//};
///// @}

EasyLinkLayer::EasyLinkLayer(bool flagAddressFiltering)
{
    _flagAddressFiltering = flagAddressFiltering;
}

EasyLink_Status EasyLinkLayer::begin()
{
    EasyLink_Status status = EasyLink_Status_Param_Error;

    status = EasyLink::begin();
    if (_flagAddressFiltering)
    {
        EasyLink_setCtrl(EasyLink_Ctrl_AddSize, EASYLINK_MAX_ADDR_SIZE);
        // Declare the authorised receiving addresses array
        status = EasyLink_enableRxAddrFilter((uint8_t*)&_addressFilter, EASYLINK_MAX_ADDR_SIZE, EASYLINK_MAX_ADDR_FILTERS);
    }

    return status;
}

EasyLink_Status EasyLinkLayer::setAddressFilter(uint8_t slot, AddressIEEE_t address)
{
    EasyLink_Status status = EasyLink_Status_Param_Error;

    if ((slot < EASYLINK_MAX_ADDR_FILTERS) and (_flagAddressFiltering))
    {
        memcpy(_addressFilter[slot], address, sizeof(AddressIEEE_t));
        status = EasyLink_Status_Success;

        // Update the filters
        status = EasyLink_enableRxAddrFilter((uint8_t*)&_addressFilter, EASYLINK_MAX_ADDR_SIZE, EASYLINK_MAX_ADDR_FILTERS);
    }
    return status;
}

EasyLink_Status EasyLinkLayer::getAddressLocal(AddressIEEE_t *ieeeAddress)
{
    return EasyLink_getIeeeAddr((uint8_t*)ieeeAddress);
}

EasyLink_Status EasyLinkLayer::getAddressFilter(uint8_t slot, AddressIEEE_t * address)
{
    EasyLink_Status status = EasyLink_Status_Param_Error;

    if ((slot < EASYLINK_MAX_ADDR_FILTERS) and (_flagAddressFiltering))
    {
        AddressIEEE_t work = { 0 };
        memcpy(work, &_addressFilter[slot], sizeof(AddressIEEE_t));
        memcpy(address, work, sizeof(AddressIEEE_t));
        status = EasyLink_Status_Success;
    }
    return status;
}

void EasyLinkLayer::setAddressDestinationTX(AddressIEEE_t ieeeAddress)
{
    memcpy(txPacket.dstAddr, ieeeAddress, sizeof(AddressIEEE_t));
}

EasyLink_Status EasyLinkLayer::transmit(const void * payload, size_t size)
{
    EasyLink_Status status = EasyLink_Status_Param_Error;

    if (size < EASYLINK_MAX_DATA_LENGTH)
    {
        memcpy(txPacket.payload, payload, size);

        // Set the length of the packet
        txPacket.len = size;
        // Transmit immediately
        txPacket.absTime = EasyLink_ms_To_RadioTime(0);

        status = EasyLink::transmit(&txPacket);
    }
    return status;
}

EasyLink_Status EasyLinkLayer::receive(void * payload, size_t size, uint32_t ms)
{
    rxPacket.rxTimeout = EasyLink_ms_To_RadioTime(ms);
    // Turn the receiver on immediately
    rxPacket.absTime = EasyLink_ms_To_RadioTime(0);

    EasyLink_Status status = EasyLink::receive(&rxPacket);

    if (status == EasyLink_Status_Success)
    {
        size = rxPacket.len;
        memcpy(payload, rxPacket.payload, rxPacket.len);
    }
    return status;
}

void EasyLinkLayer::getAddressDestinationRX(AddressIEEE_t * ieeeAddress)
{
    AddressIEEE_t work = { 0 };
    memcpy(work, &rxPacket.dstAddr, sizeof(AddressIEEE_t));
    memcpy(ieeeAddress, work, sizeof(AddressIEEE_t));
}

uint8_t EasyLinkLayer::getRSSIRX()
{
    return rxPacket.rssi;
}


// Code
