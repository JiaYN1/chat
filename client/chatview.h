#ifndef CHATVIEW_H
#define CHATVIEW_H

#include <QWidget>
#include <QLayout>
#include <QScrollArea>
#include <QVBoxLayout>

class ChatView : public QWidget
{
    Q_OBJECT
public:
    explicit ChatView(QWidget *parent = nullptr);
    void appendChatItem(QWidget *item);
    void prependChatItem(QWidget *item);
    void insertChatItem(QWidget *before, QWidget *item);//中间插
protected:
    bool eventFilter(QObject *o, QEvent *e) override;
    void paintEvent(QPaintEvent *event) override;
private slots:
    void onVScrollBarMoved(int min, int max);
private:
    void initStyleSheet();
private:
    //QWidget *m_pCenterWidget;
    QVBoxLayout *m_pVl;
    QScrollArea *m_pScrollArea;
    bool isAppended;
};

#endif // CHATVIEW_H
