#pragma once
#include "xmlelement.hpp"
#include "abstractxmlelement.hpp"
#include "project.hpp"


class DeviceLoader;

class CommandLoader : public XmlElement
{
public:
    explicit CommandLoader(DeviceLoader* pDeviceLoader);
    QString Name() const override;

protected:
    void BeginElement(const QXmlStreamReader& reader) override;

private:
    DeviceLoader& parent_;
};

class FeedbackLoader : public XmlElement
{
public:
    explicit FeedbackLoader(DeviceLoader* pParent);
    QString Name() const override;

protected:
    void BeginElement(const QXmlStreamReader& reader) override;

private:
    DeviceLoader& parent_;
};

class DeviceLoader : public AbstractXmlElement
{
public:
    explicit DeviceLoader(Project* pProject);

    QString Name() const override;
    void AddFolderElement(std::unique_ptr<FolderElement> elem);

protected:
    void BeginElement(const QXmlStreamReader& reader) override;
    void EndElement() override;

private:
    Project& project_;
    std::unique_ptr<Device> pDevice_;
};
