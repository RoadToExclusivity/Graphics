#include "editcommand.h"

EditCommand::EditCommand(LanguageModel *model, unsigned int index, Language lang, bool isInsert)
    :m_model(model), m_index(index), m_lang(lang), m_isInsert(isInsert)
{
    if (index >= m_model->rowCount(QModelIndex()))
    {
        m_prev = Language();
    }
    else
    {
        auto indexRow = m_model->index(index, 0);
        auto indexCol = m_model->index(index, 1);

        auto newLang = m_model->data(indexRow, Qt::DisplayRole).toString();
        auto pop = m_model->data(indexCol, Qt::DisplayRole).toInt();
        m_prev = Language(newLang, pop);
    }
}

void EditCommand::undo()
{
    if (m_isInsert)
    {
        m_model->RemoveLast();
    }
    else
    {
        auto indexLang = m_model->index(m_index, 0);
        auto indexPop = m_model->index(m_index, 1);

        m_model->ChangeData(indexLang, indexPop, m_prev);
    }
}

void EditCommand::redo()
{
    if (m_isInsert)
    {
        m_model->InsertLast(m_lang);
    }
    else
    {
        auto indexLang = m_model->index(m_index, 0);
        auto indexPop = m_model->index(m_index, 1);

        m_model->ChangeData(indexLang, indexPop, m_lang);
    }
}
