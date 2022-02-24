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

#include <cstdio>
#include <mntent.h>
#include "tfmounttable.hpp"

namespace TF::Linux
{
    bool MountTableEntry::operator==(const MountTableEntry & e) const
    {
        if (this == &e)
        {
            return true;
        }

        return file_system_name == e.file_system_name && directory == e.directory && type == e.type &&
               options == e.options && frequency == e.frequency && pass_number == e.pass_number;
    }

    MountTable load_mount_table()
    {
        MountTable table{};
        struct mntent * entry;

        auto handle = setmntent("/proc/mounts", "r");
        if (handle == nullptr)
        {
            throw std::system_error{errno, std::system_category()};
        }

        while ((entry = getmntent(handle)) != nullptr)
        {
            MountTableEntry table_entry{};
            if (entry->mnt_fsname)
            {
                table_entry.file_system_name = entry->mnt_fsname;
            }

            if (entry->mnt_dir)
            {
                table_entry.directory = entry->mnt_dir;
            }

            if (entry->mnt_type)
            {
                table_entry.type = entry->mnt_type;
            }

            if (entry->mnt_opts)
            {
                table_entry.options = entry->mnt_opts;
            }

            table_entry.frequency = entry->mnt_freq;
            table_entry.pass_number = entry->mnt_passno;

            table.emplace_back(table_entry);
        }
        return table;
    }

} // namespace TF::Linux
