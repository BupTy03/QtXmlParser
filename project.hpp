#pragma once
#include "folder.hpp"
#include <memory>


class Device
{
public:
    explicit Device(QString Name);

    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;

    QString Name() const;

    Folder& RootFolder();
    const Folder& RootFolder() const;

private:
    QString name_;
    Folder rootFolder_;
};

class Project
{
public:
    Project() = default;

    Project(const Project&) = delete;
    Project& operator=(const Project&) = delete;

    void Add(std::unique_ptr<Device> d);
    bool Contains(const QString& name) const;

    const std::vector<std::unique_ptr<Device>>& Devices() const;

private:
    std::vector<std::unique_ptr<Device>> devices_;
};

class QXmlStreamReader;

std::unique_ptr<Project> LoadProject(QXmlStreamReader& reader);
void ShowProject(const Project& project);

