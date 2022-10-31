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

#include <random>
#include <chrono>
#include <sys/mount.h>
#include "tfmounter.hpp"

namespace TF::Linux
{

    static constexpr int s_random_length = 6;

    auto Mounter::mount(const string_type & src, const string_type & dst, file_system_type fs_type, uint32_t flags,
                        const string_type & options) -> bool
    {
        if (src.length() == 0 || dst.length() == 0)
        {
            return false;
        }

        auto src_cstr = src.cStr();
        auto dst_cstr = dst.cStr();

        auto filesystem = fileSystemTypeToString(fs_type);
        auto fs_cstr = filesystem.cStr();

        std::unique_ptr<char[]> ptr;
        const char * option_value = nullptr;

        if (options.length() > 0)
        {
            ptr = options.cStr();
            option_value = ptr.get();
        }

        auto mount_result = ::mount(src_cstr.get(), dst_cstr.get(), fs_cstr.get(), flags, option_value);
        if (mount_result < 0)
        {
            LOG(LogPriority::Info, "Failed to mount %@ at %@: (%d) %@", src, dst, errno, strerror(errno))
            return false;
        }

        return true;
    }

    auto Mounter::mount(const string_type & src, file_system_type fs_type, uint32_t flags, const string_type & options)
        -> std::optional<string_type>
    {
        FileManager manager;
        // TODO: Need a default directory.
        string_type mount_point = manager.temporaryDirectory();

        while (true)
        {
            mount_point +=
                FileManager::pathSeparator + "mounter_" + RandomStringGenerator::string_with_length(s_random_length);

            if (manager.itemExistsAtPath(mount_point))
            {
                continue;
            }
            break;
        }

        if (! manager.directoryExistsAtPath(mount_point))
        {
            // TODO: How do we handle a failure?
            manager.createDirectoryAtPath(mount_point);
        }

        auto mount_result = mount(src, mount_point, fs_type, flags, options);

        if (mount_result)
        {
            std::optional<string_type> the_optional{mount_point};
            return the_optional;
        }

        return std::optional<string_type>{};
    }

    auto Mounter::mount_in_dir(const string_type & src, const string_type & dir, file_system_type fs_type,
                               uint32_t flags, const string_type & options) -> std::optional<string_type>
    {
        FileManager manager;
        if (! manager.directoryExistsAtPath(dir))
        {
            return std::optional<string_type>{};
        }

        auto mount_point = dir;

        while (true)
        {
            mount_point +=
                FileManager::pathSeparator + "mounter_" + RandomStringGenerator::string_with_length(s_random_length);
            if (manager.itemExistsAtPath(mount_point))
            {
                continue;
            }
            break;
        }

        if (! manager.directoryExistsAtPath(mount_point))
        {
            manager.createDirectoryAtPath(mount_point);
        }

        auto mount_result = mount(src, mount_point, fs_type, flags, options);

        if (mount_result)
        {
            std::optional<string_type> the_optional{mount_point};
            return the_optional;
        }

        return std::optional<string_type>{};
    }

    auto Mounter::unmount(const string_type & target) -> bool
    {
        return unmount(target, 0);
    }

    auto Mounter::unmount(const string_type & target, int flags) -> bool
    {
        auto target_cstr = target.cStr();
        auto result = umount2(target_cstr.get(), flags);
        if (result < 0)
        {
            LOG(LogPriority::Info, "Failed to unmount %@: (%d) %@", target, errno, strerror(errno))
            return false;
        }
        return true;
    }

} // namespace TF::Linux
