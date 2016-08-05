#ifndef CHOICEDIALOG
#define CHOICEDIALOG

#include <QDialog>
#include <QVector>
#include <QButtonGroup>
#include <QLineEdit>
#include <QVBoxLayout>
#include "SongFile.h"

class ChoiceDialog : public QDialog
{
    private:

        static const QString radioSeparator;

        QButtonGroup *mp_choicesGroup;

        QLineEdit *mp_titleEdit;

        QLineEdit *mp_authorEdit;


        QVBoxLayout* createChoicesLayout(const QList<Choice_t>& choices);

    public:

        ChoiceDialog(const SongFile& song, const QString& separator, QWidget *parent = 0);
        virtual ~ChoiceDialog();

        Choice_t getSelectedChoice();
};

#endif // CHOICEDIALOG

