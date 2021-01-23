#pragma once
#include "xmlelement.hpp"

#include <QStringRef>

class AbstractXmlElement : public XmlElement
{
public:
    void AddChild(std::unique_ptr<XmlElement> child) override;

protected:
    XmlElement* FindChildByName(QStringRef name) const;
    void ChildElement(QXmlStreamReader& reader) override;

private:
    std::vector<std::unique_ptr<XmlElement>> children_;
};
