#pragma once
#include <vector>
#include <memory>

#include <QString>
#include <QXmlStreamReader>


class XmlElement
{
public:
    XmlElement();
    virtual ~XmlElement();

    XmlElement(const XmlElement&) = delete;
    XmlElement& operator=(const XmlElement&) = delete;

    void Parse(QXmlStreamReader& reader);
    virtual QString Name() const = 0;
    virtual void AddChild(std::unique_ptr<XmlElement>);

protected:
    virtual void BeginElement(const QXmlStreamReader&);
    virtual void ChildElement(QXmlStreamReader&);
    virtual void EndElement();
};
