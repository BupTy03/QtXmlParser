#include "folder.hpp"
#include <stdexcept>


Tag::Tag(QString name, int num) : name_(std::move(name)), number_(num) {}

QString Tag::Name() const { return name_; }

int Tag::Number() const { return number_; }

Command::Command(QString name, int num) : Tag(std::move(name), num) {}

bool Command::IsCommand() const { return true; }

bool Command::IsFeedback() const { return false; }

Feedback::Feedback(QString name, int num) : Tag(std::move(name), num) {}

bool Feedback::IsCommand() const { return false; }

bool Feedback::IsFeedback() const { return true; }

Folder::Folder(QString name) : name_(std::move(name)), elements_() {}

QString Folder::Name() const { return name_; }

const std::vector<std::unique_ptr<FolderElement> > &Folder::Children() const { return elements_; }

void Folder::Add(std::unique_ptr<FolderElement> elem)
{
    if(Contains(elem->Name()))
        throw std::runtime_error("Folder already contains element with name \"" + elem->Name().toStdString() + "\"");

    elements_.emplace_back(std::move(elem));
}

bool Folder::Contains(const QString &name) const
{
    return std::find_if(std::begin(elements_), std::end(elements_),
                        [&](auto&& e){ return e->Name() == name; }) != std::end(elements_);
}

QString Folder::NextFreeName() const
{
    QString name;
    for(std::size_t num = 1; num < std::numeric_limits<std::size_t>::max(); ++num)
    {
        name = DefaultNameNum(num);
        if(!Contains(name))
            break;
    }

    assert(!Contains(name));
    return name;
}

QString Folder::DefaultName() const
{
    return "Folder";
}

QString Folder::DefaultNameNum(size_t n) const
{
    return DefaultName() + QString::number(n);
}
