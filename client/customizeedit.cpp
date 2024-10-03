#include "customizeedit.h"

CustomizeEdit::CustomizeEdit(QWidget *parent)
{
    connect(this, &QLineEdit::textChanged, this, &CustomizeEdit::limitTextLength);
}

void CustomizeEdit::SetMaxLength(int maxLen)
{
    _max_len = maxLen;
}

void CustomizeEdit::focusOutEvent(QFocusEvent *event)
{
    // 调用基类的focusOutEvent()方法，保证基类的行为得到执行
    QLineEdit::focusOutEvent(event);
    //发送失去焦点得信号
    emit sig_foucus_out();
}

void CustomizeEdit::limitTextLength(QString text)
{
    if(_max_len <= 0){
        return;
    }
    QByteArray byteArray = text.toUtf8();
    if(byteArray.size() > _max_len){
        byteArray = byteArray.left(_max_len);
        this->setText(QString::fromUtf8(byteArray));
    }
}
