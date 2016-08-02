#include "ChoiceDialog.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>


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

    unsigned int row { 4 };
    auto choices = song.getChoices(separator);
    for (const auto& choice : choices)
    {
        m_choices.append(new QRadioButton(choice.first + radioSeparator + choice.second));
        layout->addWidget(m_choices.last(), row, 0, 1, 3);
        ++row;
    }

    m_choices.first()->setChecked(true);

    QPushButton *validateButton = new QPushButton("Valider");
    QPushButton *ignoreButton = new QPushButton("Ignorer");

    layout->setRowMinimumHeight(row, 50);
    layout->addWidget(validateButton, row+1, 1);
    layout->addWidget(ignoreButton, row+1, 2);

    setLayout(layout);

    connect(validateButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ignoreButton, SIGNAL(clicked()), this, SLOT(reject()));
}

ChoiceDialog::~ChoiceDialog()
{

}

QPair<QString,QString> ChoiceDialog::getSelectedChoice()
{
    if (exec() == QDialog::Accepted)
    {
        for (QRadioButton *button : m_choices)
        {
            if (button->isChecked())
            {
                auto elements = button->text().split(radioSeparator);
                return qMakePair(elements[0], elements[1]);
            }
        }

    }

    return QPair<QString,QString>("", "");
}
