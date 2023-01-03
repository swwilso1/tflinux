/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2023 to 2023 Tectiform Inc.

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

#ifndef TFDHCPCD_HPP
#define TFDHCPCD_HPP

#include <vector>
#include "TFFoundation.hpp"
#include "tfnetworkconfiguration.hpp"

namespace TF::Linux
{

    struct DHCPCD
    {
        bool rapid_commit{true};
        bool domain_name{true};
        bool domain_search{true};
        bool classless_static_routes{true};
        bool host_name{true};
        bool interface_mtu{true};
        bool domain_name_servers{true};
        bool slaac_hwaddr{false};
        bool slaac_private{true};
        bool dhcp_server_identifier{true};
        bool hostname{true};
        bool clientid{true};
        bool duid{false};
        bool persistent{true};
        std::vector<EthernetConfiguration> interfaces{};

        DHCPCD() = default;
    };

} // namespace TF::Linux

#endif // TFDHCPCD_HPP
