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

#ifndef TFNETWORKMANAGER_HPP
#define TFNETWORKMANAGER_HPP

#include <tuple>
#include <vector>
#include <unordered_map>
#include <functional>
#include <optional>
#include "TFFoundation.hpp"
#include "tfnetworkconfiguration.hpp"

using namespace TF::Foundation;

namespace TF::Linux
{

    class NetworkManager
    {
    public:
        using string_type = String;
        using size_type = size_t;

#pragma mark - methods to check for network properties.

        [[nodiscard]] auto is_wifi_enabled() const -> bool;

        [[nodiscard]] auto is_ethernet_enabled() const -> bool;

        [[nodiscard]] auto has_wifi_configuration_for_name(const string_type & name) const -> bool;

        [[nodiscard]] auto has_ethernet_configuration_for_name(const string_type & name) const -> bool;

#pragma mark - methods to interact with OS

        void load_settings_from_system();

        void update_system_from_settings();

#pragma mark - methods to get interface information

        [[nodiscard]] auto get_number_of_wifi_configurations() const -> size_type;

        [[nodiscard]] auto get_number_of_ethernet_configurations() const -> size_type;

        [[nodiscard]] auto get_wifi_configuration_names() const -> std::vector<string_type>;

        [[nodiscard]] auto get_ethernet_configuration_names() const -> std::vector<string_type>;

        [[nodiscard]] auto get_primary_wifi_configuration_name() const -> std::optional<string_type>;

        [[nodiscard]] auto get_primary_ethernet_configuration_name() const -> std::optional<string_type>;

        [[nodiscard]] auto get_wifi_configuration_for_name(const string_type & name)
            -> std::optional<WirelessConfiguration>;

        [[nodiscard]] auto get_ethernet_configuration_for_name(const string_type & name)
            -> std::optional<EthernetConfiguration>;

#pragma mark - methods to set interface information

        void set_wifi_configuration_for_name(const string_type & name, const WirelessConfiguration & config);

        void set_ethernet_configuration_for_name(const string_type & name, const EthernetConfiguration & config);

        void remove_wifi_configuration_for_name(const string_type & name);

        void remove_ethernet_configuration_for_name(const string_type & name);

        void remove_wifi_configurations();

        void remove_ethernet_configurations();

    private:
#pragma mark - class private details

        using ethernet_configuration_map = std::unordered_map<string_type, EthernetConfiguration>;
        using wifi_configuration_map = std::unordered_map<string_type, WirelessConfiguration>;

        wifi_configuration_map m_wifi_configurations{};
        ethernet_configuration_map m_ethernet_configurations{};

        void update_wireless_configs_from_list(const std::vector<WirelessConfiguration> & configs);
        void update_ethernet_configs_from_list(const std::vector<EthernetConfiguration> & configs);
    };

} // namespace TF::Linux

#endif // TFNETWORKMANAGER_HPP
