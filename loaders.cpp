#include "loaders.hpp"


FeedbackLoader::FeedbackLoader(DeviceLoader *pParent) : parent_(*pParent) { assert(pParent); }

QString FeedbackLoader::Name() const { return "Feedback"; }

void FeedbackLoader::BeginElement(const QXmlStreamReader& reader)
{
    parent_.AddFolderElement(std::make_unique<Feedback>(
                                      reader.attributes().value("Name").toString(),
                                      reader.attributes().value("Number").toInt()));
}



CommandLoader::CommandLoader(DeviceLoader* pDeviceLoader) : parent_(*pDeviceLoader) { assert(pDeviceLoader); }

QString CommandLoader::Name() const { return "Command"; }

void CommandLoader::BeginElement(const QXmlStreamReader &reader)
{
    parent_.AddFolderElement(std::make_unique<Command>(
                                      reader.attributes().value("Name").toString(),
                                      reader.attributes().value("Number").toInt()));
}


DeviceLoader::DeviceLoader(Project* pProject)
    : AbstractXmlElement("Device")
    , project_(*pProject)
{
    assert(pProject);

    auto commands = std::make_unique<AbstractXmlElement>("Commands");
    commands->AddChild(std::make_unique<CommandLoader>(this));
    AddChild(std::move(commands));

    auto feedbacks = std::make_unique<AbstractXmlElement>("Feedbacks");
    feedbacks->AddChild(std::make_unique<FeedbackLoader>(this));
    AddChild(std::move(feedbacks));
}

QString DeviceLoader::Name() const { return "Device"; }

void DeviceLoader::AddFolderElement(std::unique_ptr<FolderElement> elem)
{
    pDevice_->RootFolder().Add(std::move(elem));
}

void DeviceLoader::BeginElement(const QXmlStreamReader &reader)
{
    pDevice_ = std::make_unique<Device>(reader.attributes().value("Name").toString());
}

void DeviceLoader::EndElement()
{
    project_.Add(std::move(pDevice_));
}
