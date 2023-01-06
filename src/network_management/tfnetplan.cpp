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

#include <fstream>
#include <string>
#include <unordered_map>
#include "tfnetplan.hpp"
#include "tfyamlinterface.hpp"

namespace TF::Linux
{

    void NetplanService::load_configurations_from_file(const string_type & file,
                                                       network_configuration_map & configurations)
    {
        auto file_cstr = file.cStr();
        YAML::Node config_file{};

        try
        {
            config_file = YAML::LoadFile(file_cstr.get());
        }
        catch (YAML::BadFile & e)
        {
            // The YAML file did not load, we cannot do anything else with this file.
            return;
        }

        if (config_file["network"])
        {
            auto network = config_file["network"];

            if (network["ethernets"] && network["ethernets"].IsMap())
            {
                auto ethernets = network["ethernets"];
                for (YAML::const_iterator yaml_iter = ethernets.begin(); yaml_iter != ethernets.end(); yaml_iter++)
                {
                    auto interface_name = yaml_iter->first.as<std::string>();
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

                    configuration->interface.set_name(interface_name);

                    auto config_details = yaml_iter->second;
                    if (config_details.IsMap())
                    {
                        for (YAML::const_iterator inner_map_iter = config_details.begin();
                             inner_map_iter != config_details.end(); inner_map_iter++)
                        {
                            auto key = inner_map_iter->first.as<std::string>();
                            if (key == "dhcp4")
                            {
                                auto value = inner_map_iter->second.as<std::string>();
                                if (value == "true")
                                {
                                    configuration->addr_mode = NetworkConfiguration::address_mode::DHCP;
                                }
                            }
                            else if (key == "addresses" && inner_map_iter->second.IsSequence())
                            {
                                // The value of addresses should be a sequence of ip addresses with CIDR netmasks.
                                for (YAML::const_iterator address_iter = inner_map_iter->second.begin();
                                     address_iter != inner_map_iter->second.end(); address_iter++)
                                {
                                    auto address_value = address_iter->as<std::string>();
                                    configuration->interface.add_address_and_netmask(
                                        IPAddressAndNetmask{address_value});
                                }
                                configuration->addr_mode = NetworkConfiguration::address_mode::STATIC;
                            }
                            else if (key == "nameservers" && inner_map_iter->second.IsMap())
                            {
                                auto nameservers = inner_map_iter->second;
                                if (nameservers["addresses"])
                                {
                                    auto nameserver_addresses = nameservers["addresses"];
                                    for (size_t i = 0; i < nameserver_addresses.size(); i++)
                                    {
                                        auto nameserver_address = nameserver_addresses[i].as<std::string>();
                                        configuration->interface.add_nameserver_address(
                                            IPAddress::address_from_string(nameserver_address));
                                    }
                                }
                            }
                        }
                    }

                    configuration->enabled = true;
                }
            }

            if (network["wifis"] && network["wifis"].IsMap())
            {
                auto wifis = network["wifis"];
                for (YAML::const_iterator wifi_iter = wifis.begin(); wifi_iter != wifis.end(); wifi_iter++)
                {
                    if (wifi_iter->second.IsMap())
                    {
                        auto interface_name = wifi_iter->first.as<std::string>();
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

                        configuration->interface.set_name(interface_name);
                        auto wifis_map = wifi_iter->second;

                        for (YAML::const_iterator wifi_map_iter = wifis_map.begin(); wifi_map_iter != wifis_map.end();
                             wifi_map_iter++)
                        {
                            auto key = wifi_map_iter->first.as<std::string>();
                            if (key == "dhcp4")
                            {
                                auto value = wifi_map_iter->second.as<std::string>();
                                if (value == "true")
                                {
                                    configuration->addr_mode = NetworkConfiguration::address_mode::DHCP;
                                }
                            }
                            else if (key == "access-points" && wifi_map_iter->second.IsMap())
                            {
                                auto access_points_map = wifi_map_iter->second;
                                for (YAML::const_iterator access_points_iter = access_points_map.begin();
                                     access_points_iter != access_points_map.end(); access_points_iter++)
                                {
                                    if (access_points_iter->second.IsMap())
                                    {
                                        auto access_point_name = access_points_iter->first.as<std::string>();
                                        configuration->ssid = access_point_name;
                                        if (access_points_iter->second["password"])
                                        {
                                            configuration->password =
                                                access_points_iter->second["password"].as<std::string>();
                                        }
                                    }
                                }
                            }
                        }

                        configuration->enabled = true;
                    }
                }
            }
        }
    }

    void NetplanService::write_configurations_to_file(const network_configuration_map & configurations,
                                                      const string_type & file)
    {
        YAML::Emitter yaml_stream{};

        auto write_interface_for_ethernets =
            [&yaml_stream](const std::shared_ptr<NetworkConfiguration> & config) -> void {
            yaml_stream << YAML::Key << config->interface.get_name();
            yaml_stream << YAML::Value;
            yaml_stream << YAML::BeginMap;
            if (config->addr_mode == NetworkConfiguration::address_mode::DHCP)
            {
                yaml_stream << YAML::Key << "dhcp4";
                yaml_stream << YAML::Value << true;
            }
            else
            {
                auto addresses_and_masks = config->interface.get_addresses_and_netmasks();
                yaml_stream << YAML::Key << "addresses";
                yaml_stream << YAML::Value;
                yaml_stream << YAML::Flow;
                yaml_stream << YAML::BeginSeq;
                std::for_each(addresses_and_masks.begin(), addresses_and_masks.end(),
                              [&yaml_stream](IPAddressAndNetmask & addr_mask) -> void {
                                  // Do not write private subnet addrs for IPv6 addresses.  We may not even need to
                                  // do anything with global IPv6, but add it for now.
                                  if (addr_mask.address.is_ipv6_address() && (! addr_mask.address.is_global_address()))
                                  {
                                      return;
                                  }
                                  yaml_stream << addr_mask.get_as_cidr_notation();
                              });
                yaml_stream << YAML::EndSeq;

                auto nameservers = config->interface.get_nameservers();
                if (! nameservers.empty())
                {
                    yaml_stream << YAML::Key << "nameservers";
                    yaml_stream << YAML::Value;
                    yaml_stream << YAML::BeginMap;
                    yaml_stream << YAML::Key << "addresses";
                    yaml_stream << YAML::Value;
                    yaml_stream << YAML::Flow;
                    yaml_stream << YAML::BeginSeq;
                    std::for_each(nameservers.begin(), nameservers.end(), [&yaml_stream](IPAddress & address) -> void {
                        auto presentation_name = address.get_presentation_name();
                        yaml_stream << *presentation_name;
                    });
                    yaml_stream << YAML::EndSeq;
                    yaml_stream << YAML::EndMap;
                }
            }
            yaml_stream << YAML::Key << "optional";
            yaml_stream << YAML::Value << true;
            yaml_stream << YAML::EndMap;
        };

        auto should_use_config_for_ethernets = [](const std::shared_ptr<NetworkConfiguration> & config) -> bool {
            return (config->enabled && ! config->wifi_interface) ||
                   (config->enabled && config->wifi_interface &&
                    ((config->mode == NetworkConfiguration::wifi_mode::ACCESS_POINT) ||
                     (config->mode == NetworkConfiguration::wifi_mode::CLIENT &&
                      config->addr_mode == NetworkConfiguration::address_mode::STATIC)));
        };

        auto needs_ethernets_section = false;
        auto needs_wifis_section = false;

        // Check for configurations that should go in the ethernets section.
        for (auto & pair : configurations)
        {
            if (should_use_config_for_ethernets(pair.second))
            {
                needs_ethernets_section = true;
                break;
            }
        }

        // Now check the wireless configurations to see if any need the wifis section.
        for (auto & pair : configurations)
        {
            if (pair.second->enabled && pair.second->mode == NetworkConfiguration::wifi_mode::CLIENT)
            {
                needs_wifis_section = true;
                break;
            }
        }

        yaml_stream << YAML::BeginMap;
        yaml_stream << YAML::Key << "network";
        yaml_stream << YAML::Value;
        yaml_stream << YAML::BeginMap;
        yaml_stream << YAML::Key << "version";
        yaml_stream << YAML::Value << 2;
        yaml_stream << YAML::Key << "renderer";
        yaml_stream << YAML::Value << "networkd";

        // Now process the content that will go in the "ethernets" section.
        if (needs_ethernets_section)
        {
            yaml_stream << YAML::Key << "ethernets";
            yaml_stream << YAML::Value;
            yaml_stream << YAML::BeginMap;

            // Run through the ethernet config list.
            std::for_each(configurations.cbegin(), configurations.cend(),
                          [&should_use_config_for_ethernets, &write_interface_for_ethernets](
                              const std::pair<string_type, std::shared_ptr<NetworkConfiguration>> & pair) -> void {
                              if (should_use_config_for_ethernets(pair.second))
                              {
                                  write_interface_for_ethernets(pair.second);
                              }
                          });

            // Run through the wireless config list.
            std::for_each(configurations.cbegin(), configurations.cend(),
                          [&write_interface_for_ethernets, &should_use_config_for_ethernets](
                              const std::pair<string_type, std::shared_ptr<NetworkConfiguration>> & pair) -> void {
                              if (should_use_config_for_ethernets(pair.second))
                              {
                                  write_interface_for_ethernets(pair.second);
                              }
                          });
            yaml_stream << YAML::EndMap;
        }

        // Now process any info that should go in the "wifis" section
        if (needs_wifis_section)
        {
            yaml_stream << YAML::Key << "wifis";
            yaml_stream << YAML::Value;

            yaml_stream << YAML::BeginMap;
            std::for_each(
                configurations.cbegin(), configurations.cend(),
                [&yaml_stream](const std::pair<string_type, std::shared_ptr<NetworkConfiguration>> & pair) -> void {
                    if (pair.second->enabled && pair.second->mode == NetworkConfiguration::wifi_mode::CLIENT)
                    {
                        yaml_stream << YAML::Key << pair.second->interface.get_name();
                        yaml_stream << YAML::Value;
                        yaml_stream << YAML::BeginMap;
                        yaml_stream << YAML::Key << "optional";
                        yaml_stream << YAML::Value << true;

                        if (pair.second->addr_mode == NetworkConfiguration::InterfaceAddressMode::DHCP)
                        {
                            yaml_stream << YAML::Key << "dhcp4";
                            yaml_stream << YAML::Value << true;
                        }

                        yaml_stream << YAML::Key << "access-points";
                        yaml_stream << YAML::Value;
                        yaml_stream << YAML::BeginMap;
                        yaml_stream << YAML::Key << YAML::DoubleQuoted << pair.second->ssid;
                        yaml_stream << YAML::Value;
                        yaml_stream << YAML::BeginMap;
                        yaml_stream << YAML::Key << "password";
                        yaml_stream << YAML::Value << YAML::DoubleQuoted << pair.second->password;
                        yaml_stream << YAML::EndMap;
                        yaml_stream << YAML::EndMap;
                        yaml_stream << YAML::EndMap;
                    }
                });
            yaml_stream << YAML::EndMap;
        }

        yaml_stream << YAML::EndMap;
        yaml_stream << YAML::EndMap;

        String yaml_file_contents{yaml_stream.c_str()};
        FileHandle write_handle = FileHandle::fileHandleForWritingAtPath(file, true);
        write_handle.writeString(yaml_file_contents);
    }

} // namespace TF::Linux
