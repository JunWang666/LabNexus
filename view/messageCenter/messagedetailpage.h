//
// Created by gouzuang on 25-7-9.
//

#ifndef MESSAGEDETAILPAGE_H
#define MESSAGEDETAILPAGE_H

#include <QWidget>

namespace view::messageCenter {
    QT_BEGIN_NAMESPACE

    namespace Ui {
        class MessageDetailPage;
    }

    QT_END_NAMESPACE

    class MessageDetailPage : public QWidget {
        Q_OBJECT

    public:
        explicit MessageDetailPage(QWidget *parent = nullptr);

        explicit MessageDetailPage(int messageId, QWidget *parent = nullptr);

        ~MessageDetailPage() override;

    private:
        Ui::MessageDetailPage *ui;
    };
} // view::messageCenter

#endif //MESSAGEDETAILPAGE_H
