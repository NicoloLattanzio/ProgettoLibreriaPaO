#ifndef EDITITEMWIDGET_H
#define EDITITEMWIDGET_H

#include <QWidget>
#include <QVector>
#include <string>
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

class EditItemWidget : public QWidget, public ItemsVisitor::ConstVisitor {
    Q_OBJECT

public:
    explicit EditItemWidget(QWidget *parent = nullptr);
    ~EditItemWidget();

    void setItem(const Library::Item* item);
    Library::Item* getUpdatedItem() const;

    // Visitor pattern implementation
    void visit(const Library::Book& book);
    void visit(const Library::Movie& movie);
    void visit(const Library::CD& cd);

signals:
    void saveClicked();
    void cancelClicked();

private slots:
    void onTypeChanged(int index);
    void onSaveButtonClicked();
    void onCancelButtonClicked();

private:
    void setupUI();
    void createSpecificFields();
    void populateCommonFields();
    void showSpecificFields(int index);

    // Membri della UI
    QComboBox *m_typeComboBox;
    QLineEdit *m_titleEdit;
    QLineEdit *m_authorEdit;
    QLineEdit *m_genreEdit;
    QSpinBox *m_yearEdit;
    QLineEdit *m_imageEdit;

    // Campi specifici per libro
    QWidget *m_bookFields;
    QLineEdit *m_publisherEdit;
    QLineEdit *m_translatorEdit;
    QSpinBox *m_pagesEdit;

    // Campi specifici per film
    QWidget *m_movieFields;
    QLineEdit *m_movieLanguageEdit;
    QSpinBox *m_minAgeEdit;
    QSpinBox *m_durationMovieEdit;
    QCheckBox *m_oscarCheckBox;
    QLineEdit *m_trailer;
    QLineEdit *m_movieTrailerEdit;

    // Campi specifici per CD
    QWidget *m_cdFields;
    QLineEdit *m_cdLanguageEdit;
    QLineEdit *m_albumEdit;
    QSpinBox *m_durationCDEdit;
    QLineEdit *m_audioTrackEdit;

    // Pulsanti
    QPushButton *m_saveButton;
    QPushButton *m_cancelButton;

    // Logica Interna
    QVector<Library::Item*> m_prototypes;
    const Library::Item* m_currentItem;
    int m_currentItemId;
};

#endif // EDITITEMWIDGET_H
