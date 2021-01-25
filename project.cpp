#include "project.hpp"
#include "abstractxmlelement.hpp"
#include "loaders.hpp"
#include "folder.hpp"

#include <QXmlStreamReader>

#include <iostream>
#include <stdexcept>


Device::Device(QString Name, std::unique_ptr<Folder> pRootFolder)
    : name_(std::move(Name))
    , pRootFolder_(std::move(pRootFolder))
{
}

Device::~Device()
{

}

QString Device::Name() const { return name_; }

Folder& Device::RootFolder() { return *pRootFolder_; }

const Folder& Device::RootFolder() const { return *pRootFolder_; }

void Project::Add(std::unique_ptr<Device> d)
{
    if(Contains(d->Name()))
        throw std::runtime_error("Project already contains device with name \"" + d->Name().toStdString() + "\"");

    devices_.emplace_back(std::move(d));
}

bool Project::Contains(const QString &name) const
{
    return std::find_if(std::begin(devices_), std::end(devices_),
                        [&](auto&& d) { return d->Name() == name; }) != std::end(devices_);
}

const std::vector<std::unique_ptr<Device>>& Project::Devices() const { return devices_; }



std::unique_ptr<Project> LoadProject(QXmlStreamReader& reader)
{
    reader.readNext();
    reader.readNext();

    auto project = std::make_unique<Project>();
    auto device = std::make_unique<DeviceLoader>(project.get());

    auto commands = std::make_unique<AbstractXmlElement>("Commands");
    commands->AddChild(std::make_unique<CommandLoader>(device.get()));

    auto commandsFolder = std::make_unique<FolderLoader>(device.get());
    commandsFolder->AddChild(std::make_unique<CommandLoader>(commandsFolder.get()));
    commands->AddChild(std::move(commandsFolder));

    auto feedbacks = std::make_unique<AbstractXmlElement>("Feedbacks");
    feedbacks->AddChild(std::make_unique<FeedbackLoader>(device.get()));

    auto feedbacksFolder = std::make_unique<FolderLoader>(device.get());
    feedbacksFolder->AddChild(std::make_unique<FeedbackLoader>(feedbacksFolder.get()));
    feedbacks->AddChild(std::move(feedbacksFolder));

    device->AddChild(std::move(commands));
    device->AddChild(std::move(feedbacks));

    AbstractXmlElement devices("Devices");
    devices.AddChild(std::move(device));
    devices.Parse(reader);

    return project;
}

void ShowFolderElem(const FolderElement& elem, std::size_t level = 0)
{
    std::cout << std::string(level * 4, ' ') << elem.Name().toStdString() << std::endl;

    auto pFolder = dynamic_cast<const Folder*>(&elem);
    if(pFolder == nullptr)
        return;

    for(auto&& child : pFolder->Children())
        ShowFolderElem(*child, level + 1);
}

void ShowProject(const Project& project)
{
    for(auto&& device : project.Devices())
    {
        std::cout << "Device: " << device->Name().toStdString() << '\n';
        ShowFolderElem(device->RootFolder(), 1);
        std::cout << std::endl;
    }
}
