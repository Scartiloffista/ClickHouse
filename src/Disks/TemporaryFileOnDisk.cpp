#include <Disks/TemporaryFileOnDisk.h>
#include <Disks/IDisk.h>
#include <Poco/TemporaryFile.h>


namespace DB
{

TemporaryFileOnDisk::TemporaryFileOnDisk(const DiskPtr & disk_, const String & prefix_, bool create_directories)
    : disk(disk_)
{
    String dummy_prefix = "a/";
    filepath = Poco::TemporaryFile::tempName(dummy_prefix);
    dummy_prefix += "tmp";
    assert(filepath.starts_with(dummy_prefix));
    filepath.replace(0, dummy_prefix.length(), prefix_);

    if (create_directories)
        disk->createDirectories(parentPath(getPath()));
}

TemporaryFileOnDisk::~TemporaryFileOnDisk()
{
    try
    {
        if (disk && !filepath.empty() && disk->exists(filepath))
            disk->removeRecursive(filepath);
    }
    catch (...)
    {
    }
}

}
