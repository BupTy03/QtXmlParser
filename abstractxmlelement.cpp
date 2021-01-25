#include "abstractxmlelement.hpp"

#include <algorithm>
#include <cassert>
#include <stdexcept>


AbstractXmlElement::AbstractXmlElement(QString name)
    : name_(std::move(name))
{
}

XmlElement* AbstractXmlElement::FindChildByName(QStringRef name) const
{
    auto it = std::find_if(std::cbegin(children_), std::cend(children_),
                           [&](auto&& p){ return p->Name() == name; });

    if(it == std::cend(children_))
        return nullptr;

    return it->get();
}

void AbstractXmlElement::ChildElement(QXmlStreamReader& reader)
{
    assert(reader.isStartElement());
    auto child = FindChildByName(reader.name());
    if(child == nullptr)
        throw std::runtime_error("Unknown xml element");

    child->Parse(reader);
}

void AbstractXmlElement::AddChild(std::unique_ptr<XmlElement> child)
{
    const auto name = child->Name();
    if(FindChildByName(QStringRef(&name)) != nullptr)
        return;

    children_.emplace_back(std::move(child));
}

QString AbstractXmlElement::Name() const
{
    return name_;
}
