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

#ifndef TFUDEV_HPP
#define TFUDEV_HPP

#include <ostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <libudev.h>
#include "TFFoundation.hpp"

using namespace TF::Foundation;

namespace TF::Linux::Udev
{

    /** Forward declare these classes for use in friend declarations */
    class Query;
    class Device;
    class Monitor;

    /**
     * Operations on libudev require a struct udev context object.  This Context
     * class wraps the udev object and provides simple memory management for
     * the context.
     */
    class Context
    {
    public:
        /**
         * @brief default constructor.
         */
        Context();

        /**
         * @brief destructor
         */
        ~Context();

        /**
         * @brief method to increase the reference count of the udev context.
         */
        void retain();

        /**
         * @brief method to decrease the reference count of the udev context.
         */
        void release();

    private:
        struct udev * m_context;

        // Query needs access to the udev *.
        friend class Query;

        // Device needs access to the udev *.
        friend class Device;

        // Monitor needs access to the udev *.
        friend class Monitor;
    };

    /**
     * The Device class encapsulates the udev device functionality.
     */
    class Device
    {
    public:
        using context_type = Context;
        using string_type = String;
        using string_map_type = std::unordered_map<String, String>;

        /**
         * @brief default constructor needed for use with Result<Device> objects.
         */
        Device() : m_device{nullptr} {}

        /**
         * @brief copy constructor
         * @param d the other d
         */
        Device(Device & d);

        /**
         * @brief move constructor
         * @param d the other d
         */
        Device(Device && d);

        /**
         * @brief constructor with context and path
         * @param ctx the udev context
         * @param path the path
         */
        Device(const context_type & ctx, const string_type & path);

        /**
         * @brief constructor with context, device type and device number
         * @param ctx the udev context
         * @param type the device type
         * @param number the number
         */
        Device(const context_type & ctx, char type, dev_t number);

        /**
         * @brief constructor with context, subsystem and system name
         * @param ctx the udev context
         * @param subsystem the subsystem name
         * @param sysname the system name
         */
        Device(const context_type & ctx, const string_type & subsystem, const string_type & sysname);

        /** destructor */
        ~Device();

        /**
         * @brief copy assignment operator
         * @param d the other device
         * @return this device with a reference to @e d.
         */
        Device & operator=(Device & d);

        /**
         * @brief move assignment operator
         * @param d the other device
         * @return this device witha reference @e d.
         */
        Device & operator=(Device && d);

        /**
         * @brief method to increase the reference count on the device object.
         */
        void retain();

        /**
         * @brief method to decrease the reference count on the device object.
         */
        void release();

        /**
         * @brief method to get a map of all the key/value pairs of links associated
         * with the device.
         * @return the map of the links.
         */
        [[nodiscard]] string_map_type get_devlinks() const;

        /**
         * @brief method to get a map of the all the key/value pairs of properties associated
         * with the device.
         * @return the map of the properties
         */
        [[nodiscard]] string_map_type get_properties() const;

        /**
         * @brief method to get a map of the key/value pairs of tags associated with the
         * device.
         * @return the map of tags.
         */
        [[nodiscard]] string_map_type get_tags() const;

        /**
         * @brief method to get a map of the key/value pairs of the 'current' tags associated
         * with the device.
         * @return the map of 'current' tags.
         */
        [[nodiscard]] string_map_type get_current_tags() const;

        /**
         * @brief method to get map of the key/value pairs of the tags associated with
         * the device.
         * @return the map of attributes.
         */
        [[nodiscard]] string_map_type get_system_attributes() const;

        /**
         * @brief method to retrieve a property value for a given
         * property key.
         * @param key the key
         * @return the value associated with key
         */
        [[nodiscard]] string_type get_property_value_for_key(const string_type & key);

        /**
         * @brief method to get a system attribute value for a key.
         * @param key the key
         * @return the value associated with the key
         */
        [[nodiscard]] string_type get_system_attribute_for_key(const string_type & key);

        /**
         * @brief method to set a system attribute with key and value
         * @param key the key
         * @param value the value
         */
        void set_system_attribute_value(const string_type & key, const string_type & value);

        /**
         * @brief method to get the sys path for the device
         * @return the path
         */
        [[nodiscard]] string_type get_syspath() const;

        /**
         * @brief method to get the system name for the device
         * @return the name
         */
        [[nodiscard]] string_type get_sysname() const;

        /**
         * @brief method to get the system number of the device.
         * @return the number in string form.
         */
        [[nodiscard]] string_type get_sysnum() const;

        /**
         * @brief method to get the device path of the device.
         * @return the path.
         */
        [[nodiscard]] string_type get_devpath() const;

        /**
         * @brief method to get the device node of the device
         * @return the node of the device in string form.
         */
        [[nodiscard]] string_type get_devnode() const;

        /**
         * @brief method to get the device type of the device.
         * @return the node of the device in string form.
         */
        [[nodiscard]] string_type get_devtype() const;

        /**
         * @brief method to get the subsystem of the device.
         * @return the subsystem
         */
        [[nodiscard]] string_type get_subsystem() const;

        /**
         * @brief method to get the driver of the device
         * @return the driver
         */
        [[nodiscard]] string_type get_driver() const;

        /**
         * @brief method to get the action associated with the device
         * @return 'add', 'remove', 'change' as a device action.
         */
        [[nodiscard]] string_type get_action() const;

        /**
         * @brief method to get the device number of the device
         * @return the number
         */
        [[nodiscard]] dev_t get_devnum() const;

        /**
         * @brief method to return the parent of a device
         * @return a result object whose succeeded member is
         * true if the parent was retrieved and false otherwise.
         * On success the result object value member contains the
         * parent device.
         */
        [[nodiscard]] Result<Device> get_parent();

        /**
         * @brief method to get the parent of the device with a particular subsystem and device type
         * @param subsystem the subsystem
         * @param devtype the device type
         * @return a result object whose succeeded member is true if the parent was retrieved and
         * false otherwise.  On success the result object value member contains the parent device.
         */
        [[nodiscard]] Result<Device> get_parent_with_subsystem_dev_type(const string_type & subsystem,
                                                                        const string_type & devtype);

        /**
         * @brief method to load attributes of a device from the system path of the device.
         * @return a map of the key/value pairs of attributes.
         *
         * This method walks back up the tree of parents and collects those attributes as well.
         */
        [[nodiscard]] string_map_type load_attributes_from_device_path();

        /**
         * @brief method for helping write a device object to a stream
         * @param o the stream object
         * @return the stream object.
         */
        [[nodiscard]] std::ostream & description(std::ostream & o) const;

    private:
        /**
         * @brief method to help with the udev library functions that all return
         * key/value pairs.
         * @param f the udev function to execute
         * @return a map of the key value pairs returned by the udev function call.
         */
        [[nodiscard]] string_map_type get_values(const std::function<udev_list_entry *(udev_device *)> & f) const;

        /**
         * @brief helper method for udev functions that take a key and return a value.
         * @param f the udev function
         * @param key the key
         * @return the value associated with key.
         */
        [[nodiscard]] string_type get_value(const std::function<const char *(udev_device *, const char * Key)> & f,
                                            const string_type & key);

        /**
         * @brief helper method for udev functions that return a property-like value.
         * @param f the udev library function
         * @return the property-like value returned from the library function.
         */
        [[nodiscard]] string_type get_property(const std::function<const char *(udev_device *)> & f) const;

        /**
         * @brief helper method to assist in loading attributes for a device from the /sys
         * directory into a key/value pair map
         * @param manager the file manager to assist in reading data
         * @param map the map to store the results
         * @param path the path of the directory to read for attributes
         * @param prefix the prefix of the attribute (using for recursive descent of attribute trees)
         * @param is_sub_dir true if the method is called on a sub-directory (ie a recursed dir) and
         * false otherwise.
         */
        static void load_sub_attributes_into_map(FileManager & manager, string_map_type & map, const string_type & path,
                                                 const string_type & prefix, bool is_sub_dir);

        udev_device * m_device;

        // A query needs access to the libudev udev_device pointer.
        friend class Query;

        // A Monitor needs access to the libudev udev_device pointer.
        friend class Monitor;
    };

    /**
     * The Query class encapsulates the udev library concept of enumeration
     * requests.
     */
    class Query
    {
    public:
        using context_type = Context;
        using string_type = String;
        using string_list_type = std::vector<string_type>;

        /**
         * @brief constructor
         * @param ctx the context object.
         */
        explicit Query(const context_type & ctx);

        /** destructor */
        ~Query();

        /**
         * @brief method to increase the references to the query object.
         */
        void retain();

        /**
         * @brief method to decrement the referrences to the query object.
         */
        void release();

        /**
         * @brief method to add a subsystem value to the query.
         * @param subsystem the subsystem
         */
        void match_subsystem(const string_type & subsystem);

        /**
         * @brief method to tell the query not to match a subsystem.
         * @param subsystem the subsystem
         */
        void do_not_match_subsystem(const string_type & subsystem);

        /**
         * @brie method to add an attribute/value pair to the query
         * @param attribute the attribute name
         * @param value the value
         */
        void match_system_attribute(const string_type & attribute, const string_type & value);

        /**
         * @brief method to tell the query to not match an attribute/value pair.
         * @param attribute the attribute
         * @param value the value
         */
        void do_not_match_system_attribute(const string_type & attribute, const string_type & value);

        /**
         * @brief method to match a property/value pair.
         * @param property
         * @param value
         */
        void match_property(const string_type & property, const string_type & value);

        /**
         * @brief method to match a system name
         * @param name
         */
        void match_system_name(const string_type & name);

        /**
         * @brief method to match a tag
         * @param tag
         */
        void match_tag(const string_type & tag);

        /**
         * @brief method to have the query match a device with a given parent.
         * @param device the parent device
         */
        void match_parent(const Device & device);

        /**
         * @brief method to run the query and return the values.
         * @return a list of the values
         */
        string_list_type run();

    private:
        udev_enumerate * m_enumerator;

        // Need access to Device internal for setting parent.
        //        friend class Device;
    };

    /**
     * The Monitor class encapsulates the functionality needed to set up
     * a device monitor.  The only thing not provided is a poll kind of object
     * that will check to see if there is a device ready as a result of monitoring.
     *
     * The class lets you specify the kind of device to monitor for and the system
     * will notify the monitor when a device is ready.
     */
    class Monitor
    {
    public:
        using context_type = Context;
        using device_type = Device;
        using string_type = String;

        /**
         * @brief constructor with context and name
         * @param ctx
         * @param name
         */
        Monitor(const context_type & ctx, const string_type & name);

        /** descrutor */
        ~Monitor();

        /**
         * @brief method to match a subsystem and device type
         * @param subsystem
         * @param devtype
         */
        void match_subsystem_and_devtype(const string_type & subsystem, const string_type & devtype);

        /**
         * @brief method to match a subsystem
         * @param subsystem
         */
        void match_subsystem(const string_type & subsystem);

        /**
         * @brief method to match a tag
         * @param tag
         */
        void match_tag(const string_type & tag);

        /**
         * @brief method to get the file descriptor for the monitor
         * @return the file descriptor
         */
        [[nodiscard]] int get_file_descriptor() const;

        /**
         * @brief method to get the context object used by the monitor
         * @return the context object used by the monitor
         */
        [[nodiscard]] context_type get_context() const;

        /**
         * @brief method to set the receive buffer size
         * @param size
         */
        void set_receive_buffer_size(int size);

        /**
         * @brief method to start the monitor listening for events from the
         * system.
         */
        void monitor();

        /**
         * @brief method to get a device from the monitor when a device matches
         * @return a device that matched the monitor specifications.
         */
        [[nodiscard]] device_type get_device();

        /**
         * @brief method to increase the reference count on the monitor object.
         */
        void retain();

        /**
         * @brief mthod to decrease the reference count on the monitor object.
         */
        void release();

    private:
        udev_monitor * m_monitor;
    };

} // namespace TF::Linux::Udev

#endif // TFUDEV_HPP
