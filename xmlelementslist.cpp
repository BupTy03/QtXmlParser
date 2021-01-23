#include "xmlelementslist.hpp"
#include <QDebug>
#include <stdexcept>


XmlElementsList::XmlElementsList(QString name, std::unique_ptr<XmlElement> item)
    : name_(std::move(name))
    , item_(item.release())
{
}

QString XmlElementsList::Name() const
{
    return name_;
}

void XmlElementsList::ChildElement(QXmlStreamReader& reader)
{
    item_->Parse(reader);
}

void XmlElementsList::AddChild(std::unique_ptr<XmlElement> child)
{
    item_= std::move(child);
}

