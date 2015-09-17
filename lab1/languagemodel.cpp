#include "languagemodel.h"

LanguageModel::LanguageModel(QObject *parent) :
       QAbstractTableModel(parent)
{
}

int LanguageModel::rowCount(const QModelIndex&) const
{
    return m_data.count();
}

int LanguageModel::columnCount(const QModelIndex&) const
{
    return 2;
}

QVariant LanguageModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    const Language &lang = m_data[index.row()];
    switch (index.column())
    {
        case 0: return lang.GetLanguage();
        case 1: return lang.GetPopulation();
        default: return QVariant();
    };
}

QVariant LanguageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal)
        return QVariant();
    if (role != Qt::DisplayRole)
        return QVariant();

    switch (section)
    {
        case 0: return "Language";
        case 1: return "Population";
        default: return QVariant();
    }
}

void LanguageModel::append(const Language &lang)
{
    beginInsertRows(QModelIndex(), m_data.count(), m_data.count());
    m_data.append(lang);
    endInsertRows();
}

Qt::ItemFlags LanguageModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::ItemIsEnabled;
    }

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool LanguageModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        auto pp = &value;
        m_data.replace(index.row(), qvariant_cast<Language>(value));
        emit dataChanged(index, index);
        return true;
    }

    return false;
}
