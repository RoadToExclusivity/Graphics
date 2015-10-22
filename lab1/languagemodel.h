#ifndef LANGUAGEMODEL_H
#define LANGUAGEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "language.h"
#include <QUndoStack>

class LanguageModel : public QAbstractTableModel
{
    Q_OBJECT;
public:
    LanguageModel(QUndoStack *stack, QAction *save, QObject *parent = 0);
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void append(const Language &language);
    void InsertLast(const Language &language);
    void RemoveLast();
    void ChangeData(const QModelIndex &indexRow, const QModelIndex &indexCol, const Language &lang);
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    void clear();

    bool wasChanged;
private:
    QList<Language> m_data;
    QUndoStack *m_stack;
    QAction *m_save;
};

#endif // LANGUAGEMODEL_H
