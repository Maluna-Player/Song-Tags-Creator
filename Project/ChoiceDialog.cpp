#include "ChoiceDialog.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>


const QString ChoiceDialog::radioSeparator = "\t\t\t\t\t\t\t\t\t\t";

ChoiceDialog::ChoiceDialog(const SongFile& song, const QString& separator, QWidget *parent)
    : QDialog(parent)
{
    QGridLayout *layout = new QGridLayout;
    layout->setRowMinimumHeight(1, 35);
    layout->setRowMinimumHeight(3, 25);
    layout->setColumnMinimumWidth(0, 60);

    layout->addWidget(new QLabel("<b>Filename</b> : ", 0, 0));
    layout->addWidget(new QLabel(song.getFilename()), 0, 1, 1, 2, Qt::AlignCenter);
    layout->addWidget(new QLabel("Author"), 2, 1);
    layout->addWidget(new QLabel("Title"), 2, 2);

    mp_choicesGroup = new QButtonGroup;

    QPushButton *validateButton = new QPushButton("Valider");
    QPushButton *ignoreButton = new QPushButton("Ignorer");

    layout->addLayout(createChoicesLayout(song.getChoices(separator)), 4, 0, 1, 3);
    layout->setRowMinimumHeight(5, 50);
    layout->addWidget(validateButton, 6, 1);
    layout->addWidget(ignoreButton, 6, 2);

    setLayout(layout);

    connect(validateButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ignoreButton, SIGNAL(clicked()), this, SLOT(reject()));
}

ChoiceDialog::~ChoiceDialog()
{

}

QVBoxLayout* ChoiceDialog::createChoicesLayout(const QList<Choice_t>& choices)
{
    QVBoxLayout *choicesLayout = new QVBoxLayout;

    unsigned int row { 1 };
    for (const auto& choice : choices)
    {
        QRadioButton *button = new QRadioButton(choice.first + radioSeparator + choice.second);
        choicesLayout->addWidget(button);
        mp_choicesGroup->addButton(button, row);

        ++row;
    }

    QRadioButton *button = new QRadioButton("Autre");
    choicesLayout->addWidget(button);
    mp_choicesGroup->addButton(button, 0);

    if (mp_choicesGroup->button(1))
        mp_choicesGroup->button(1)->setChecked(true);
    else
        mp_choicesGroup->button(0)->setChecked(true);

    QHBoxLayout *customChoiceLayout = new QHBoxLayout;
    mp_authorEdit = new QLineEdit;
    mp_titleEdit = new QLineEdit;
    customChoiceLayout->addWidget(mp_authorEdit);
    customChoiceLayout->addWidget(mp_titleEdit);

    choicesLayout->addLayout(customChoiceLayout);

    return choicesLayout;
}

Choice_t ChoiceDialog::getSelectedChoice()
{
    if (exec() == QDialog::Accepted)
    {
        auto id { mp_choicesGroup->checkedId() };
        if (id > 0)
        {
            auto elements = mp_choicesGroup->button(id)->text().split(radioSeparator);
            return qMakePair(elements[0], elements[1]);
        }
        else if (id == 0)
            return qMakePair(mp_authorEdit->text(), mp_titleEdit->text());
    }

    return Choice_t("", "");
}
