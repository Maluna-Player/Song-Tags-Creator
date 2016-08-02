#ifndef CHOICEDIALOG
#define CHOICEDIALOG

#include <QDialog>
#include <QVector>
#include <QRadioButton>
#include "SongFile.h"

class ChoiceDialog : public QDialog
{
    private:

        static const QString radioSeparator;

        QVector<QRadioButton*> m_choices;

    public:

        ChoiceDialog(const SongFile& song, const QString& separator, QWidget *parent = 0);
        virtual ~ChoiceDialog();

        QPair<QString,QString> getSelectedChoice();
};

#endif // CHOICEDIALOG

