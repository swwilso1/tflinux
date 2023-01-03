/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2022 to 2022 Tectiform Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


******************************************************************************/

#include "tfsystemnetworkinterface.hpp"

namespace TF::Linux
{

    SystemNetworkInterface::SystemNetworkInterface() : NetworkInterface() {}

    SystemNetworkInterface::SystemNetworkInterface(const NetworkInterface & interface) : NetworkInterface{interface} {}

    SystemNetworkInterface & SystemNetworkInterface::operator=(const NetworkInterface & interface)
    {
        if (this == &interface)
        {
            return *this;
        }

        NetworkInterface::operator=(interface);
        m_nameserver_addresses.clear();
        m_gateway_addresses.clear();

        return *this;
    }

    void SystemNetworkInterface::add_nameserver_address(const address_type & address)
    {
        m_nameserver_addresses.emplace_back(address);
    }

    auto SystemNetworkInterface::get_nameservers() const -> address_list_type
    {
        return m_nameserver_addresses;
    }

    void SystemNetworkInterface::add_gateway_address(const address_type & address)
    {
        m_gateway_addresses.emplace_back(address);
    }

    auto SystemNetworkInterface::get_gateways() const -> address_list_type
    {
        return m_gateway_addresses;
    }

    void SystemNetworkInterface::clear()
    {
        base_type::clear();
        m_gateway_addresses.clear();
        m_nameserver_addresses.clear();
    }

    void SystemNetworkInterface::clear_gateways()
    {
        m_gateway_addresses.clear();
    }

    void SystemNetworkInterface::clear_nameservers()
    {
        m_nameserver_addresses.clear();
    }

} // namespace TF::Linux
