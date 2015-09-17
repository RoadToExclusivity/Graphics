#include "language.h"

Language::Language()
    :m_language(""), m_population(0)
{
}

Language::Language(const QString &lang, unsigned int population)
    :m_language(lang), m_population(population)
{
}

QString Language::GetLanguage() const
{
    return m_language;
}

unsigned int Language::GetPopulation() const
{
    return m_population;
}

