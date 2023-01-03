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

#ifndef TFSYSTEMNETWORKINTERFACE_HPP
#define TFSYSTEMNETWORKINTERFACE_HPP

#include "TFFoundation.hpp"

using namespace TF::Foundation;

namespace TF::Linux
{

    class SystemNetworkInterface : public NetworkInterface
    {
    public:
        SystemNetworkInterface();
        explicit SystemNetworkInterface(const NetworkInterface & interface);

        SystemNetworkInterface & operator=(const NetworkInterface & interface);

        void add_nameserver_address(const address_type & address);

        [[nodiscard]] auto get_nameservers() const -> address_list_type;

        void add_gateway_address(const address_type & address);

        [[nodiscard]] auto get_gateways() const -> address_list_type;

        void clear();

        void clear_gateways();

        void clear_nameservers();

    private:
        using base_type = NetworkInterface;

        address_list_type m_nameserver_addresses{};
        address_list_type m_gateway_addresses{};
    };

} // namespace TF::Linux

#endif // TFSYSTEMNETWORKINTERFACE_HPP
