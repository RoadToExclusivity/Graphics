#ifndef EDITCOMMAND_H
#define EDITCOMMAND_H

#include <QUndoCommand>
#include <QTableView>
#include "language.h"
#include "languagemodel.h"

class EditCommand : public QUndoCommand
{
public:
    EditCommand(LanguageModel *model, unsigned int index, Language lang, bool isInsert);

public slots:
    void redo();
    void undo();
private:
    unsigned int m_index;
    Language m_lang, m_prev;
    bool m_isInsert;
    LanguageModel *m_model;
};

#endif // EDITCOMMAND_H
