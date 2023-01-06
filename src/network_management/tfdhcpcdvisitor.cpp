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

#include "tfdhcpcdvisitor.hpp"

namespace TF::Linux
{
    std::any ServicesdDhcpcdVisitor::visitDeclHelper1(dhcpcdParser::DeclHelper1Context * context)
    {
        return visit(context->decl());
    }

    std::any ServicesdDhcpcdVisitor::visitDeclHelper2(dhcpcdParser::DeclHelper2Context * context)
    {
        visit(context->config_file());
        return visit(context->decl());
    }

    std::any ServicesdDhcpcdVisitor::visitCommentHelper(dhcpcdParser::CommentHelperContext * context)
    {
        return visit(context->comment());
    }

    std::any ServicesdDhcpcdVisitor::visitOtherHelper(dhcpcdParser::OtherHelperContext * context)
    {
        return visit(context->otherKeywordDecl());
    }

    std::any ServicesdDhcpcdVisitor::visitOptionHelper(dhcpcdParser::OptionHelperContext * context)
    {
        std::any result;
        auto thing = context->optionDecl();
        for (auto & value : thing->optionValue())
        {
            result = visit(value);
        }
        return result;
    }

    std::any ServicesdDhcpcdVisitor::visitRequireHelper(dhcpcdParser::RequireHelperContext * context)
    {
        return visit(context->requireDecl());
    }

    std::any ServicesdDhcpcdVisitor::visitSlaacHelper(dhcpcdParser::SlaacHelperContext * context)
    {
        return visit(context->slaacDecl());
    }

    std::any ServicesdDhcpcdVisitor::visitInterfaceHelper(dhcpcdParser::InterfaceHelperContext * context)
    {
        return visit(context->interfaceDecl());
    }

    std::any ServicesdDhcpcdVisitor::visitControlHelper(dhcpcdParser::ControlHelperContext * context)
    {
        return visit(context->controlGroupDecl());
    }

    std::any ServicesdDhcpcdVisitor::visitOneLineComment(dhcpcdParser::OneLineCommentContext * context)
    {
        (void)context;
        return {};
    }

    std::any ServicesdDhcpcdVisitor::visitMultiLineComment(dhcpcdParser::MultiLineCommentContext * context)
    {
        return visit(context->comment());
    }

    std::any ServicesdDhcpcdVisitor::visitInterfaceDetail(dhcpcdParser::InterfaceDetailContext * context)
    {
        if (! m_ethernet_stack.empty())
        {
            auto & config = m_ethernet_stack.top();
            m_configuration.configurations.emplace_back(config);
            m_ethernet_stack.pop();
        }

        auto text = context->TEXT()->getText();
        NetworkConfiguration config{};
        config.interface.set_name(text);
        config.enabled = true;
        config.addr_mode = NetworkConfiguration::address_mode::STATIC;
        m_ethernet_stack.push(config);
        return {};
    }

    std::any ServicesdDhcpcdVisitor::visitStaticHelper(dhcpcdParser::StaticHelperContext * context)
    {
        visit(context->interfaceDecl());
        return visit(context->staticLine());
    }

    std::any ServicesdDhcpcdVisitor::visitIpv4Address(dhcpcdParser::Ipv4AddressContext * context)
    {
        auto text = context->TEXT()->getText();
        return add_address_and_netmask(text);
    }

    std::any ServicesdDhcpcdVisitor::visitIpv6Address(dhcpcdParser::Ipv6AddressContext * context)
    {
        auto text = context->TEXT()->getText();
        return add_address_and_netmask(text);
    }

    std::any ServicesdDhcpcdVisitor::visitRouters(dhcpcdParser::RoutersContext * context)
    {
        auto any_server_list = visit(context->server_list());
        auto server_list = std::any_cast<std::vector<std::string>>(any_server_list);
        auto & config = m_ethernet_stack.top();
        for (auto & server : server_list)
        {
            config.interface.add_gateway_address(IPAddress::address_from_string(server));
        }
        return {};
    }

    std::any ServicesdDhcpcdVisitor::visitDns(dhcpcdParser::DnsContext * context)
    {
        auto any_server_list = visit(context->server_list());
        auto server_list = std::any_cast<std::vector<std::string>>(any_server_list);
        auto & config = m_ethernet_stack.top();
        for (auto & server : server_list)
        {
            config.interface.add_nameserver_address(IPAddress::address_from_string(server));
        }
        return {};
    }

    std::any ServicesdDhcpcdVisitor::visitMultipleServer(dhcpcdParser::MultipleServerContext * context)
    {
        auto any_server_list = visit(context->server_list());
        auto server_list = std::any_cast<std::vector<std::string>>(any_server_list);
        auto text = context->TEXT()->getText();
        server_list.emplace_back(text);
        return {server_list};
    }

    std::any ServicesdDhcpcdVisitor::visitSimpleServer(dhcpcdParser::SimpleServerContext * context)
    {
        std::vector<std::string> server_list{};
        auto text = context->TEXT()->getText();
        server_list.emplace_back(text);
        return {server_list};
    }

    std::any ServicesdDhcpcdVisitor::visitControlGroupDecl(dhcpcdParser::ControlGroupDeclContext * context)
    {
        auto text = context->TEXT()->getText();
        return {text};
    }

    std::any ServicesdDhcpcdVisitor::visitOptionDecl(dhcpcdParser::OptionDeclContext * context)
    {
        std::any result;
        auto option_values = context->optionValue();
        for (auto & value : option_values)
        {
            result = visit(value);
        }
        return result;
    }

    std::any ServicesdDhcpcdVisitor::visitOptionText(dhcpcdParser::OptionTextContext * context)
    {
        auto text = context->TEXT()->getText();
        if (text == "rapid_commit")
        {
            m_configuration.rapid_commit = true;
        }
        else if (text == "domain_name")
        {
            m_configuration.domain_name = true;
        }
        else if (text == "domain_search")
        {
            m_configuration.domain_search = true;
        }
        else if (text == "classless_static_routes")
        {
            m_configuration.classless_static_routes = true;
        }
        else if (text == "interface_mtu")
        {
            m_configuration.interface_mtu = true;
        }
        else if (text == "host_name")
        {
            m_configuration.host_name = true;
        }
        return {};
    }

    std::any ServicesdDhcpcdVisitor::visitOptionDns(dhcpcdParser::OptionDnsContext * context)
    {
        (void)context;
        m_configuration.domain_name_servers = true;
        return {};
    }

    std::any ServicesdDhcpcdVisitor::visitRequireDecl(dhcpcdParser::RequireDeclContext * context)
    {
        auto text = context->TEXT()->getText();
        if (text == "dhcp_server_identifier")
        {
            m_configuration.dhcp_server_identifier = true;
        }
        return {};
    }

    std::any ServicesdDhcpcdVisitor::visitSlaacHwaddr(dhcpcdParser::SlaacHwaddrContext * context)
    {
        (void)context;
        m_configuration.slaac_hwaddr = true;
        return {};
    }

    std::any ServicesdDhcpcdVisitor::visitSlaacPrivate(dhcpcdParser::SlaacPrivateContext * context)
    {
        (void)context;
        m_configuration.slaac_private = true;
        return {};
    }

    std::any ServicesdDhcpcdVisitor::visitHostname(dhcpcdParser::HostnameContext * context)
    {
        (void)context;
        m_configuration.hostname = true;
        return {};
    }

    std::any ServicesdDhcpcdVisitor::visitClientid(dhcpcdParser::ClientidContext * context)
    {
        (void)context;
        m_configuration.clientid = true;
        return {};
    }

    std::any ServicesdDhcpcdVisitor::visitDuid(dhcpcdParser::DuidContext * context)
    {
        (void)context;
        m_configuration.duid = true;
        return {};
    }

    std::any ServicesdDhcpcdVisitor::visitPersistent(dhcpcdParser::PersistentContext * context)
    {
        (void)context;
        m_configuration.persistent = true;
        return {};
    }

    auto ServicesdDhcpcdVisitor::add_address_and_netmask(const string_type & s) -> std::any
    {
        IPAddressAndNetmask addr_mask{s};
        auto & config = m_ethernet_stack.top();
        config.interface.add_address_and_netmask(addr_mask);
        return {};
    }

    auto ServicesdDhcpcdVisitor::get_configuration() -> DHCPCD
    {
        while (! m_ethernet_stack.empty())
        {
            auto & config = m_ethernet_stack.top();
            m_configuration.configurations.emplace_back(config);
            m_ethernet_stack.pop();
        }
        return m_configuration;
    }
} // namespace TF::Linux
