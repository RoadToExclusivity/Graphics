#ifndef LANGUAGEMODEL_H
#define LANGUAGEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "language.h"

class LanguageModel : public QAbstractTableModel
{
public:
    LanguageModel(QObject *parent = 0);
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void append(const Language &language);
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

private:
    QList<Language> m_data;
};

#endif // LANGUAGEMODEL_H
