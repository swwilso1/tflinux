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

#ifndef TFNETWORKSERVICE_HPP
#define TFNETWORKSERVICE_HPP

#include <vector>
#include <unordered_map>
#include <memory>
#include "TFFoundation.hpp"
#include "tfnetworkconfiguration.hpp"

namespace TF::Linux
{

    class NetworkService
    {
    public:
        using string_type = String;
        using network_configuration_map = std::unordered_map<string_type, std::shared_ptr<NetworkConfiguration>>;

        NetworkService() = default;
        virtual ~NetworkService() = default;

        virtual void load_configurations_from_file(const string_type & file,
                                                   network_configuration_map & configurations) = 0;

        virtual void write_configurations_to_file(const network_configuration_map & configurations,
                                                  const string_type & file) = 0;
    };

} // namespace TF::Linux

#endif // TFNETWORKSERVICE_HPP
