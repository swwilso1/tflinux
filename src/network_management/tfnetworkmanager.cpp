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
#include "tfdnsmasqservice.hpp"
#include "tfhostapdservice.hpp"
#include "tfdhcpcdservice.hpp"

namespace TF::Linux
{

    auto NetworkManager::is_wifi_enabled() const -> bool
    {
        for (auto & pair : m_configuration_map)
        {
            if (pair.second.enabled && pair.second.wifi_interface)
            {
                return true;
            }
        }
        return false;
    }

    auto NetworkManager::is_ethernet_enabled() const -> bool
    {
        for (auto & pair : m_configuration_map)
        {
            if (pair.second.enabled && (! pair.second.wifi_interface))
            {
                return true;
            }
        }
        return false;
    }

    auto NetworkManager::has_configuration_for_name(const string_type & name) const -> bool
    {
        return m_configuration_map.contains(name);
    }

    auto NetworkManager::get_number_of_wifi_configurations() const -> size_type
    {
        size_type count{0};
        std::for_each(m_configuration_map.cbegin(), m_configuration_map.cend(),
                      [&count](const std::pair<string_type, NetworkConfiguration> & pair) {
                          if (pair.second.wifi_interface)
                          {
                              ++count;
                          }
                      });
        return count;
    }

    auto NetworkManager::get_number_of_ethernet_configurations() const -> size_type
    {
        size_type count{0};
        std::for_each(m_configuration_map.cbegin(), m_configuration_map.cend(),
                      [&count](const std::pair<string_type, NetworkConfiguration> & pair) {
                          if (! pair.second.wifi_interface)
                          {
                              ++count;
                          }
                      });
        return count;
    }

    auto NetworkManager::get_wifi_configuration_names() const -> std::vector<string_type>
    {
        std::vector<string_type> names{};
        std::for_each(m_configuration_map.cbegin(), m_configuration_map.cend(),
                      [&names](const std::pair<const string_type, NetworkConfiguration> & pair) -> void {
                          if (pair.second.wifi_interface)
                          {
                              names.emplace_back(pair.first);
                          }
                      });
        return names;
    }

    auto NetworkManager::get_ethernet_configuration_names() const -> std::vector<string_type>
    {
        std::vector<string_type> names{};
        std::for_each(m_configuration_map.cbegin(), m_configuration_map.cend(),
                      [&names](const std::pair<const string_type, NetworkConfiguration> & pair) -> void {
                          if (! pair.second.wifi_interface)
                          {
                              names.emplace_back(pair.first);
                          }
                      });
        return names;
    }

    auto NetworkManager::get_primary_wifi_configuration_name() const -> std::optional<string_type>
    {
        for (auto & pair : m_configuration_map)
        {
            if (pair.second.wifi_interface && pair.second.interface.has_ipv4_address())
            {
                return {pair.second.interface.get_name()};
            }
        }
        return {};
    }

    auto NetworkManager::get_primary_ethernet_configuration_name() const -> std::optional<string_type>
    {
        for (auto & pair : m_configuration_map)
        {
            if ((! pair.second.wifi_interface) && pair.second.interface.has_ipv4_address() &&
                (! pair.second.interface.is_loopback_interface()))
            {
                return {pair.second.interface.get_name()};
            }
        }
        return {};
    }

    auto NetworkManager::get_configuration_for_name(const string_type & name) -> std::optional<NetworkConfiguration>
    {
        if (! m_configuration_map.contains(name))
        {
            return {};
        }
        return {m_configuration_map[name]};
    }

    void NetworkManager::set_configuration_for_name(const string_type & name, const NetworkConfiguration & config)
    {
        m_configuration_map.insert(std::make_pair(name, config));
    }

    void NetworkManager::remove_configuration_for_name(const string_type & name)
    {
        m_configuration_map.erase(name);
    }

    void NetworkManager::clear()
    {
        m_configuration_map.clear();
    }

    void NetworkManager::load_settings_from_system()
    {
        FileManager manager{};

        NetworkService::network_configuration_map config_map{};

        // First we load the network configurations currently present in the system.
        NetworkInterfaces network_interfaces{};
        network_interfaces.load_interfaces();

        // Load the network interface basics into the Wi-Fi and ethernet tables.
        for (auto & network_interface : network_interfaces.get_interfaces())
        {
            auto config = std::make_shared<NetworkConfiguration>();
            config->interface = SystemNetworkInterface{network_interface};
            if (config->interface.get_number_of_addresses() > 0)
            {
                config->enabled = true;
            }

            if (config->interface.is_wireless_interface())
            {
                config->wifi_interface = true;
            }

            config_map.insert(std::make_pair(config->interface.get_name(), config));
        }

        PlatformId platform_id{};

        // Now for platform specific configurations we want to go through the subsystems and
        // load each subsystem configuration.
        if (platform_id.get_vendor() == "Ubuntu" &&
            platform_id.get_processor_architecture() == PlatformId::ProcessorArchitecture::X86_64)
        {
            // We are running on an Intel x64 Ubuntu box.  Assume that we have access to the netplan service.
            NetplanService netplan_service{};

            // For the netplan configurations, we need to list the files in /etc/netplan that end in .yaml and
            // load the configuration information for each .yaml file.
            string_type netplan_directory{"/etc/netplan"};
            auto files = manager.contentsOfDirectoryAtPath("/etc/netplan");
            for (auto & file : files)
            {
                auto full_file_name = netplan_directory + FileManager::pathSeparator + file;
                if (manager.fileExistsAtPath(full_file_name) && manager.extensionOfItemAtPath(full_file_name) == "yaml")
                {
                    netplan_service.load_configurations_from_file(full_file_name, config_map);
                }
            }
        }
        else if (platform_id.get_vendor() == "Debian" &&
                 ((platform_id.get_processor_architecture() == PlatformId::ProcessorArchitecture::ARM64) ||
                  (platform_id.get_processor_architecture() == PlatformId::ProcessorArchitecture::ARM32)))
        {
            // We are running on a Debian ARM box, probably a Raspberry Pi.  Assume that we have access to dhcpcd.
            DHCPCDService dhcpcd_service{};

            string_type dhcpcd_conf_file{"/etc/dhcpcd.conf"};
            if (manager.fileExistsAtPath(dhcpcd_conf_file))
            {
                dhcpcd_service.load_configurations_from_file(dhcpcd_conf_file, config_map);
            }
        }

        DNSMasqService dnsmasq_service{};
        HostapdService hostapd_service{};

        string_type dnsmasq_conf_file{"/etc/dnsmasq.conf"};
        if (manager.fileExistsAtPath(dnsmasq_conf_file))
        {
            dnsmasq_service.load_configurations_from_file(dnsmasq_conf_file, config_map);
        }

        string_type hostapd_conf_file{"/etc/hostapd/hostapd.conf"};
        if (manager.fileExistsAtPath(hostapd_conf_file))
        {
            hostapd_service.load_configurations_from_file(hostapd_conf_file, config_map);
        }

        for (auto & pair : config_map)
        {
            m_configuration_map.insert(std::make_pair(pair.first, *pair.second));
        }
    }

    void NetworkManager::update_system_from_settings()
    {
        FileManager manager{};
        PlatformId platform_id{};

        NetworkService::network_configuration_map config_map{};

        for (auto & pair : m_configuration_map)
        {
            auto config = std::make_shared<NetworkConfiguration>();
            *config = pair.second;
            config_map.insert(std::make_pair(pair.first, config));
        }

        DNSMasqService dnsmasq_service{};
        string_type dnsmasq_conf_file{"/etc/dnsmasq.conf"};
        dnsmasq_service.write_configurations_to_file(config_map, dnsmasq_conf_file);

        HostapdService hostapd_service{};
        string_type hostapd_conf_file{"/etc/hostapd/hostapd.conf"};
        hostapd_service.write_configurations_to_file(config_map, hostapd_conf_file);

        if (platform_id.get_vendor() == "Ubuntu" &&
            platform_id.get_processor_architecture() == PlatformId::ProcessorArchitecture::X86_64)
        {
            // We are running on an Intel x64 Ubuntu box.  Assume that we have access to the netplan service.
            NetplanService netplan_service{};
            string_type netplan_conf_file{"/etc/netplan/99-tflinux-yaml"};
            netplan_service.write_configurations_to_file(config_map, netplan_conf_file);

            Process netplan{"netplan apply"};
            netplan.launch();
            netplan.wait();
        }
        else if (platform_id.get_vendor() == "Debian" &&
                 ((platform_id.get_processor_architecture() == PlatformId::ProcessorArchitecture::ARM64) ||
                  (platform_id.get_processor_architecture() == PlatformId::ProcessorArchitecture::ARM32)))
        {
            // We are running on a Debian ARM box, probably a Raspberry Pi.  Assume that we have access to dhcpcd.
            DHCPCDService dhcpcd_service{};
            string_type dhcpcd_conf_file{"/etc/dhcpcd.conf"};
            dhcpcd_service.write_configurations_to_file(config_map, dhcpcd_conf_file);

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

} // namespace TF::Linux
