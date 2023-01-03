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

#include "tfdhcpcdconfiguration.hpp"
#include "ANTLRFileStream.h"
#include "CommonTokenStream.h"
#include "dhcpcdLexer.h"
#include "dhcpcdParser.h"
#include "tfdhcpcdvisitor.hpp"

using namespace antlr4;
using namespace TF::Linux::dhcpcd;

namespace TF::Linux
{
    auto DHCPCDConfiguration::load_configurations_from_file(const string_type & file)
        -> std::pair<wireless_configuration_list, ethernet_configuration_list>
    {
        wireless_configuration_list wireless_list{};
        ethernet_configuration_list ethernet_list{};
        auto file_stl_string = file.stlString();
        ANTLRFileStream file_stream{};
        file_stream.loadFromFile(file_stl_string);
        dhcpcdLexer lexer{&file_stream};
        CommonTokenStream token_stream{&lexer, 0};
        dhcpcdParser parser{&token_stream};
        parser.setBuildParseTree(true);
        auto tree = parser.config_file();
        ServicesdDhcpcdVisitor visitor{};
        visitor.visit(tree);
        m_configuration = visitor.get_configuration();

        for (auto & interface : m_configuration.interfaces)
        {
            ethernet_list.emplace_back(interface);
        }

        return {wireless_list, ethernet_list};
    }

    void DHCPCDConfiguration::write_configurations_to_file(const wireless_configuration_list & wireless_list,
                                                           const ethernet_configuration_list & ethernet_list,
                                                           const string_type & file)
    {
        // Ignore the wireless configuration list.
        (void)wireless_list;

        auto config_file = FileHandle::fileHandleForWritingAtPath(file, true);
        if (m_configuration.hostname)
        {
            config_file.writeString("hostname\n");
        }
        if (m_configuration.clientid)
        {
            config_file.writeString("clientid\n");
        }
        if (m_configuration.duid)
        {
            config_file.writeString("duid\n");
        }
        if (m_configuration.persistent)
        {
            config_file.writeString("persistent\n");
        }
        if (m_configuration.rapid_commit)
        {
            config_file.writeString("option rapid_commit\n");
        }

        if (m_configuration.domain_name_servers || m_configuration.domain_name || m_configuration.domain_search ||
            m_configuration.hostname)
        {
            config_file.writeString("option ");
        }
        bool needs_comma{false};
        if (m_configuration.domain_name_servers)
        {
            config_file.writeString("domain_name_servers");
            needs_comma = true;
        }
        if (m_configuration.domain_name)
        {
            if (needs_comma)
            {
                config_file.writeString(", ");
            }
            config_file.writeString("domain_name");
            needs_comma = true;
        }
        if (m_configuration.domain_search)
        {
            if (needs_comma)
            {
                config_file.writeString(", ");
            }
            config_file.writeString("domain_search");
            needs_comma = true;
        }
        if (m_configuration.hostname)
        {
            if (needs_comma)
            {
                config_file.writeString(", ");
            }
            config_file.writeString("host_name");
        }
        if (m_configuration.domain_name_servers || m_configuration.domain_name || m_configuration.domain_search ||
            m_configuration.hostname)
        {
            config_file.writeString("\n");
        }

        if (m_configuration.classless_static_routes)
        {
            config_file.writeString("option classless_static_routes\n");
        }
        if (m_configuration.interface_mtu)
        {
            config_file.writeString("option interface_mtu\n");
        }
        if (m_configuration.dhcp_server_identifier)
        {
            config_file.writeString("require dhcp_server_identifier\n");
        }
        if (m_configuration.slaac_private)
        {
            config_file.writeString("slaac private\n");
        }
        if (m_configuration.slaac_hwaddr)
        {
            config_file.writeString("slaac hwaddr\n");
        }

        auto write_configuration_to_file = [&config_file](const NetworkConfiguration & config) -> void {
            if (config.enabled && config.addr_mode == NetworkConfiguration::address_mode::STATIC)
            {
                auto interface_name = config.interface.get_name();
                auto interface_line = string_type::initWithFormat("interface %@\n", &interface_name);
                config_file.writeString(interface_line);

                for (auto & addr_mask : config.interface.get_addresses_and_netmasks())
                {
                    string_type ip_address_string{};
                    auto address_in_cidr = addr_mask.get_as_cidr_notation();
                    if (addr_mask.address.is_ipv4_address())
                    {
                        ip_address_string = "ip_address";
                    }
                    else if (addr_mask.address.is_ipv6_address())
                    {
                        ip_address_string = "ip6_address";
                    }
                    auto line = string_type::initWithFormat("static %@=%@\n", &ip_address_string, &address_in_cidr);
                    config_file.writeString(line);
                }

                auto write_address_list_to_file = [&config_file](const string_type & phrase,
                                                                 std::vector<IPAddress> & address_list) -> void {
                    auto size = address_list.size();
                    if (size > 0)
                    {
                        config_file.writeString(phrase);
                    }
                    for (decltype(size) i = 0; i < size; i++)
                    {
                        auto presentation_name = address_list[i].get_presentation_name();
                        config_file.writeString(*presentation_name);
                        if (i < (size - 1))
                        {
                            config_file.writeString(", ");
                        }
                    }
                    if (size > 0)
                    {
                        config_file.writeString("\n");
                    }
                };

                auto gateways = config.interface.get_gateways();
                write_address_list_to_file("static routers=", gateways);

                auto nameservers = config.interface.get_nameservers();
                write_address_list_to_file("static domain_name_servers=", nameservers);

                config_file.writeString("\n");
            }
        };

        std::for_each(ethernet_list.cbegin(), ethernet_list.cend(), write_configuration_to_file);
        std::for_each(wireless_list.cbegin(), wireless_list.cend(), write_configuration_to_file);
    }
} // namespace TF::Linux
