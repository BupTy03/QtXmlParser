#pragma once
#include <QString>

#include <vector>
#include <memory>


class FolderElement
{
public:
    virtual ~FolderElement() = default;
    virtual QString Name() const = 0;
};

class Tag : public FolderElement
{
public:
    explicit Tag(QString name, int num);

    Tag(const Tag&) = delete;
    Tag& operator=(const Tag&) = delete;

    QString Name() const override;
    int Number() const;

    virtual bool IsCommand() const = 0;
    virtual bool IsFeedback() const = 0;

private:
    QString name_;
    int number_ = 0;
};


class Command : public Tag
{
public:
    explicit Command(QString name, int num);

    bool IsCommand() const override;
    bool IsFeedback() const override;
};

class Feedback : public Tag
{
public:
    explicit Feedback(QString name, int num);

    bool IsCommand() const override;
    bool IsFeedback() const override;
};

class Folder : public FolderElement
{
public:
    explicit Folder(QString name);

    QString Name() const override;
    const std::vector<std::unique_ptr<FolderElement>>& Children() const;
    void Add(std::unique_ptr<FolderElement> elem);
    bool Contains(const QString& name) const;

private:
    QString NextFreeName() const;
    QString DefaultName() const;
    QString DefaultNameNum(std::size_t n) const;

private:
    QString name_;
    std::vector<std::unique_ptr<FolderElement>> elements_;
};
