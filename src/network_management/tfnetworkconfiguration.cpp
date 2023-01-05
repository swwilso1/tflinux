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
    }

    auto NetworkConfiguration::get_name() const -> string_type
    {
        return interface.get_name();
    }

    void NetworkConfiguration::update_from(const NetworkConfiguration & config)
    {
        addr_mode = config.addr_mode;
        interface = config.interface;
	// If the interface is already enabled we probably do not want to disable it
	// when we have a config with a different enabled setting.  We use configs
	// that only have partial data and the new config might not have full enabled
	// information.
        enabled = enabled ? enabled : config.enabled;
    }

    void NetworkConfiguration::update_all_but_interface_from(const NetworkConfiguration & config)
    {
        addr_mode = config.addr_mode;
	// If the interface is already enabled we probably do not want to disable it
	// when we have a config with a different enabled setting.  We use configs
	// that only have partial data and the new config might not have full enabled
	// information.
        enabled = enabled ? enabled : config.enabled;
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

    std::ostream & operator<<(std::ostream & o, const NetworkConfiguration & c)
    {
        ClassFormatter * formatter = FormatterFactory::getFormatter();
        if (formatter != nullptr)
        {
            formatter->setClassName("NetworkConfiguration");
            formatter->addClassMember("address_mode", "addr_mode", c.addr_mode);
            formatter->addClassMember("network_interface", "interface", c.interface);
            formatter->addClassMember<bool>("bool", "enabled", c.enabled);
            o << *formatter;
            delete formatter;
        }
        return o;
    }

    void WirelessConfiguration::clear()
    {
        NetworkConfiguration::clear();
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

    void WirelessConfiguration::update_from(const WirelessConfiguration & config)
    {
        NetworkConfiguration::update_from(config);
        update_non_interface_details_from(config);
    }

    void WirelessConfiguration::update_all_but_interface_from(const WirelessConfiguration & config)
    {
        NetworkConfiguration::update_all_but_interface_from(config);
        update_non_interface_details_from(config);
    }

    void WirelessConfiguration::update_from(const NetworkConfiguration & config)
    {
        NetworkConfiguration::update_from(config);
    }

    void WirelessConfiguration::update_all_but_interface_from(const NetworkConfiguration & config)
    {
        NetworkConfiguration::update_all_but_interface_from(config);
    }

    void WirelessConfiguration::update_non_interface_details_from(const WirelessConfiguration & config)
    {
        channel = config.channel;
        mode = config.mode;
        standard = config.standard;
        ssid = config.ssid;
        password = config.password;
        wpa_mode = config.wpa_mode;
        wpa_key_management = config.wpa_key_management;
        wpa_pairwise = config.wpa_pairwise;
        rsn_pairwise = config.rsn_pairwise;
        dhcp_start_address = config.dhcp_start_address;
        dhcp_end_address = config.dhcp_end_address;
    }

    std::ostream & operator<<(std::ostream & o, const WirelessConfiguration::WifiMode & mode)
    {
        switch (mode)
        {
            case WirelessConfiguration::WifiMode::ACCESS_POINT:
                o << "ACCESS_POINT";
                break;
            case WirelessConfiguration::WifiMode::CLIENT:
                o << "CLIENT";
                break;
            case WirelessConfiguration::WifiMode::NONE:
                o << "NONE";
                break;
        }

        return o;
    }

    std::ostream & operator<<(std::ostream & o, const WirelessConfiguration::WifiStandard & standard)
    {
        switch (standard)
        {
            case WirelessConfiguration::WifiStandard::A:
                o << "A";
                break;
            case WirelessConfiguration::WifiStandard::B:
                o << "B";
                break;
            case WirelessConfiguration::WifiStandard::G:
                o << "G";
                break;
            case WirelessConfiguration::WifiStandard::N:
                o << "N";
                break;
            case WirelessConfiguration::WifiStandard::NONE:
                o << "NONE";
                break;
        }

        return o;
    }

    std::ostream & operator<<(std::ostream & o, const WirelessConfiguration & c)
    {
        ClassFormatter * formatter = FormatterFactory::getFormatter();
        if (formatter != nullptr)
        {
            formatter->setClassName("WirelessConfiguration");
            formatter->addClassMember("address_mode", "addr_mode", c.addr_mode);
            formatter->addClassMember("network_interface", "interface", c.interface);
            formatter->addClassMember<bool>("bool", "enabled", c.enabled);
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
            o << *formatter;
            delete formatter;
        }
        return o;
    }

} // namespace TF::Linux
