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

#ifndef TFNETWORKCONFIGURATION_HPP
#define TFNETWORKCONFIGURATION_HPP

#include <ostream>
#include "TFFoundation.hpp"
#include "tfsystemnetworkinterface.hpp"

using namespace TF::Foundation;

namespace TF::Linux
{
    struct NetworkConfiguration
    {
        enum class InterfaceAddressMode
        {
            DHCP,
            STATIC,
            NONE,
        };

        enum class WifiStandard
        {
            A,
            B,
            G,
            N,
            NONE,
        };

        enum class WifiMode
        {
            CLIENT,
            ACCESS_POINT,
            NONE,
        };

        using address_mode = InterfaceAddressMode;
        using wifi_standard = WifiStandard;
        using wifi_mode = WifiMode;

        using string_type = String;
        using address_type = IPAddress;
        using address_and_netmask_type = IPAddressAndNetmask;
        using network_interface = SystemNetworkInterface;

        address_mode addr_mode{};
        network_interface interface {};
        bool enabled{false};
        bool wifi_interface{false};
        int32_t channel{0};
        wifi_mode mode{};
        wifi_standard standard{};
        string_type ssid{};
        string_type password{};
        int wpa_mode{};
        string_type wpa_key_management{};
        string_type wpa_pairwise{};
        string_type rsn_pairwise{};
        address_type dhcp_start_address{};
        address_type dhcp_end_address{};

        void clear();

        [[nodiscard]] auto get_name() const -> string_type;
    };

    std::ostream & operator<<(std::ostream & o, const NetworkConfiguration::InterfaceAddressMode & addr_mode);
    std::ostream & operator<<(std::ostream & o, const NetworkConfiguration::WifiMode & mode);
    std::ostream & operator<<(std::ostream & o, const NetworkConfiguration::WifiStandard & standard);
    std::ostream & operator<<(std::ostream & o, const NetworkConfiguration & c);

} // namespace TF::Linux

#endif // TFNETWORKCONFIGURATION_HPP
