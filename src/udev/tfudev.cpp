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

#include "tfconfigure.hpp"
#include "tfudev.hpp"
#include "tfexceptions.hpp"

using namespace TF::Linux;

namespace TF::Linux::Udev
{

    Context::Context() : m_context{nullptr}
    {
        m_context = udev_new();
        if (m_context == nullptr)
        {
            throw system_no_code_error("Udev_new failed");
        }
    }

    Context::~Context()
    {
        udev_unref(m_context);
    }

    void Context::retain()
    {
        udev_ref(m_context);
    }

    void Context::release()
    {
        udev_unref(m_context);
    }

    Device::Device(Device & d) : m_device{}
    {
        d.retain();
        m_device = d.m_device;
    }

    Device::Device(Device && d) : m_device{}
    {
        m_device = d.m_device;
        d.m_device = nullptr;
    }

    Device::Device(const context_type & ctx, const string_type & path) : m_device{nullptr}
    {
        auto path_cstring_value = path.cStr();
        m_device = udev_device_new_from_syspath(ctx.m_context, path_cstring_value.get());
        if (m_device == nullptr)
        {
            throw system_no_code_error{"device creation from syspath failed"};
        }
    }

    Device::Device(const context_type & ctx, char type, dev_t number) : m_device{nullptr}
    {
        m_device = udev_device_new_from_devnum(ctx.m_context, type, number);
        if (m_device == nullptr)
        {
            throw system_no_code_error{"device creation from device number failed"};
        }
    }

    Device::Device(const context_type & ctx, const string_type & subsystem, const string_type & sysname) :
        m_device{nullptr}
    {
        auto subsystem_cstring_value = subsystem.cStr();
        auto sysname_cstring_value = sysname.cStr();
        m_device = udev_device_new_from_subsystem_sysname(ctx.m_context, subsystem_cstring_value.get(),
                                                          sysname_cstring_value.get());
        if (m_device == nullptr)
        {
            throw system_no_code_error{"device creation from subsystem and sysname failed"};
        }
    }

    Device::Device(udev_device * device) : m_device{nullptr}
    {
        if (device == nullptr)
        {
            throw std::invalid_argument{"Device argument cannot be nullptr"};
        }
        m_device = device;
    }

    Device::~Device()
    {
        if (m_device)
        {
            udev_device_unref(m_device);
        }
        m_device = reinterpret_cast<decltype(m_device)>(0xdeadbeef);
    }

    Device & Device::operator=(Device & d)
    {
        if (this == &d)
        {
            return *this;
        }

        d.retain();
        release();
        m_device = d.m_device;
        return *this;
    }

    Device & Device::operator=(Device && d)
    {
        if (this == &d)
        {
            return *this;
        }

        m_device = d.m_device;
        d.m_device = nullptr;
        return *this;
    }

    void Device::retain()
    {
        udev_device_ref(m_device);
    }

    void Device::release()
    {
        udev_device_unref(m_device);
    }

    Device::string_map_type Device::get_devlinks() const
    {
        return get_values(udev_device_get_devlinks_list_entry);
    }

    Device::string_map_type Device::get_properties() const
    {
        return get_values(udev_device_get_properties_list_entry);
    }

    Device::string_map_type Device::get_tags() const
    {
        return get_values(udev_device_get_tags_list_entry);
    }

    Device::string_map_type Device::get_current_tags() const
    {
#if defined(HAVE_UDEV_DEVICE_GET_CURRENT_TAGS_LIST_ENTRY)
        return get_values(udev_device_get_current_tags_list_entry);
#else
        return {};
#endif
    }

    Device::string_map_type Device::get_system_attributes() const
    {
        return get_values(udev_device_get_sysattr_list_entry);
    }

    Device::string_type Device::get_property_value_for_key(const string_type & key)
    {
        auto key_cstring_value = key.cStr();
        if (! key_cstring_value)
        {
            throw std::invalid_argument{"key argument not valid"};
        }
        return get_value(udev_device_get_property_value, key_cstring_value.get());
    }

    Device::string_type Device::get_system_attribute_for_key(const string_type & key)
    {
        auto key_cstring_value = key.cStr();
        if (! key_cstring_value)
        {
            throw std::invalid_argument{"key argument not valid"};
        }
        return get_value(udev_device_get_sysattr_value, key_cstring_value.get());
    }

    void Device::set_system_attribute_value(const string_type & key, const string_type & value)
    {
        auto key_cstring_value = key.cStr();
        auto value_cstring_value = value.cStr();

        if (! key_cstring_value)
        {
            throw std::invalid_argument{"key argument not valid"};
        }

        if (! value_cstring_value)
        {
            throw std::invalid_argument{"value argument not valid"};
        }

        auto udev_api_result =
            udev_device_set_sysattr_value(m_device, key_cstring_value.get(), value_cstring_value.get());
        if (udev_api_result < 0)
        {
            throw system_no_code_error{"set system attribute failed"};
        }
    }

    Device::string_type Device::get_syspath() const
    {
        return get_property(udev_device_get_syspath);
    }

    Device::string_type Device::get_sysname() const
    {
        return get_property(udev_device_get_sysname);
    }

    Device::string_type Device::get_sysnum() const
    {
        return get_property(udev_device_get_sysnum);
    }

    Device::string_type Device::get_devpath() const
    {
        return get_property(udev_device_get_devpath);
    }

    Device::string_type Device::get_devnode() const
    {
        return get_property(udev_device_get_devnode);
    }

    Device::string_type Device::get_devtype() const
    {
        return get_property(udev_device_get_devtype);
    }

    Device::string_type Device::get_subsystem() const
    {
        return get_property(udev_device_get_subsystem);
    }

    Device::string_type Device::get_driver() const
    {
        return get_property(udev_device_get_driver);
    }

    Device::string_type Device::get_action() const
    {
        return get_property(udev_device_get_action);
    }

    dev_t Device::get_devnum() const
    {
        return udev_device_get_devnum(m_device);
    }

    std::optional<Device> Device::get_parent()
    {
        auto parent_device = udev_device_get_parent(m_device);
        if (parent_device)
        {
            udev_device_ref(parent_device);
            return std::optional<Device>{Device{parent_device}};
        }
        return std::optional<Device>{};
    }

    std::optional<Device> Device::get_parent_with_subsystem_dev_type(const string_type & subsystem,
                                                                     const string_type & devtype)
    {
        auto subsystem_cstring_value = subsystem.cStr();
        auto devtype_cstring_value = devtype.cStr();

        // udev_device_get_parent_with_subsystem_devtype will decrement the refcount of m_device if the
        // function succeeds.  In order to make the destructor work as expected (which calls udev_device_unref)
        // we manually manage the reference counts here.

        // Increase the reference count.
        udev_device_ref(m_device);

        auto parent_device = udev_device_get_parent_with_subsystem_devtype(m_device, subsystem_cstring_value.get(),
                                                                           devtype_cstring_value.get());
        if (parent_device)
        {
            // The reference count was auto-decremented, so we do not need to decrement the count here.
            // Retain a reference here. So that when the destructor runs the reference counts are correct.
            udev_device_ref(parent_device);
            return std::optional<Device>{Device{parent_device}};
        }

        // udev_device_get_parent_with_subsystem_devtype failed to find a parent, so it left m_device's
        // refcount unmodified which means we have one refcount too many.  Decrement the count here.
        udev_device_unref(m_device);
        return std::optional<Device>{};
    }

    Device::string_map_type Device::load_attributes_from_device_path()
    {
        string_map_type attribute_map;
        auto property_map = get_properties();
        if (property_map.contains("DEVPATH"))
        {
            FileManager manager;
            auto device_path = string_type("/sys") + property_map["DEVPATH"];
            load_sub_attributes_into_map(manager, attribute_map, device_path, "", false);

            auto get_parent_worked = get_parent();
            if (get_parent_worked)
            {
                attribute_map.merge(get_parent_worked.value().load_attributes_from_device_path());
            }
        }
        return attribute_map;
    }

    std::ostream & Device::description(std::ostream & o) const
    {
        bool needs_comma{false};

        auto data_writer = [&o, &needs_comma](const string_type & text,
                                              const std::function<string_type(const Device &)> & method,
                                              const Device & device) {
            if (const auto var = method(device); ! var.empty())
            {
                if (needs_comma)
                {
                    o << ",";
                }
                o << text << var;
                needs_comma = true;
            }
        };

        o << "Device(";

        data_writer("sysname=", &Device::get_sysname, *this);

        // OSTREAM_HELPER(sysname, "sysname=", get_sysname)

        // Do not output syspath, it is quite verbose.
        data_writer("syspath=", &Device::get_syspath, *this);
        data_writer("sysnum=", &Device::get_sysnum, *this);

        // Do not output dev path, it is quite verbose.
        data_writer("devpath=", &Device::get_devpath, *this);
        data_writer("subsystem=", &Device::get_subsystem, *this);
        data_writer("driver=", &Device::get_driver, *this);
        data_writer("action=", &Device::get_action, *this);

        o << ")";

        return o;
    }

    Device::string_map_type Device::get_values(const std::function<udev_list_entry *(udev_device *)> & f) const
    {
        string_map_type result_map;
        auto entries = f(m_device);
        if (entries == nullptr)
        {
            return result_map;
        }
        udev_list_entry * entry;
        udev_list_entry_foreach(entry, entries)
        {
            auto key = udev_list_entry_get_name(entry);
            auto value = udev_list_entry_get_value(entry);
            if (key && value)
            {
                result_map.insert(std::make_pair(key, value));
            }
        }
        return result_map;
    }

    Device::string_type Device::get_value(const std::function<const char *(udev_device *, const char * Key)> & f,
                                          const string_type & key)
    {
        string_type string_result;
        auto key_cstring_value = key.cStr();
        auto api_result = f(m_device, key_cstring_value.get());
        if (api_result)
        {
            string_result = api_result;
        }
        return string_result;
    }

    Device::string_type Device::get_property(const std::function<const char *(udev_device *)> & f) const
    {
        string_type string_result;
        auto api_result = f(m_device);
        if (api_result)
        {
            string_result = api_result;
        }
        return string_result;
    }

    void Device::load_sub_attributes_into_map(FileManager & manager, string_map_type & map, const string_type & path,
                                              const string_type & prefix, bool is_sub_dir)
    {
        static string_type attribute_path_separator{"/"};
        auto path_contents = manager.contentsOfDirectoryAtPath(path);

        auto uevent_path = path + FileManager::pathSeparator + "uevent";
        // If we have recursed into a sub-directory and that directory contains an uevent property,
        // then that directory is a child device.  Skip over it.
        if (is_sub_dir && manager.itemExistsAtPath(uevent_path))
        {
            return;
        }
        for (auto & path_item : path_contents)
        {
            // path_item is just the name of the item in the directory, not the full path to that
            // item.
            auto full_path_to_item = path + FileManager::pathSeparator + path_item;
            auto item_properties = manager.propertiesForItemAtPath(full_path_to_item);

            // If the item is not a regular file or a directory, then skip over it.
            if (! (item_properties.type == FileType::Regular || item_properties.type == FileType::Directory))
            {
                continue;
            }

            // If the item has the wrong permissions, then skip over it.
            if (! (item_properties.permission.hasUserReadPermission() ||
                   item_properties.permission.hasUserWritePermission()))
            {
                continue;
            }

            // The name of the attribute is a combination of the name of the item in the filesystem
            // and the prefix.  Sub-directory attributes have the form dir/name where the '/' character
            // separates the components of the name.
            auto attribute_name = prefix.length() > 0 ? prefix + attribute_path_separator + path_item : path_item;

            // After calculating the attribute name, see if the map already contains a value for the
            // attribute, if so, skip over it.
            if (map.contains(attribute_name))
            {
                continue;
            }

            // If the path is a directory then recurse into that directory.
            if (manager.directoryExistsAtPath(full_path_to_item))
            {
                load_sub_attributes_into_map(manager, map, full_path_to_item, attribute_name, true);
                continue;
            }

            FileHandle read_handle;
            decltype(read_handle)::data_type data;

            // Try to open the item for reading and try reading the data in the file.  Be careful to catch
            // the exception here as some files will either not open or not read.
            try
            {
                read_handle = FileHandle::fileHandleForReadingAtPath(full_path_to_item);
                read_handle.setAutoClose(true);
                data = read_handle.readAvailableData();
            }
            catch (std::runtime_error & e)
            {
                continue;
            }

            // The data object contains the file contents, but these are not just arbitrary bytes, they are
            // string contents.  We can turn the contents back into a string and stick the attribute/value
            // in the map.  We also take care to remove any trailing newline character.
            auto the_bytes = data.bytes();
            auto the_length = data.length();
            if (the_bytes != nullptr && the_length > 0)
            {
                auto length_to_insert = *(the_bytes + the_length - 1) == '\n' ? the_length - 1 : the_length;
                string_type attribute_value;
                try
                {
                    attribute_value = string_type{the_bytes, length_to_insert};
                }
                catch (std::runtime_error & e)
                {
                    continue;
                }
                map.insert(std::make_pair(attribute_name, attribute_value));
            }
        }
    }

    std::ostream & operator<<(std::ostream & o, const Device & d)
    {
        return d.description(o);
    }

    Query::Query(const context_type & ctx) : m_enumerator(nullptr)
    {
        m_enumerator = udev_enumerate_new(ctx.m_context);
        if (m_enumerator == nullptr)
        {
            throw system_no_code_error{"unable to create the libudev enumerator"};
        }
    }

    Query::~Query()
    {
        udev_enumerate_unref(m_enumerator);
    }

    void Query::retain()
    {
        udev_enumerate_ref(m_enumerator);
    }

    void Query::release()
    {
        udev_enumerate_unref(m_enumerator);
    }

    void Query::match_subsystem(const string_type & subsystem)
    {
        if (subsystem.length() > 0)
        {
            auto subsystem_cstring_value = subsystem.cStr();
            auto udev_api_result = udev_enumerate_add_match_subsystem(m_enumerator, subsystem_cstring_value.get());
            if (udev_api_result < 0)
            {
                throw system_no_code_error{"add match failed"};
            }
        }
    }

    void Query::do_not_match_subsystem(const string_type & subsystem)
    {
        if (subsystem.length() > 0)
        {
            auto subsystem_cstring_value = subsystem.cStr();
            auto udev_api_result = udev_enumerate_add_nomatch_subsystem(m_enumerator, subsystem_cstring_value.get());
            if (udev_api_result < 0)
            {
                throw system_no_code_error{"add nomatch failed"};
            }
        }
    }

    void Query::match_system_attribute(const string_type & attribute, const string_type & value)
    {
        if (attribute.length() > 0 && value.length() > 0)
        {
            auto attribute_cstring_value = attribute.cStr();
            auto value_cstring_value = value.cStr();
            auto udev_api_result = udev_enumerate_add_match_sysattr(m_enumerator, attribute_cstring_value.get(),
                                                                    value_cstring_value.get());
            if (udev_api_result < 0)
            {
                throw system_no_code_error{"add sysattr match failed"};
            }
        }
    }

    void Query::do_not_match_system_attribute(const string_type & attribute, const string_type & value)
    {
        if (attribute.length() > 0 && value.length() > 0)
        {
            auto attribute_cstring_value = attribute.cStr();
            auto value_cstring_value = value.cStr();
            auto udev_api_result = udev_enumerate_add_nomatch_sysattr(m_enumerator, attribute_cstring_value.get(),
                                                                      value_cstring_value.get());
            if (udev_api_result < 0)
            {
                throw system_no_code_error{"add nomatch sysattr failed"};
            }
        }
    }

    void Query::match_property(const string_type & property, const string_type & value)
    {
        if (property.length() > 0 && value.length() > 0)
        {
            auto property_cstring_value = property.cStr();
            auto value_cstring_value = value.cStr();
            auto udev_api_result = udev_enumerate_add_match_property(m_enumerator, property_cstring_value.get(),
                                                                     value_cstring_value.get());
            if (udev_api_result < 0)
            {
                throw system_no_code_error{"add match property failed"};
            }
        }
    }

    void Query::match_system_name(const string_type & name)
    {
        if (name.length() > 0)
        {
            auto name_cstring_value = name.cStr();
            auto udev_api_result = udev_enumerate_add_match_sysname(m_enumerator, name_cstring_value.get());
            if (udev_api_result < 0)
            {
                throw system_no_code_error{"add match sysname failes"};
            }
        }
    }

    void Query::match_tag(const string_type & tag)
    {
        if (tag.length() > 0)
        {
            auto tag_cstring_value = tag.cStr();
            auto udev_api_result = udev_enumerate_add_match_tag(m_enumerator, tag_cstring_value.get());
            if (udev_api_result < 0)
            {
                throw system_no_code_error{"add match tag failed"};
            }
        }
    }

    void Query::match_parent(const Device & device)
    {
        if (device.m_device)
        {
            auto udev_api_result = udev_enumerate_add_match_parent(m_enumerator, device.m_device);
            if (udev_api_result < 0)
            {
                throw system_no_code_error{"add match parent failed"};
            }
        }
    }

    Query::string_list_type Query::run()
    {
        string_list_type query_results_list;
        udev_enumerate_scan_devices(m_enumerator);
        auto devices = udev_enumerate_get_list_entry(m_enumerator);
        udev_list_entry * device_list_entry;

        udev_list_entry_foreach(device_list_entry, devices)
        {
            auto entry = udev_list_entry_get_name(device_list_entry);
            query_results_list.emplace_back(entry);
        }

        return query_results_list;
    }

    Monitor::Monitor(const context_type & ctx, const string_type & name) : m_monitor{nullptr}
    {
        auto name_cstring_contents = name.cStr();
        m_monitor = udev_monitor_new_from_netlink(ctx.m_context, name_cstring_contents.get());
        if (m_monitor == nullptr)
        {
            throw system_no_code_error{"new monitor from netlink failed"};
        }
    }

    Monitor::~Monitor()
    {
        udev_monitor_unref(m_monitor);
    }

    void Monitor::match_subsystem_and_devtype(const string_type & subsystem, const string_type & devtype)
    {
        auto subsystem_cstring_contents = subsystem.cStr();
        auto devtype_cstring_contents = devtype.cStr();
        auto udev_api_result = udev_monitor_filter_add_match_subsystem_devtype(
            m_monitor, subsystem_cstring_contents.get(), devtype_cstring_contents.get());
        if (udev_api_result < 0)
        {
            throw system_no_code_error{"add match subsystem and devtype failed"};
        }
    }

    void Monitor::match_subsystem(const string_type & subsystem)
    {
        auto substring_cstring_contents = subsystem.cStr();
        auto udev_api_result =
            udev_monitor_filter_add_match_subsystem_devtype(m_monitor, substring_cstring_contents.get(), nullptr);
        if (udev_api_result < 0)
        {
            throw system_no_code_error{"add match subsystem and devtype failed (no devtype)"};
        }
    }

    void Monitor::match_tag(const string_type & tag)
    {
        auto tag_cstring_value = tag.cStr();
        auto udev_api_result = udev_monitor_filter_add_match_tag(m_monitor, tag_cstring_value.get());
        if (udev_api_result < 0)
        {
            throw system_no_code_error{"add match tag failed"};
        }
    }

    int Monitor::get_file_descriptor() const
    {
        return udev_monitor_get_fd(m_monitor);
    }

    Monitor::context_type Monitor::get_context() const
    {
        Context ctx;
        ctx.m_context = udev_monitor_get_udev(m_monitor);
        return ctx;
    }

    void Monitor::set_receive_buffer_size(int size)
    {
        auto udev_api_result = udev_monitor_set_receive_buffer_size(m_monitor, size);
        if (udev_api_result < 0)
        {
            throw system_no_code_error{"set receive buffer size failed"};
        }
    }

    void Monitor::monitor()
    {
        auto udev_api_result = udev_monitor_enable_receiving(m_monitor);
        if (udev_api_result < 0)
        {
            throw system_no_code_error{"enable receiving failed"};
        }
    }

    Monitor::device_type Monitor::get_device()
    {
        device_type dev;
        dev.m_device = udev_monitor_receive_device(m_monitor);
        if (dev.m_device == nullptr)
        {
            throw system_no_code_error{"receive device failed"};
        }
        return dev;
    }

    void Monitor::retain()
    {
        udev_monitor_ref(m_monitor);
    }

    void Monitor::release()
    {
        udev_monitor_unref(m_monitor);
    }

} // namespace TF::Linux::Udev
