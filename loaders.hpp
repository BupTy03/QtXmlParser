#pragma once
#include "xmlelement.hpp"
#include "abstractxmlelement.hpp"
#include "project.hpp"
#include "folder.hpp"

#include <memory>


class DeviceLoader;

class RootFolder
{
public:
    virtual ~RootFolder();
    virtual void AddFolderElement(std::unique_ptr<FolderElement>) = 0;
};


class FolderLoader : public AbstractXmlElement, public RootFolder
{
public:
    explicit FolderLoader(RootFolder* pParent = nullptr);
    void AddFolderElement(std::unique_ptr<FolderElement> elem) override;

protected:
    void BeginElement(const QXmlStreamReader& reader) override;
    void EndElement() override;

private:
    RootFolder* pParent_;
    std::unique_ptr<Folder> pCurrentFolder_;
};


class CommandLoader : public XmlElement
{
public:
    explicit CommandLoader(RootFolder* pFolder);
    QString Name() const override;

protected:
    void BeginElement(const QXmlStreamReader& reader) override;

private:
    RootFolder* pFolder_;
};

class FeedbackLoader : public XmlElement
{
public:
    explicit FeedbackLoader(RootFolder* pFolder);
    QString Name() const override;

protected:
    void BeginElement(const QXmlStreamReader& reader) override;

private:
    RootFolder* pFolder_;
};

class DeviceLoader : public AbstractXmlElement, public RootFolder
{
public:
    explicit DeviceLoader(Project* pProject);

    QString Name() const override;
    void AddFolderElement(std::unique_ptr<FolderElement> elem) override;

protected:
    void BeginElement(const QXmlStreamReader& reader) override;
    void EndElement() override;

private:
    Project& project_;
    QString deviceName_;
    std::unique_ptr<Folder> pDeviceRootFolder_;
};
