#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QString>

class Language
{
public:
    Language();
    Language(const QString &lang, unsigned int population);
    QString GetLanguage() const;
    unsigned int GetPopulation() const;

private:
    QString m_language;
    unsigned int m_population;
};

//Q_DECLARE_METATYPE(Language);
#endif // LANGUAGE_H
