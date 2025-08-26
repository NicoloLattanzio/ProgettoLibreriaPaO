#ifndef ADDITEMWIDGET_H
#define ADDITEMWIDGET_H

#include <QWidget>
#include <QVector>
#include <string> // <-- INCLUSIONE MANCANTE
#include "model/Item.h"
#include "model/Visitor.h"

// Definizione del tipo 'String'
using String = std::string;

// Forward declarations
class QLineEdit;
class QSpinBox;
class QCheckBox;
class QComboBox;
class QPushButton;

class ADDItemWidget : public QWidget, public ItemsVisitor::Visitor {
    Q_OBJECT // <-- Assicurati che Q_OBJECT sia presente

public:
    explicit ADDItemWidget(QWidget *parent = nullptr);
    ~ADDItemWidget();

    Library::Item* createItem();

    void visit(Library::Book& book) override;
    void visit(Library::Movie& movie) override;
    void visit(Library::CD& cd) override;

signals: // <-- SEZIONE MANCANTE O SCRITTA MALE
    void saveClicked();
    void cancelClicked();

private slots:
    void onTypeChanged(int index);

private:
    void setupCommonFields();
    void createSpecificFields();
    void showSpecificFields(int index);

    // Membri della UI
    QComboBox *m_typeComboBox;
    QLineEdit *m_titleEdit;
    QLineEdit *m_authorEdit;
    QLineEdit *m_genreEdit;
    QSpinBox *m_yearEdit;
    QLineEdit *m_imageEdit;

    // ... altri campi specifici ...
    QWidget *m_bookFields, *m_movieFields, *m_cdFields;
    QLineEdit *m_publisherEdit, *m_translatorEdit;
    QSpinBox *m_pagesEdit;
    QLineEdit *m_movieLanguageEdit;
    QLineEdit *m_movieTrailerEdit;
    QSpinBox *m_minAgeEdit, *m_durationMovieEdit;
    QCheckBox *m_oscarCheckBox;
    QLineEdit *m_cdLanguageEdit, *m_albumEdit;
    QSpinBox *m_durationCDEdit;
    QLineEdit *m_audioTrackEdit;
    QLineEdit *m_trailer;

    // Logica Interna
    QVector<Library::Item*> m_prototypes;
};

#endif // ADDITEMWIDGET_H
