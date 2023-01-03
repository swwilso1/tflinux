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

#include "tfdnsmasqconfiguration.hpp"
#include "tfkeyvalueconfigfile.hpp"

namespace TF::Linux
{

    auto DNSMasqConfiguration::load_configurations_from_file(const string_type & file)
        -> std::pair<wireless_configuration_list, ethernet_configuration_list>
    {
        wireless_configuration_list wireless_list{};
        ethernet_configuration_list ethernet_list{};

        KeyValueConfigFile config_file{file};

        if (! config_file.file_exists())
        {
            return {};
        }

        auto config_table = config_file.load_configuration();

        if (config_table.contains("interface"))
        {
            WirelessConfiguration wireless_config{};
            wireless_config.interface.set_name(config_table["interface"]);

            if (config_table.contains("dhcp-range"))
            {
                auto dhcp_range_substrings = config_table["dhcp-range"].split(",");
                if (dhcp_range_substrings.size() > 0)
                {
                    wireless_config.dhcp_start_address = IPAddress::address_from_string(dhcp_range_substrings[0]);
                }
                if (dhcp_range_substrings.size() > 1)
                {
                    wireless_config.dhcp_end_address = IPAddress::address_from_string(dhcp_range_substrings[1]);
                }
            }

            wireless_list.emplace_back(wireless_config);
        }

        return {wireless_list, ethernet_list};
    }

    void DNSMasqConfiguration::write_configurations_to_file(const wireless_configuration_list & wireless_list,
                                                            const ethernet_configuration_list & ethernet_list,
                                                            const string_type & file)
    {
        // We ignore the ethernet list for this configuration.
        (void)ethernet_list;

        if (wireless_list.size() == 0)
        {
            return;
        }

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

        KeyValueConfigFile config_file{file};
        KeyValueConfigFile::key_value_table_type table{};

        table["interface"] = wireless_config.interface.get_name();

        auto start_address = wireless_config.dhcp_start_address.get_presentation_name();
        auto end_address = wireless_config.dhcp_end_address.get_presentation_name();

        table["dhcp-range"] = string_type::initWithFormat("%@,%@,12h", start_address.get(), end_address.get());
        table["port"] = "0";
        table["bogus-priv"] = string_type{};
        table["dnssec"] = string_type{};

        config_file.save_configuration(table);
    }

} // namespace TF::Linux
