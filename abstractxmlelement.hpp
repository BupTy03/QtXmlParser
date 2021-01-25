#pragma once
#include "xmlelement.hpp"
#include <QStringRef>


class AbstractXmlElement : public XmlElement
{
public:
    explicit AbstractXmlElement(QString name);

    void AddChild(std::unique_ptr<XmlElement> child) override;
    QString Name() const override;

protected:
    XmlElement* FindChildByName(QStringRef name) const;
    void ChildElement(QXmlStreamReader& reader) override;

private:
    QString name_;
    std::vector<std::unique_ptr<XmlElement>> children_;
};
