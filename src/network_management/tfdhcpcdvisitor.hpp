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

#ifndef TFDHCPCDVISITOR_HPP
#define TFDHCPCDVISITOR_HPP

#include <stack>
#include <vector>
#include "TFFoundation.hpp"
#include "dhcpcdVisitor.h"
#include "tfnetworkconfiguration.hpp"
#include "tfdhcpcd.hpp"

using namespace TF::Foundation;
using namespace TF::Linux::dhcpcd;

namespace TF::Linux
{

    class ServicesdDhcpcdVisitor : public dhcpcdVisitor
    {
    public:
        ServicesdDhcpcdVisitor() : dhcpcdVisitor() {}

        std::any visitDeclHelper1(dhcpcdParser::DeclHelper1Context * context) override;

        std::any visitDeclHelper2(dhcpcdParser::DeclHelper2Context * context) override;

        std::any visitCommentHelper(dhcpcdParser::CommentHelperContext * context) override;

        std::any visitOtherHelper(dhcpcdParser::OtherHelperContext * context) override;

        std::any visitOptionHelper(dhcpcdParser::OptionHelperContext * context) override;

        std::any visitRequireHelper(dhcpcdParser::RequireHelperContext * context) override;

        std::any visitSlaacHelper(dhcpcdParser::SlaacHelperContext * context) override;

        std::any visitInterfaceHelper(dhcpcdParser::InterfaceHelperContext * context) override;

        std::any visitControlHelper(dhcpcdParser::ControlHelperContext * context) override;

        std::any visitOneLineComment(dhcpcdParser::OneLineCommentContext * context) override;

        std::any visitMultiLineComment(dhcpcdParser::MultiLineCommentContext * context) override;

        std::any visitInterfaceDetail(dhcpcdParser::InterfaceDetailContext * context) override;

        std::any visitStaticHelper(dhcpcdParser::StaticHelperContext * context) override;

        std::any visitIpv4Address(dhcpcdParser::Ipv4AddressContext * context) override;

        std::any visitIpv6Address(dhcpcdParser::Ipv6AddressContext * context) override;

        std::any visitRouters(dhcpcdParser::RoutersContext * context) override;

        std::any visitDns(dhcpcdParser::DnsContext * context) override;

        std::any visitMultipleServer(dhcpcdParser::MultipleServerContext * context) override;

        std::any visitSimpleServer(dhcpcdParser::SimpleServerContext * context) override;

        std::any visitControlGroupDecl(dhcpcdParser::ControlGroupDeclContext * context) override;

        std::any visitOptionDecl(dhcpcdParser::OptionDeclContext * context) override;

        std::any visitOptionText(dhcpcdParser::OptionTextContext * context) override;

        std::any visitOptionDns(dhcpcdParser::OptionDnsContext * context) override;

        std::any visitRequireDecl(dhcpcdParser::RequireDeclContext * context) override;

        std::any visitSlaacHwaddr(dhcpcdParser::SlaacHwaddrContext * context) override;

        std::any visitSlaacPrivate(dhcpcdParser::SlaacPrivateContext * context) override;

        std::any visitHostname(dhcpcdParser::HostnameContext * context) override;

        std::any visitClientid(dhcpcdParser::ClientidContext * context) override;

        std::any visitDuid(dhcpcdParser::DuidContext * context) override;

        std::any visitPersistent(dhcpcdParser::PersistentContext * context) override;

        auto get_configuration() -> DHCPCD;

    private:
        using string_type = String;

        DHCPCD m_configuration{};
        std::stack<EthernetConfiguration> m_ethernet_stack{};

        auto add_address_and_netmask(const string_type & s) -> std::any;
    };
} // namespace TF::Linux

#endif // TFDHCPCDVISITOR_HPP
