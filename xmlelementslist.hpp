#pragma once
#include "xmlelement.hpp"


class XmlElementsList : public XmlElement
{
public:
    explicit XmlElementsList(QString name, std::unique_ptr<XmlElement> item = {});
    QString Name() const override;

protected:
    void ChildElement(QXmlStreamReader& reader) override;
    void AddChild(std::unique_ptr<XmlElement> child) override;

private:
    QString name_;
    std::unique_ptr<XmlElement> item_;
};

