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

#include "tfhostapdservice.hpp"

namespace TF::Linux
{

    void HostapdService::load_configurations_from_file(const string_type & file,
                                                       network_configuration_map & configurations)
    {
        KeyValueConfigFile config_file{file};

        if (! config_file.file_exists())
        {
            return;
        }

        auto config_table = config_file.load_configuration();

        std::shared_ptr<NetworkConfiguration> configuration{};
        if (config_table.contains("interface"))
        {
            auto interface_name = config_table["interface"];
            if (configurations.contains(interface_name))
            {
                configuration = configurations[interface_name];
            }
            else
            {
                configuration = std::make_shared<NetworkConfiguration>();
                configurations.insert(std::make_pair(interface_name, configuration));
            }
            configuration->interface.set_name(config_table["interface"]);
            configuration->wifi_interface = true;
            configuration->mode = NetworkConfiguration::WifiMode::ACCESS_POINT;

            if (config_table.contains("ssid"))
            {
                configuration->ssid = config_table["ssid"];
            }
            if (config_table.contains("hw_mode"))
            {
                auto & hw_mode = config_table["hw_mode"];
                if (hw_mode == "a")
                {
                    configuration->standard = NetworkConfiguration::WifiStandard::A;
                }
                else if (hw_mode == "b")
                {
                    configuration->standard = NetworkConfiguration::WifiStandard::B;
                }
                else if (hw_mode == "g")
                {
                    configuration->standard = NetworkConfiguration::WifiStandard::G;
                }
                else if (hw_mode == "n")
                {
                    configuration->standard = NetworkConfiguration::WifiStandard::N;
                }
            }
            if (config_table.contains("channel"))
            {
                auto channel_value = Conversion::convertStringToInt(config_table["channel"]);
                if (channel_value)
                {
                    configuration->channel = channel_value.value();
                }
            }
            if (config_table.contains("wpa_passphrase"))
            {
                configuration->password = config_table["wpa_passphrase"];
            }
            if (config_table.contains("wpa"))
            {
                auto wpa_mode_value = Conversion::convertStringToInt(config_table["wpa"]);
                if (wpa_mode_value)
                {
                    configuration->wpa_mode = wpa_mode_value.value();
                }
            }
            if (config_table.contains("wpa_key_mgmt"))
            {
                configuration->wpa_key_management = config_table["wpa_key_mgmt"];
            }
            if (config_table.contains("wpa_pairwise"))
            {
                configuration->wpa_pairwise = config_table["wpa_pairwise"];
            }
            if (config_table.contains("rsn_pairwise"))
            {
                configuration->rsn_pairwise = config_table["rsn_pairwise"];
            }
        }
    }

    void HostapdService::write_configurations_to_file(const network_configuration_map & configurations,
                                                      const string_type & file)
    {
        for (auto & pair : configurations)
        {
            auto & configuration = pair.second;
            if (configuration->wifi_interface)
            {
                if (! configuration->enabled)
                {
                    return;
                }

                KeyValueConfigFile config_file{file};
                KeyValueConfigFile::key_value_table_type table{};

                auto interface_name = configuration->interface.get_name();
                if (interface_name.length() == 0)
                {
                    return;
                }

                table["interface"] = interface_name;
                table["driver"] = "nl80211";
                table["ssid"] = configuration->ssid;

                if (configuration->standard == NetworkConfiguration::WifiStandard::A)
                {
                    table["hw_mode"] = "a";
                }
                else if (configuration->standard == NetworkConfiguration::WifiStandard::B)
                {
                    table["hw_mode"] = "b";
                }
                else if (configuration->standard == NetworkConfiguration::WifiStandard::G)
                {
                    table["hw_mode"] = "g";
                }
                else if (configuration->standard == NetworkConfiguration::WifiStandard::N)
                {
                    table["hw_mode"] = "n";
                }

                table["channel"] = string_type::initWithFormat("%d", configuration->channel);
                table["macaddr_acl"] = "0";
                table["auth_algs"] = "1";
                table["ignore_broadcast_ssid"] = "0";

                table["wpa"] = string_type::initWithFormat("%d", configuration->wpa_mode);
                table["wpa_passphrase"] = configuration->password;

                if (configuration->wpa_key_management.empty())
                {
                    table["wpa_key_mgmt"] = "WPA-PSK";
                }
                else
                {
                    table["wpa_key_mgmt"] = configuration->wpa_key_management;
                }

                if (configuration->wpa_pairwise.empty())
                {
                    table["wpa_pairwise"] = "TKIP";
                }
                else
                {
                    table["wpa_pairwise"] = configuration->wpa_pairwise;
                }

                if (configuration->rsn_pairwise.empty())
                {
                    table["rsn_pairwise"] = "CCMP";
                }
                else
                {
                    table["rsn_pairwise"] = configuration->rsn_pairwise;
                }

                config_file.save_configuration(table);

                break;
            }
        }
    }

} // namespace TF::Linux
