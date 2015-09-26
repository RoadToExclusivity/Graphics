#include "editcommand.h"

EditCommand::EditCommand(LanguageModel *model, unsigned int index, Language lang, bool isInsert)
    :m_model(model), m_index(index), m_lang(lang), m_isInsert(isInsert)
{
    auto indexRow = m_model->index(index, 0);
    auto indexCol = m_model->index(index, 1);
    m_prev = m_model->data(indexRow, Qt::DisplayRole);
}

void EditCommand::undo()
{
    auto indexLang = m_model->index(m_index, 0);
    auto indexPop = m_model->index(m_index, 1);
    if (m_isInsert)
    {
        m_model->removeRow(m_index);
    }
    else
    {

    }
}

void EditCommand::redo()
{
    auto indexLang = m_model->index(m_index, 0);
    auto indexPop = m_model->index(m_index, 1);
    if (m_isInsert)
    {
        m_model->append(m_lang);
        //m_model->insertRow(m_index, QModelIndex());

        //m_model->setData(indexLang, m_lang.GetLanguage());
        //m_model->setData(indexPop, m_lang.GetPopulation());
    }
    else
    {
        m_model->setData(indexLang, m_lang.GetLanguage());
        m_model->setData(indexPop, m_lang.GetPopulation());
    }
}
