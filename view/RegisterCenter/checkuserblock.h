//
// Created by gouzuang on 25-7-13.
//

#ifndef CHECKUSERBLOCK_H
#define CHECKUSERBLOCK_H

#include <QLabel>
#include <QWidget>

namespace view::RegisterCenter {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class CheckUserBlock;
    }

    QT_END_NAMESPACE

    class CheckUserBlock : public QWidget {
        Q_OBJECT

    public:
        explicit CheckUserBlock(QWidget *parent = nullptr) = delete;

        explicit CheckUserBlock(int userId, QWidget *parent = nullptr);

        ~CheckUserBlock() override;

        void addCheckButton();


        void addStatusString();

        void setButtonStyle();

        void RejectCheck();

    public slots:
        void AcceptCheck();

    private:
        Ui::CheckUserBlock *ui;
        int userId;
        QPushButton acceptButton,
                rejectButton;
        QLabel label_status;

        void addStatusString(QString status);
    };
} // view::RegisterCenter

#endif //CHECKUSERBLOCK_H
