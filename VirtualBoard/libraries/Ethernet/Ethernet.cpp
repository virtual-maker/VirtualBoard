/* Copyright 2018 Paul Stoffregen
 *
 * Modified 2022 for the VirtualBoard project https://github.com/virtual-maker/VirtualBoard
 * Copyright (c) 2022 Immo Wache
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "Ethernet.h"

int EthernetClass::begin(uint8_t *mac_address, unsigned long timeout, unsigned long responseTimeout)
{
  IPAddress local_ip(192, 168, 1, 2);
  begin(mac_address, local_ip);
  return 1;
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip)
{
  // Assume the DNS server will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress dns_server = local_ip;
  dns_server[3] = 1;
  begin(mac_address, local_ip, dns_server);
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server)
{
  // Assume the gateway will be the machine on the same network as the local IP
  // but with last octet being '1'
  IPAddress gateway = local_ip;
  gateway[3] = 1;
  begin(mac_address, local_ip, dns_server, gateway);
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server, IPAddress gateway)
{
  // Assume the subnet will be 255.255.255.0
  IPAddress subnet(255, 255, 255, 0);
  begin(mac_address, local_ip, dns_server, gateway, subnet);
}

void EthernetClass::begin(uint8_t *mac, IPAddress local_ip, IPAddress dns_server, IPAddress gateway, IPAddress subnet)
{
  _localIpAddress = ethernetWrapper.localIpAddress();
  _dnsServerAddress = ethernetWrapper.dnsServerAddress();
  _gatewayAddress = ethernetWrapper.gatewayAddress();
  _subnetMask = ethernetWrapper.subnetMask();
}

int EthernetClass::maintain()
{
  return DHCP_CHECK_RENEW_OK;
}

IPAddress EthernetClass::localIP()
{
  return _localIpAddress;
}

IPAddress EthernetClass::subnetMask()
{
  return _subnetMask;
}

IPAddress EthernetClass::gatewayIP()
{
  return _gatewayAddress;
}

IPAddress EthernetClass::dnsServerIP()
{
  return _dnsServerAddress;
}

EthernetClass Ethernet;
