#include <libudev.h>

int main()
{
    auto entry = udev_device_get_current_tags_list_entry(nullptr);
    return 0;
}
