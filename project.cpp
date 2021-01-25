#include "project.hpp"
#include "abstractxmlelement.hpp"
#include "loaders.hpp"

#include <QXmlStreamReader>

#include <iostream>
#include <stdexcept>


Device::Device(QString Name)
    : name_(std::move(Name))
    , rootFolder_("Root")
{}

QString Device::Name() const { return name_; }

Folder& Device::RootFolder() { return rootFolder_; }

const Folder& Device::RootFolder() const { return rootFolder_; }

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

    AbstractXmlElement devices("Devices");
    devices.AddChild(std::move(device));
    devices.Parse(reader);

    return project;
}

void ShowProject(const Project& project)
{
    for(auto&& device : project.Devices())
    {
        std::cout << "Device: " << device->Name().toStdString() << '\n';
        for(auto&& child : device->RootFolder().Children())
            std::cout << "    Child: \"" << child->Name().toStdString() << "\"\n";

        std::cout << std::endl;
    }
}
