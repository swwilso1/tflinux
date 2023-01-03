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

#ifndef TFSERVICECONFIGURATION_HPP
#define TFSERVICECONFIGURATION_HPP

#include <vector>
#include "TFFoundation.hpp"
#include "tfnetworkconfiguration.hpp"

namespace TF::Linux
{

    class ServiceConfiguration
    {
    public:
        using string_type = String;
        using ethernet_configuration_list = std::vector<EthernetConfiguration>;
        using wireless_configuration_list = std::vector<WirelessConfiguration>;

        ServiceConfiguration() = default;
        virtual ~ServiceConfiguration() = default;

        virtual auto load_configurations_from_file(const string_type & file)
            -> std::pair<wireless_configuration_list, ethernet_configuration_list> = 0;

        virtual void write_configurations_to_file(const wireless_configuration_list & wireless_list,
                                                  const ethernet_configuration_list & ethernet_list,
                                                  const string_type & file) = 0;
    };

} // namespace TF::Linux

#endif // TFSERVICECONFIGURATION_HPP
