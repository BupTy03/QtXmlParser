#include "xmlelement.hpp"
#include "xmlelementslist.hpp"
#include "abstractxmlelement.hpp"

#include <QCoreApplication>
#include <QFile>

#include <iostream>
#include <vector>

struct Command
{
    explicit Command(QString name, int num) : Name(std::move(name)), Number(num) {}

    QString Name;
    int Number;
};

struct Feedback
{
    explicit Feedback(QString name, int num) : Name(std::move(name)), Number(num) {}

    QString Name;
    int Number;
};

struct Device
{
    Device() = default;
    explicit Device(QString Name) : Name(std::move(Name)) {}

    QString Name;
    std::vector<Command> Commands;
    std::vector<Feedback> Feedbacks;
};

struct Project
{
    std::vector<Device> Devices;
};

class CommandLoader : public XmlElement
{
public:
    explicit CommandLoader(Device* pDevice) : pDevice_(pDevice) {}

    QString Name() const override { return "Command"; }

protected:
    void BeginElement(const QXmlStreamReader& reader) override
    {
        pDevice_->Commands.emplace_back(reader.attributes().value("Name").toString(),
                                        reader.attributes().value("Number").toInt());
    }

private:
    Device* pDevice_;
};

class FeedbackLoader : public XmlElement
{
public:
    explicit FeedbackLoader(Device* pDevice) : pDevice_(pDevice) {}

    QString Name() const override { return "Feedback"; }

protected:
    void BeginElement(const QXmlStreamReader& reader) override
    {
        pDevice_->Feedbacks.emplace_back(reader.attributes().value("Name").toString(),
                                        reader.attributes().value("Number").toInt());
    }

private:
    Device* pDevice_;
};

class DeviceLoader : public AbstractXmlElement
{
public:
    explicit DeviceLoader(Project* pProject) : pProject_(pProject)
    {
        AddChild(std::make_unique<XmlElementsList>("Commands", std::make_unique<CommandLoader>(&device_)));
        AddChild(std::make_unique<XmlElementsList>("Feedbacks", std::make_unique<FeedbackLoader>(&device_)));
    }

    QString Name() const override { return "Device"; }

    void BeginElement(const QXmlStreamReader& reader) override
    {
        device_ = Device();
        device_.Name = reader.attributes().value("Name").toString();
    }

    void EndElement() override
    {
        pProject_->Devices.emplace_back(device_);
    }

private:
    Device device_;
    Project* pProject_;
};

Project LoadProject(QXmlStreamReader& reader)
{
    reader.readNext();
    reader.readNext();

    Project project;
    XmlElementsList devices("Devices", std::make_unique<DeviceLoader>(&project));
    devices.Parse(reader);

    return project;
}

void ShowProject(const Project& project)
{
    for(auto&& device : project.Devices)
    {
        std::cout << "Device: " << device.Name.toStdString() << std::endl;
        std::cout << "    Commands:" << std::endl;
        for(auto&& command : device.Commands)
            std::cout << "        "  << command.Number << ' ' << command.Name.toStdString() << std::endl;

        std::cout << "    Feedbacks:" << std::endl;
        for(auto&& feedback : device.Feedbacks)
            std::cout << "        "  << feedback.Number << ' ' << feedback.Name.toStdString() << std::endl;
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("../QtXmlParser/example.xml");
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        std::cerr << "Unable to open file\n";
        return 0;
    }

    QXmlStreamReader reader(&file);
    const auto project = LoadProject(reader);
    ShowProject(project);
    return a.exec();
}
