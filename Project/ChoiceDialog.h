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
    Q_OBJECT

    private:

        static const QString radioSeparator;

        QButtonGroup *mp_choicesGroup;

        QLineEdit *mp_titleEdit;

        QLineEdit *mp_authorEdit;

        Choice_t m_choice;

        bool m_ignoreAll;


        QVBoxLayout* createChoicesLayout(const QList<Choice_t>& choices);

        void execute();

    private slots:

        void ignoreAll();

        void reverseChoices();

    public:

        ChoiceDialog(const SongFile& song, const QString& separator, QWidget *parent = 0);
        virtual ~ChoiceDialog();

        bool allIgnored() const;

        Choice_t getSelectedChoice();
};

#endif // CHOICEDIALOG

