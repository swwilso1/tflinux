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

#include "tfdnsmasqservice.hpp"
#include "tfkeyvalueconfigfile.hpp"

namespace TF::Linux
{

    void DNSMasqService::load_configurations_from_file(const string_type & file,
                                                       network_configuration_map & configurations)
    {
        KeyValueConfigFile config_file{file};

        if (! config_file.file_exists())
        {
            return;
        }

        auto config_table = config_file.load_configuration();

        if (config_table.contains("interface"))
        {
            auto interface_name = config_table["interface"];
            std::shared_ptr<NetworkConfiguration> configuration{};
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

            if (config_table.contains("dhcp-range"))
            {
                auto dhcp_range_substrings = config_table["dhcp-range"].split(",");
                if (dhcp_range_substrings.size() > 0)
                {
                    configuration->dhcp_start_address = IPAddress::address_from_string(dhcp_range_substrings[0]);
                }
                if (dhcp_range_substrings.size() > 1)
                {
                    configuration->dhcp_end_address = IPAddress::address_from_string(dhcp_range_substrings[1]);
                }
            }
        }
    }

    void DNSMasqService::write_configurations_to_file(const network_configuration_map & configurations,
                                                      const string_type & file)
    {
        for (auto & pair : configurations)
        {
            auto & configuration = pair.second;
            if (configuration->enabled && configuration->wifi_interface)
            {
                KeyValueConfigFile config_file{file};
                KeyValueConfigFile::key_value_table_type table{};

                table["interface"] = configuration->interface.get_name();

                auto start_address = configuration->dhcp_start_address.get_presentation_name();
                auto end_address = configuration->dhcp_end_address.get_presentation_name();

                table["dhcp-range"] = string_type::initWithFormat("%@,%@,12h", start_address.get(), end_address.get());
                table["port"] = "0";
                table["bogus-priv"] = string_type{};
                table["dnssec"] = string_type{};

                config_file.save_configuration(table);
                break;
            }
        }
    }

} // namespace TF::Linux
