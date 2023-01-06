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

#include "tfnetworkconfiguration.hpp"

namespace TF::Linux
{
    void NetworkConfiguration::clear()
    {
        addr_mode = address_mode::NONE;
        interface.clear();
        enabled = false;
        wifi_interface = false;
        channel = 0;
        mode = wifi_mode::NONE;
        standard = wifi_standard::NONE;
        ssid = string_type{};
        password = string_type{};
        wpa_mode = 0;
        wpa_key_management = string_type{};
        wpa_pairwise = string_type{};
        rsn_pairwise = string_type{};
        dhcp_start_address = address_type{};
        dhcp_end_address = address_type{};
    }

    auto NetworkConfiguration::get_name() const -> string_type
    {
        return interface.get_name();
    }

    std::ostream & operator<<(std::ostream & o, const NetworkConfiguration::InterfaceAddressMode & addr_mode)
    {
        switch (addr_mode)
        {
            case NetworkConfiguration::InterfaceAddressMode::DHCP:
                o << "DHCP";
                break;
            case NetworkConfiguration::InterfaceAddressMode::STATIC:
                o << "STATIC";
                break;
            case NetworkConfiguration::InterfaceAddressMode::NONE:
                o << "NONE";
                break;
        }

        return o;
    }

    std::ostream & operator<<(std::ostream & o, const NetworkConfiguration::WifiMode & mode)
    {
        switch (mode)
        {
            case NetworkConfiguration::WifiMode::ACCESS_POINT:
                o << "ACCESS_POINT";
                break;
            case NetworkConfiguration::WifiMode::CLIENT:
                o << "CLIENT";
                break;
            case NetworkConfiguration::WifiMode::NONE:
                o << "NONE";
                break;
        }

        return o;
    }

    std::ostream & operator<<(std::ostream & o, const NetworkConfiguration::WifiStandard & standard)
    {
        switch (standard)
        {
            case NetworkConfiguration::WifiStandard::A:
                o << "A";
                break;
            case NetworkConfiguration::WifiStandard::B:
                o << "B";
                break;
            case NetworkConfiguration::WifiStandard::G:
                o << "G";
                break;
            case NetworkConfiguration::WifiStandard::N:
                o << "N";
                break;
            case NetworkConfiguration::WifiStandard::NONE:
                o << "NONE";
                break;
        }

        return o;
    }

    std::ostream & operator<<(std::ostream & o, const NetworkConfiguration & c)
    {
        ClassFormatter * formatter = FormatterFactory::getFormatter();
        if (formatter != nullptr)
        {
            formatter->setClassName("NetworkConfiguration");
            formatter->addClassMember("address_mode", "addr_mode", c.addr_mode);
            formatter->addClassMember("network_interface", "interface", c.interface);
            formatter->addClassMember<bool>("bool", "enabled", c.enabled);
            formatter->addClassMember<bool>("bool", "wifi_interface", c.wifi_interface);
            if (c.wifi_interface)
            {
                formatter->addClassMember<int32_t>("int32_t", "channel", c.channel);
                formatter->addClassMember("wifi_mode", "mode", c.mode);
                formatter->addClassMember("wifi_standard", "standard", c.standard);
                formatter->addClassMember("string_type", "ssid", c.ssid);
                formatter->addClassMember("string_type", "password", c.password);
                formatter->addClassMember<int>("int", "wpa_mode", c.wpa_mode);
                formatter->addClassMember("string_type", "wpa_key_management", c.wpa_key_management);
                formatter->addClassMember("string_type", "wpa_pairwise", c.wpa_pairwise);
                formatter->addClassMember("string_type", "rsn_pairwise", c.rsn_pairwise);
                formatter->addClassMember("address_type", "dhcp_start_address", c.dhcp_start_address);
                formatter->addClassMember("address_type", "dhcp_end_address", c.dhcp_end_address);
            }
            o << *formatter;
            delete formatter;
        }
        return o;
    }

} // namespace TF::Linux
