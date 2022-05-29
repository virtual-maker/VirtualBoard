/*
  This file is part of the WiFiNINA library.
  Copyright (c) 2018 Arduino SA. All rights reserved.

  Modified 2022 for the VirtualBoard project https://github.com/virtual-maker/VirtualBoard
  Copyright (c) 2022 Immo Wache

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef WiFiNINA_h
#define WiFiNINA_h

#define WiFiClient EthernetClient
#define WiFiServer EthernetServer
#define WiFiUDP EthernetUDP

#include "IPAddress.h"

typedef enum {
  WL_NO_SHIELD = 255,
  WL_NO_MODULE = WL_NO_SHIELD,
  WL_IDLE_STATUS = 0,
  WL_NO_SSID_AVAIL,
  WL_SCAN_COMPLETED,
  WL_CONNECTED,
  WL_CONNECT_FAILED,
  WL_CONNECTION_LOST,
  WL_DISCONNECTED,
  WL_AP_LISTENING,
  WL_AP_CONNECTED,
  WL_AP_FAILED
} wl_status_t;

/* Encryption modes */
enum wl_enc_type {  /* Values map to 802.11 encryption suites... */
  ENC_TYPE_WEP = 5,
  ENC_TYPE_TKIP = 2,
  ENC_TYPE_CCMP = 4,
  /* ... except these two, 7 and 8 are reserved in 802.11-2007 */
  ENC_TYPE_NONE = 7,
  ENC_TYPE_AUTO = 8,

  ENC_TYPE_UNKNOWN = 255
};


class WiFiClass
{
  private:
    IPAddress _dnsServerAddress;
    IPAddress _localIP;
    const char* _ssid;
  public:
    WiFiClass() {}

    /*
     * Get firmware version
     */
    static const char* firmwareVersion()
    {
      return "1.0.0";
    }


    /* Start Wifi connection for OPEN networks
     *
     * param ssid: Pointer to the SSID string.
     */
    int begin(const char* ssid)
    {
      _ssid = ssid;
      return 0;
    }

    /* Start Wifi connection with WEP encryption.
     * Configure a key into the device. The key type (WEP-40, WEP-104)
     * is determined by the size of the key (5 bytes for WEP-40, 13 bytes for WEP-104).
     *
     * param ssid: Pointer to the SSID string.
     * param key_idx: The key index to set. Valid values are 0-3.
     * param key: Key input buffer.
     */
    int begin(const char* ssid, uint8_t key_idx, const char* key)
    {
      return begin(ssid);
    }

    /* Start Wifi connection with passphrase
     * the most secure supported mode will be automatically selected
     *
     * param ssid: Pointer to the SSID string.
     * param passphrase: Passphrase. Valid characters in a passphrase
     *        must be between ASCII 32-126 (decimal).
     */
    int begin(const char* ssid, const char *passphrase)
    {
      return begin(ssid);
    }

    uint8_t beginAP(const char *ssid)
    {
      return begin(ssid);
    }
    uint8_t beginAP(const char *ssid, uint8_t channel)
    {
      return begin(ssid);
    }
    uint8_t beginAP(const char *ssid, const char* passphrase)
    {
      return begin(ssid);
    }
    uint8_t beginAP(const char *ssid, const char* passphrase, uint8_t channel)
    {
      return begin(ssid);
    }

    /* Change Ip configuration settings disabling the dhcp client
        *
        * param local_ip: 	Static ip configuration
        */
    void config(IPAddress local_ip)
    {
      _localIP = local_ip;
    }

    /* Change Ip configuration settings disabling the dhcp client
        *
        * param local_ip: 	Static ip configuration
    * param dns_server:     IP configuration for DNS server 1
        */
    void config(IPAddress local_ip, IPAddress dns_server)
    {
      _localIP = local_ip;
    }

    /* Change Ip configuration settings disabling the dhcp client
        *
        * param local_ip: 	Static ip configuration
    * param dns_server:     IP configuration for DNS server 1
        * param gateway : 	Static gateway configuration
        */
    void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway)
    {
      _localIP = local_ip;
    }

    /* Change Ip configuration settings disabling the dhcp client
        *
        * param local_ip: 	Static ip configuration
    * param dns_server:     IP configuration for DNS server 1
        * param gateway: 	Static gateway configuration
        * param subnet:		Static Subnet mask
        */
    void config(IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
    {
      _localIP = local_ip;
    }

    /* Change DNS Ip configuration
     *
     * param dns_server1: ip configuration for DNS server 1
     */
    void setDNS(IPAddress dns_server1)
    {
    }

    /* Change DNS Ip configuration
     *
     * param dns_server1: ip configuration for DNS server 1
     * param dns_server2: ip configuration for DNS server 2
     *
     */
    void setDNS(IPAddress dns_server1, IPAddress dns_server2)
    {
    }


    /* Set the hostname used for DHCP requests
     *
     * param name: hostname to set
     *
     */
    void setHostname(const char* name)
    {
    }

    /*
     * Disconnect from the network
     *
     * return: one value of wl_status_t enum
     */
    int disconnect(void)
    {
      return WL_DISCONNECTED;
    }

    void end(void)
    {
    }

    /*
     * Get the interface MAC address.
     *
     * return: pointer to uint8_t array with length WL_MAC_ADDR_LENGTH
     */
    uint8_t* macAddress(uint8_t* mac)
    {
      return 0;
    }

    /*
     * Get the interface IP address.
     *
     * return: Ip address value
     */
    IPAddress localIP()
    {
      return _localIP;
    }

    /*
     * Get the interface subnet mask address.
     *
     * return: subnet mask address value
     */
    IPAddress subnetMask()
    {
      return IPAddress(0, 0, 0, 0);
    }

    /*
     * Get the gateway ip address.
     *
     * return: gateway ip address value
     */
    IPAddress gatewayIP()
    {
      return IPAddress(0, 0, 0, 0);
    }

    /*
     * Return the current SSID associated with the network
     *
     * return: ssid string
     */
    const char* SSID()
    {
      return _ssid;
    }

    /*
      * Return the current BSSID associated with the network.
      * It is the MAC address of the Access Point
      *
      * return: pointer to uint8_t array with length WL_MAC_ADDR_LENGTH
      */
    uint8_t* BSSID(uint8_t* bssid)
    {
      return 0;
    }

    /*
      * Return the current RSSI /Received Signal Strength in dBm)
      * associated with the network
      *
      * return: signed value
      */
    int32_t RSSI()
    {
      return 50;
    }

    /*
      * Return the Encryption Type associated with the network
      *
      * return: one value of wl_enc_type enum
      */
    uint8_t	encryptionType()
    {
      return ENC_TYPE_TKIP;
    }

    /*
     * Start scan WiFi networks available
     *
     * return: Number of discovered networks
     */
    int8_t scanNetworks()
    {
      return 1;
    }

    /*
     * Return the SSID discovered during the network scan.
     *
     * param networkItem: specify from which network item want to get the information
    *
     * return: ssid string of the specified item on the networks scanned list
     */
    const char*	SSID(uint8_t networkItem)
    {
      return _ssid;
    }

    /*
     * Return the encryption type of the networks discovered during the scanNetworks
     *
     * param networkItem: specify from which network item want to get the information
    *
     * return: encryption type (enum wl_enc_type) of the specified item on the networks scanned list
     */
    uint8_t	encryptionType(uint8_t networkItem)
    {
      return ENC_TYPE_TKIP;
    }

    uint8_t* BSSID(uint8_t networkItem, uint8_t* bssid)
    {
      return 0;
    }
    uint8_t channel(uint8_t networkItem)
    {
      return 7;
    }

    /*
     * Return the RSSI of the networks discovered during the scanNetworks
     *
     * param networkItem: specify from which network item want to get the information
    *
     * return: signed value of RSSI of the specified item on the networks scanned list
     */
    int32_t RSSI(uint8_t networkItem)
    {
      return 50;
    }

    /*
     * Return Connection status.
     *
     * return: one of the value defined in wl_status_t
     */
    uint8_t status()
    {
      return WL_CONNECTED;
    }

    /*
     * Resolve the given hostname to an IP address.
     * param aHostname: Name to be resolved
     * param aResult: IPAddress structure to store the returned IP address
     * result: 1 if aIPAddrString was successfully converted to an IP address,
     *          else error code
     */
    int hostByName(const char* aHostname, IPAddress& aResult)
    {
      return 0;
    }

    unsigned long getTime()
    {
      return 0;
    }

    void lowPowerMode()
    {

    }
    void noLowPowerMode()
    {

    }

    int ping(const char* hostname, uint8_t ttl = 128)
    {
      return 0;
    }
    int ping(const String &hostname, uint8_t ttl = 128)
    {
      return 0;
    }
    int ping(IPAddress host, uint8_t ttl = 128)
    {
      return 0;
    }
};

WiFiClass WiFi;

#endif
