
#include "ZipArchive.h"

using namespace egg;

ZipEntry::ZipEntry()
{
}

ZipEntry::~ZipEntry()
{
}

std::string ZipEntry::getName()
{
    return "";
}

bool ZipEntry::isFile()
{
    return false;
}

bool ZipEntry::isDir()
{
    return false;
}

unsigned int ZipEntry::getLen()
{
    return 0;
}

void ZipEntry::read(char* data)
{
}




ZipArchive::ZipArchive(egg::niubi_entry_arg& arg)
    :Archive(arg)
{
}

ZipArchive::~ZipArchive()
{
}

//std::string ZipArchive::getExt()
//{
//    return "";
//}

void ZipArchive::open(const std::string path, int mode)
{
}

void ZipArchive::close()
{
}

bool ZipArchive::isOpen()
{
    return false;
}

Entry* ZipArchive::getEntry(const std::string path)
{
    return 0;
}

void ZipArchive::setEntryAsFile(const std::string path, const char* data, unsigned int len, bool _override)
{
}

bool ZipArchive::newEntryAsDir(const std::string path)
{
    return false;
}

void ZipArchive::delEntry(const std::string path)
{
}

std::vector<std::string> ZipArchive::listFile(const char* path, bool ergodic)
{

    return std::vector<std::string>();
}