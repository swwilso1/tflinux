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

#include "tffilesystems.hpp"

namespace TF::Linux
{

    auto operator<<(std::ostream & o, const FileSystem & fs) -> std::ostream &
    {
        o << fileSystemTypeToString(fs);
        return o;
    }

    auto stringToFileSystemType(const TF::Foundation::String & s) -> FileSystem
    {
        auto lower_case_s = s.lowercaseString();
        if (lower_case_s == "vfat" || lower_case_s == "fat32")
        {
            return FileSystem::FAT32;
        }
        else if (lower_case_s == "ntfs")
        {
            return FileSystem::NTFS;
        }
        else if (lower_case_s == "ext4")
        {
            return FileSystem::EXT4;
        }
        else if (lower_case_s == "ext3")
        {
            return FileSystem::EXT3;
        }
        else if (lower_case_s == "ext2")
        {
            return FileSystem::EXT2;
        }
        else if (lower_case_s == "hfsplus")
        {
            return FileSystem::HFSPLUS;
        }
        else if (lower_case_s == "apfs")
        {
            return FileSystem::APFS;
        }
        else if (lower_case_s == "cifs")
        {
            return FileSystem::CIFS;
        }

        return FileSystem::UNKNOWN;
    }

    auto fileSystemTypeToString(FileSystem t) -> String
    {
        TF::Foundation::String type;
        switch (t)
        {
            case FileSystem::EXT2:
                type = "ext2";
                break;
            case FileSystem::EXT3:
                type = "ext3";
                break;
            case FileSystem::EXT4:
                type = "ext4";
                break;
            case FileSystem::FAT16:
            case FileSystem::FAT32:
                type = "vfat";
                break;
            case FileSystem::NTFS:
                type = "ntfs";
                break;
            case FileSystem::HFSPLUS:
                type = "hfsplus";
                break;
            case FileSystem::APFS:
                type = "apfs";
                break;
            case FileSystem::CIFS:
                type = "cifs";
                break;
            case FileSystem::UNKNOWN:
            default:
                type = "unknown";
        }

        return type;
    }

    auto fileSystemTypeIsMountable(FileSystem t) -> bool
    {
        bool can_mount{false};
        switch (t)
        {
            case FileSystem::EXT2:
            case FileSystem::EXT3:
            case FileSystem::EXT4:
            case FileSystem::FAT16:
            case FileSystem::FAT32:
            case FileSystem::CIFS:
                can_mount = true;
                break;
            default:
                break;
        }

        return can_mount;
    }

    auto fileSystemTypeIsFormattable(FileSystem t) -> bool
    {
        bool can_format{false};
        switch (t)
        {
            case FileSystem::EXT2:
            case FileSystem::EXT3:
            case FileSystem::EXT4:
            case FileSystem::FAT16:
            case FileSystem::FAT32:
                can_format = true;
                break;
            default:
                break;
        }

        return can_format;
    }

} // namespace TF::Linux
