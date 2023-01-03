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

#include "tfhostapdconfiguration.hpp"

namespace TF::Linux
{

    auto HostapdConfiguration::load_configurations_from_file(const string_type & file)
        -> std::pair<wireless_configuration_list, ethernet_configuration_list>
    {
        KeyValueConfigFile config_file{file};
        wireless_configuration_list wireless_list{};
        ethernet_configuration_list ethernet_list{};

        if (! config_file.file_exists())
        {
            return {};
        }

        auto config_table = config_file.load_configuration();

        WirelessConfiguration wireless_config{};
        if (config_table.contains("interface"))
        {
            wireless_config.interface.set_name(config_table["interface"]);
        }
        if (config_table.contains("ssid"))
        {
            wireless_config.ssid = config_table["ssid"];
        }
        if (config_table.contains("hw_mode"))
        {
            auto & hw_mode = config_table["hw_mode"];
            if (hw_mode == "a")
            {
                wireless_config.standard = WirelessConfiguration::WifiStandard::A;
            }
            else if (hw_mode == "b")
            {
                wireless_config.standard = WirelessConfiguration::WifiStandard::B;
            }
            else if (hw_mode == "g")
            {
                wireless_config.standard = WirelessConfiguration::WifiStandard::G;
            }
            else if (hw_mode == "n")
            {
                wireless_config.standard = WirelessConfiguration::WifiStandard::N;
            }
        }
        if (config_table.contains("channel"))
        {
            auto channel_value = Conversion::convertStringToInt(config_table["channel"]);
            if (channel_value)
            {
                wireless_config.channel = channel_value.value();
            }
        }
        if (config_table.contains("wpa_passphrase"))
        {
            wireless_config.password = config_table["wpa_passphrase"];
        }
        if (config_table.contains("wpa"))
        {
            auto wpa_mode_value = Conversion::convertStringToInt(config_table["wpa"]);
            if (wpa_mode_value)
            {
                wireless_config.wpa_mode = wpa_mode_value.value();
            }
        }
        if (config_table.contains("wpa_key_mgmt"))
        {
            wireless_config.wpa_key_management = config_table["wpa_key_mgmt"];
        }
        if (config_table.contains("wpa_pairwise"))
        {
            wireless_config.wpa_pairwise = config_table["wpa_pairwise"];
        }
        if (config_table.contains("rsn_pairwise"))
        {
            wireless_config.rsn_pairwise = config_table["rsn_pairwise"];
        }

        if (wireless_config.interface.get_name() != string_type{})
        {
            wireless_list.emplace_back(wireless_config);
        }

        return {wireless_list, ethernet_list};
    }

    void HostapdConfiguration::write_configurations_to_file(const wireless_configuration_list & wireless_list,
                                                            const ethernet_configuration_list & ethernet_list,
                                                            const string_type & file)
    {
        // This service ignores the ethernet configurations.
        (void)ethernet_list;

        if (wireless_list.size() == 0)
        {
            return;
        }

        KeyValueConfigFile config_file{file};
        KeyValueConfigFile::key_value_table_type table{};

        WirelessConfiguration wireless_config{};
        for (auto & config : wireless_list)
        {
            if (config.enabled)
            {
                wireless_config = config;
                break;
            }
        }

        if (! wireless_config.enabled)
        {
            return;
        }

        auto interface_name = wireless_config.interface.get_name();
        if (interface_name.length() == 0)
        {
            return;
        }

        table["interface"] = interface_name;
        table["driver"] = "nl80211";
        table["ssid"] = wireless_config.ssid;

        if (wireless_config.standard == WirelessConfiguration::WifiStandard::A)
        {
            table["hw_mode"] = "a";
        }
        else if (wireless_config.standard == WirelessConfiguration::WifiStandard::B)
        {
            table["hw_mode"] = "b";
        }
        else if (wireless_config.standard == WirelessConfiguration::WifiStandard::G)
        {
            table["hw_mode"] = "g";
        }
        else if (wireless_config.standard == WirelessConfiguration::WifiStandard::N)
        {
            table["hw_mode"] = "n";
        }

        table["channel"] = string_type::initWithFormat("%d", wireless_config.channel);
        table["mac_addr_acl"] = "0";
        table["auth_algs"] = "1";
        table["ignore_broadcast_ssid"] = "0";

        table["wpa"] = string_type::initWithFormat("%d", wireless_config.wpa_mode);
        table["wpa_passphrase"] = wireless_config.password;

        if (wireless_config.wpa_key_management.empty())
        {
            table["wpa_key_mgmt"] = "WPA-PSK";
        }
        else
        {
            table["wpa_key_mgmt"] = wireless_config.wpa_key_management;
        }

        if (wireless_config.wpa_pairwise.empty())
        {
            table["wpa_pairwise"] = "TKIP";
        }
        else
        {
            table["wpa_pairwise"] = wireless_config.wpa_pairwise;
        }

        if (wireless_config.rsn_pairwise.empty())
        {
            table["rsn_pairwise"] = "CCMP";
        }
        else
        {
            table["rsn_pairwise"] = wireless_config.rsn_pairwise;
        }

        config_file.save_configuration(table);
    }

} // namespace TF::Linux
