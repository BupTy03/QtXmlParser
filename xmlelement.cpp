#pragma once
#include "xmlelement.hpp"
#include <stdexcept>


XmlElement::XmlElement()
{
}

XmlElement::~XmlElement()
{
}

void XmlElement::Parse(QXmlStreamReader &reader)
{
    if(!(reader.isStartElement() && reader.name() == Name()))
        throw std::runtime_error("Invalid xml element");

    BeginElement(reader);
    reader.readNext();
    while(!reader.atEnd())
    {
        if(reader.isStartElement())
        {
            ChildElement(reader);
        }
        else if(reader.isEndElement() && reader.name() == Name())
        {
            EndElement();
            return;
        }

        reader.readNext();
    }

    throw std::runtime_error("End element not found");
}

void XmlElement::AddChild(std::unique_ptr<XmlElement>)
{
}

void XmlElement::BeginElement(const QXmlStreamReader&)
{
}

void XmlElement::ChildElement(QXmlStreamReader&)
{
    throw std::runtime_error("Element hasn't chilren");
}

void XmlElement::EndElement()
{
}


