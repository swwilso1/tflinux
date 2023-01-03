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

#include "tfnetworkmanager.hpp"
#include "tfnetplan.hpp"
#include "tfdnsmasqconfiguration.hpp"
#include "tfhostapdconfiguration.hpp"
#include "tfdhcpcdconfiguration.hpp"

namespace TF::Linux
{

    auto NetworkManager::is_wifi_enabled() const -> bool
    {
        for (auto & pair : m_wifi_configurations)
        {
            if (pair.second.enabled)
            {
                return true;
            }
        }
        return false;
    }

    auto NetworkManager::is_ethernet_enabled() const -> bool
    {
        for (auto & pair : m_ethernet_configurations)
        {
            if (pair.second.enabled)
            {
                return true;
            }
        }
        return false;
    }

    auto NetworkManager::has_wifi_configuration_for_name(const string_type & name) const -> bool
    {
        return m_wifi_configurations.contains(name);
    }

    auto NetworkManager::has_ethernet_configuration_for_name(const string_type & name) const -> bool
    {
        return m_ethernet_configurations.contains(name);
    }

    auto NetworkManager::get_number_of_wifi_configurations() const -> size_type
    {
        return m_wifi_configurations.size();
    }

    auto NetworkManager::get_number_of_ethernet_configurations() const -> size_type
    {
        return m_ethernet_configurations.size();
    }

    auto NetworkManager::get_wifi_configuration_names() const -> std::vector<string_type>
    {
        std::vector<string_type> names{};
        std::for_each(m_wifi_configurations.cbegin(), m_wifi_configurations.cend(),
                      [&names](const std::pair<const string_type, WirelessConfiguration> & pair) -> void {
                          names.emplace_back(pair.first);
                      });
        return names;
    }

    auto NetworkManager::get_ethernet_configuration_names() const -> std::vector<string_type>
    {
        std::vector<string_type> names{};
        std::for_each(m_ethernet_configurations.cbegin(), m_ethernet_configurations.cend(),
                      [&names](const std::pair<const string_type, EthernetConfiguration> & pair) -> void {
                          names.emplace_back(pair.first);
                      });
        return names;
    }

    auto NetworkManager::get_primary_wifi_configuration_name() const -> string_type
    {
        for (auto & pair : m_wifi_configurations)
        {
            if (pair.second.interface.has_ipv4_address())
            {
                return pair.second.interface.get_name();
            }
        }

        throw std::runtime_error{"Unable to find suitable wifi configuration"};
    }

    auto NetworkManager::get_primary_ethernet_configuration_name() const -> string_type
    {
        for (auto & pair : m_ethernet_configurations)
        {
            if (pair.second.interface.has_ipv4_address())
            {
                return pair.second.interface.get_name();
            }
        }

        throw std::runtime_error{"Unable to find suitable ethernet configuration"};
    }

    auto NetworkManager::get_wifi_configuration_for_name(const string_type & name) -> WirelessConfiguration
    {
        if (! m_wifi_configurations.contains(name))
        {
            throw std::invalid_argument{"No configuration for the given name"};
        }
        return m_wifi_configurations[name];
    }

    auto NetworkManager::get_ethernet_configuration_for_name(const string_type & name) -> EthernetConfiguration
    {
        if (! m_ethernet_configurations.contains(name))
        {
            throw std::invalid_argument{"No configuration for the given name"};
        }
        return m_ethernet_configurations[name];
    }

    void NetworkManager::set_wifi_configuration_for_name(const string_type & name, const WirelessConfiguration & config)
    {
        m_wifi_configurations[name] = config;
    }

    void NetworkManager::set_ethernet_configuration_for_name(const string_type & name,
                                                             const EthernetConfiguration & config)
    {
        m_ethernet_configurations[name] = config;
    }

    void NetworkManager::load_settings_from_system()
    {
        FileManager manager{};

        m_wifi_configurations.clear();
        m_ethernet_configurations.clear();

        // First we load the network interfaces currently present in the system.
        NetworkInterfaces network_interfaces{};
        network_interfaces.load_interfaces();

        // Load the network interface basics into the Wi-Fi and ethernet tables.
        for (auto & network_interface : network_interfaces.get_interfaces())
        {
            if (network_interface.is_wireless_interface())
            {
                WirelessConfiguration config{};
                if (network_interface.get_number_of_addresses() > 0)
                {
                    config.enabled = true;
                }
                config.interface = SystemNetworkInterface{network_interface};
                m_wifi_configurations[config.interface.get_name()] = config;
            }
            else
            {
                EthernetConfiguration config{};
                if (network_interface.get_number_of_addresses() > 0)
                {
                    config.enabled = true;
                }
                config.interface = SystemNetworkInterface{network_interface};
                m_ethernet_configurations[config.interface.get_name()] = config;
            }
        }

        PlatformId platform_id{};

        // Now for platform specific configurations we want to go through the subsystems and
        // load each subsystem configuration.
        if (platform_id.get_vendor() == "Ubuntu" &&
            platform_id.get_processor_architecture() == PlatformId::ProcessorArchitecture::X86_64)
        {
            // We are running on an Intel x64 Ubuntu box.  Assume that we have access to the netplan service.
            NetplanConfiguration netplan_configuration{};

            // For the netplan configurations, we need to list the files in /etc/netplan that end in .yaml and
            // load the configuration information for each .yaml file.
            string_type netplan_directory{"/etc/netplan"};
            auto files = manager.contentsOfDirectoryAtPath("/etc/netplan");
            for (auto & file : files)
            {
                auto full_file_name = netplan_directory + FileManager::pathSeparator + file;
                if (manager.fileExistsAtPath(full_file_name) && manager.extensionOfItemAtPath(full_file_name) == "yaml")
                {
                    auto netplan_configs = netplan_configuration.load_configurations_from_file(full_file_name);
                    update_wireless_configs_from_list(netplan_configs.first);
                    update_ethernet_configs_from_list(netplan_configs.second);
                }
            }
        }
        else if (platform_id.get_vendor() == "Debian" &&
                 ((platform_id.get_processor_architecture() == PlatformId::ProcessorArchitecture::ARM64) ||
                  (platform_id.get_processor_architecture() == PlatformId::ProcessorArchitecture::ARM32)))
        {
            // We are running on a Debian ARM box, probably a Raspberry Pi.  Assume that we have access to dhcpcd.
            DHCPCDConfiguration dhcpcd_configuration{};

            string_type dhcpcd_conf_file{"/etc/dhcpcd.conf"};
            if (manager.fileExistsAtPath(dhcpcd_conf_file))
            {
                // For now the dhcpcd code puts all interface details in the Ethernet configuration list.
                auto dhcpcd_configs = dhcpcd_configuration.load_configurations_from_file(dhcpcd_conf_file);
                for (auto & ethernet_config : dhcpcd_configs.second)
                {
                    // Because we can have wireless and ethernet interfaces in this config file
                    // we have to check to see if the interface is already in the wireless or ethernet list.
                    if (m_wifi_configurations.contains(ethernet_config.get_name()))
                    {
                        auto & config = m_wifi_configurations[ethernet_config.get_name()];

                        // Here we do a full update_from() because the interface details in the dhcpcd.conf file
                        // might contain nameservers and gateways.
                        config.update_from(ethernet_config);
                    }
                    else if (m_ethernet_configurations.contains(ethernet_config.get_name()))
                    {
                        auto & config = m_ethernet_configurations[ethernet_config.get_name()];

                        // Here we do a full update_from() because the interface details in the dhcpcd.conf file
                        // might contain nameservers and gateways.
                        config.update_from(ethernet_config);
                    }
                    else
                    {
                        // We did not already have a record of the interface, so treat it as an ethernet
                        // interface because we cannot distinguish between ethernet and Wi-Fi at this point.
                        m_ethernet_configurations[ethernet_config.get_name()] = ethernet_config;
                    }
                }
            }
        }

        DNSMasqConfiguration dnsmasq_configuration{};
        HostapdConfiguration hostapd_configuration{};

        string_type dnsmasq_conf_file{"/etc/dnsmasq.conf"};
        if (manager.fileExistsAtPath(dnsmasq_conf_file))
        {
            // DNSMasq only configures Wi-Fi settings.
            auto dnsmasq_configs = dnsmasq_configuration.load_configurations_from_file(dnsmasq_conf_file);
            update_wireless_configs_from_list(dnsmasq_configs.first);
        }

        string_type hostapd_conf_file{"/etc/hostapd/hostapd.conf"};
        if (manager.fileExistsAtPath(hostapd_conf_file))
        {
            auto hostapd_configs = hostapd_configuration.load_configurations_from_file(hostapd_conf_file);
            update_wireless_configs_from_list(hostapd_configs.first);
            update_ethernet_configs_from_list(hostapd_configs.second);
        }
    }

    void NetworkManager::update_system_from_settings()
    {
        FileManager manager{};
        PlatformId platform_id{};
        std::vector<WirelessConfiguration> wireless_list{};
        std::vector<EthernetConfiguration> ethernet_list{};

        // Put the configurations into wireless and ethernet lists.
        std::for_each(m_wifi_configurations.cbegin(), m_wifi_configurations.cend(),
                      [&wireless_list](const std::pair<const string_type, WirelessConfiguration> & pair) -> void {
                          wireless_list.emplace_back(pair.second);
                      });

        std::for_each(m_ethernet_configurations.cbegin(), m_ethernet_configurations.cend(),
                      [&ethernet_list](const std::pair<const string_type, EthernetConfiguration> & pair) -> void {
                          ethernet_list.emplace_back(pair.second);
                      });

        DNSMasqConfiguration dnsmasq_configuration{};
        string_type dnsmasq_conf_file{"/etc/dnsmasq.conf"};
        dnsmasq_configuration.write_configurations_to_file(wireless_list, ethernet_list, dnsmasq_conf_file);

        HostapdConfiguration hostapd_configuration{};
        string_type hostapd_conf_file{"/etc/hostapd/hostapd.conf"};
        hostapd_configuration.write_configurations_to_file(wireless_list, ethernet_list, hostapd_conf_file);

        if (platform_id.get_vendor() == "Ubuntu" &&
            platform_id.get_processor_architecture() == PlatformId::ProcessorArchitecture::X86_64)
        {
            // We are running on an Intel x64 Ubuntu box.  Assume that we have access to the netplan service.
            NetplanConfiguration netplan_configuration{};
            string_type netplan_conf_file{"/etc/netplan/99-tflinux-yaml"};
            netplan_configuration.write_configurations_to_file(wireless_list, ethernet_list, netplan_conf_file);

            Process netplan{"netplan apply"};
            netplan.launch();
            netplan.wait();
        }
        else if (platform_id.get_vendor() == "Debian" &&
                 ((platform_id.get_processor_architecture() == PlatformId::ProcessorArchitecture::ARM64) ||
                  (platform_id.get_processor_architecture() == PlatformId::ProcessorArchitecture::ARM32)))
        {
            // We are running on a Debian ARM box, probably a Raspberry Pi.  Assume that we have access to dhcpcd.
            DHCPCDConfiguration dhcpcd_configuration{};
            string_type dhcpcd_conf_file{"/etc/dhcpcd.conf"};
            dhcpcd_configuration.write_configurations_to_file(wireless_list, ethernet_list, dhcpcd_conf_file);

            Process dhcpcd{"systemctl restart dhcpcd"};
            dhcpcd.launch();
            dhcpcd.wait();
        }

        Process dnsmasq{"systemctl restart dnsmasq"};
        dnsmasq.launch();
        dnsmasq.wait();

        Process hostapd{"systemctl restart hostapd"};
        hostapd.launch();
        hostapd.wait();
    }

    void NetworkManager::update_wireless_configs_from_list(const std::vector<WirelessConfiguration> & configs)
    {
        for (auto & wifi_config : configs)
        {
            if (m_wifi_configurations.contains(wifi_config.get_name()))
            {
                auto & stored_wifi_config = m_wifi_configurations[wifi_config.get_name()];
                stored_wifi_config.update_all_but_interface_from(wifi_config);
            }
            else
            {
                m_wifi_configurations[wifi_config.get_name()] = wifi_config;
            }
        }
    }

    void NetworkManager::update_ethernet_configs_from_list(const std::vector<EthernetConfiguration> & configs)
    {
        for (auto & ethernet_config : configs)
        {
            if (m_ethernet_configurations.contains(ethernet_config.get_name()))
            {
                auto & stored_ethernet_config = m_ethernet_configurations[ethernet_config.get_name()];
                stored_ethernet_config.update_all_but_interface_from(ethernet_config);
            }
            else
            {
                m_ethernet_configurations[ethernet_config.get_name()] = ethernet_config;
            }
        }
    }

} // namespace TF::Linux
