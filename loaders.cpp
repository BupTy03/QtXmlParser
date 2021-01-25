#include "loaders.hpp"


RootFolder::~RootFolder()
{
}


FeedbackLoader::FeedbackLoader(RootFolder* pFolder) : pFolder_(pFolder) { assert(pFolder_); }

QString FeedbackLoader::Name() const { return "Feedback"; }

void FeedbackLoader::BeginElement(const QXmlStreamReader& reader)
{
    pFolder_->AddFolderElement(std::make_unique<Feedback>(
                          reader.attributes().value("Name").toString(),
                          reader.attributes().value("Number").toInt()));
}



CommandLoader::CommandLoader(RootFolder* pFolder) : pFolder_(pFolder) { assert(pFolder_); }

QString CommandLoader::Name() const { return "Command"; }

void CommandLoader::BeginElement(const QXmlStreamReader &reader)
{
    pFolder_->AddFolderElement(std::make_unique<Command>(
                          reader.attributes().value("Name").toString(),
                          reader.attributes().value("Number").toInt()));
}


DeviceLoader::DeviceLoader(Project* pProject)
    : AbstractXmlElement("Device")
    , project_(*pProject)
    , pDeviceRootFolder_()
{
    assert(pProject);
}

QString DeviceLoader::Name() const { return "Device"; }

void DeviceLoader::AddFolderElement(std::unique_ptr<FolderElement> elem)
{
    pDeviceRootFolder_->Add(std::move(elem));
}

void DeviceLoader::BeginElement(const QXmlStreamReader& reader)
{
    deviceName_ = reader.attributes().value("Name").toString();
    pDeviceRootFolder_ = std::make_unique<Folder>("Root");
}

void DeviceLoader::EndElement()
{
    project_.Add(std::make_unique<Device>(std::move(deviceName_), std::move(pDeviceRootFolder_)));
}



FolderLoader::FolderLoader(RootFolder* pParent)
    : AbstractXmlElement("Folder")
    , pParent_(pParent)
    , pCurrentFolder_()
{
    assert(pParent_ != nullptr);
}

void FolderLoader::AddFolderElement(std::unique_ptr<FolderElement> elem)
{
    if(pCurrentFolder_ == nullptr)
        throw std::runtime_error("Attemt to add element in unexisting folder");

    pCurrentFolder_->Add(std::move(elem));
}

void FolderLoader::BeginElement(const QXmlStreamReader& reader)
{
    pCurrentFolder_ = std::make_unique<Folder>(reader.attributes().value("Name").toString());
}

void FolderLoader::EndElement()
{
    pParent_->AddFolderElement(std::move(pCurrentFolder_));
}
