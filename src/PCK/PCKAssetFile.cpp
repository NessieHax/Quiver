#include "PCK/PCKAssetFile.h"

size_t PCKAssetFile::getFileSize() const
{
	return mData.Size;
}

const Buffer &PCKAssetFile::getData() const
{
	return mData;
}

void PCKAssetFile::setData(const std::vector<unsigned char> &data)
{
	mData = data;
}

const std::filesystem::path &PCKAssetFile::getPath() const
{
	return mPath;
}

void PCKAssetFile::setPath(const std::filesystem::path &inpath)
{
	mPath = inpath;
}

PCKAssetFile::Type PCKAssetFile::getAssetType() const
{
	return mAssetType;
}

void PCKAssetFile::addProperty(const std::string &key, const std::u16string &value)
{
	mProperties.push_back(PCKAssetFile::Property(key, value));
}

void PCKAssetFile::removeProperty(int index)
{
	if (index >= 0 && index < (int)mProperties.size())
		mProperties.erase(mProperties.begin() + index);
}

void PCKAssetFile::setPropertyAtIndex(int index, const std::string &key, const std::u16string &value)
{
	if (index < 0 || index >= (int)mProperties.size())
		return;
	mProperties[index] = {key, value};
}

void PCKAssetFile::clearProperties()
{
	mProperties.clear();
}

const std::vector<PCKAssetFile::Property> &PCKAssetFile::getProperties() const
{
	return mProperties;
}

const PCKAssetFile::Type IMAGE_ASSET_TYPES[]{PCKAssetFile::Type::SKIN, PCKAssetFile::Type::CAPE, PCKAssetFile::Type::TEXTURE};

bool PCKAssetFile::isImageType() const
{
	return mAssetType >= PCKAssetFile::Type::SKIN && mAssetType <= PCKAssetFile::Type::TEXTURE;
}