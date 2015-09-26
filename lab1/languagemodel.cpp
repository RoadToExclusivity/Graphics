#include "languagemodel.h"
#include "editcommand.h"
#include <QAction>

LanguageModel::LanguageModel(QUndoStack *stack, QAction *save,QObject *parent) :
       QAbstractTableModel(parent), wasChanged(false), m_stack(stack), m_save(save)
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

    wasChanged = true;
    m_save->setEnabled(true);
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
        auto language = m_data[index.row()].GetLanguage();
        auto population = m_data[index.row()].GetPopulation();
        Language newLang;
        if (index.column() == 0)
        {
            if (value.toString().length() == 0)
            {
                return false;
            }

            auto newLanguage = value.toString();
            if (language != newLanguage)
            {
                wasChanged = true;
                m_save->setEnabled(true);
            }

            newLang = Language(newLanguage, population);
        }
        else
        {
            unsigned int newPop = value.value<unsigned int>();
            if (population != newPop)
            {
                wasChanged = true;
                m_save->setEnabled(true);
            }

            newLang = Language(language, newPop);
        }

        m_data.replace(index.row(), newLang);
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

void LanguageModel::clear()
{
    m_data.clear();
    layoutChanged();

    wasChanged = true;
    m_save->setEnabled(true);
}

void LanguageModel::insert(const Language &language, int index)
{

}

void LanguageModel::remove(int index)
{

}
