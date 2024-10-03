#include "statewidget.h"
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>

StateWidget::StateWidget(QWidget *parent)
    : QWidget{parent}, _curState(ClickLbState::Normal)
{
    setCursor(Qt::PointingHandCursor);
    AddRedPoint();
}

void StateWidget::SetState(QString normal, QString hover, QString press, QString select, QString select_hover, QString select_press)
{
    _normal = normal;
    _normal_press = press;
    _normal_hover = hover;

    _selected = select;
    _selected_hover = select_hover;
    _selected_press = select_press;

    setProperty("state", _normal);
    repolish(this);
}

ClickLbState StateWidget::GetCurState()
{
    return _curState;
}

void StateWidget::ClearState()
{
    _curState = ClickLbState::Normal;
    setProperty("state", _normal);
    repolish(this);
    update();
}

void StateWidget::SetSelected(bool bselected)
{
    if(bselected){
        _curState = ClickLbState::Selected;
        setProperty("state", _selected);
        repolish(this);
        update();
        return;
    }

    _curState = ClickLbState::Normal;
    setProperty("state",_normal);
    repolish(this);
    update();
    return;
}

void StateWidget::AddRedPoint()
{
    //添加红点示意图
    _red_point = new QLabel();
    _red_point->setObjectName("red_point");
    QVBoxLayout* layout2 = new QVBoxLayout;
    _red_point->setAlignment(Qt::AlignCenter);
    layout2->addWidget(_red_point);
    layout2->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout2);
    _red_point->setVisible(false);
}

void StateWidget::ShowRedPoint()
{
    _red_point->setVisible(true);
}

void StateWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    return;
}

void StateWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(_curState == ClickLbState::Selected) {
            qDebug() << "PressEvent , already to selected press: "<< _selected_press;
            // 调用基类的mousePressEvent以保证正常的事件处理
            QWidget::mousePressEvent(event);
            return;
        }

        if(_curState == ClickLbState::Normal) {
            qDebug()<<"PressEvent , change to selected press: "<< _selected_press;
            _curState = ClickLbState::Selected;
            setProperty("state",_selected_press);
            repolish(this);
            update();
        }
    }
    QWidget::mousePressEvent(event);
}

void StateWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        if(_curState == ClickLbState::Normal) {
            qDebug()<<"ReleaseEvent , change to normal hover: "<< _normal_hover;
            setProperty("state", _normal_hover);
            repolish(this);
            update();
        }else {
            qDebug()<<"ReleaseEvent , change to select hover: "<< _selected_hover;
            setProperty("state",_selected_hover);
            repolish(this);
            update();
        }
        emit clicked();
        return;
    }
    // 调用基类的mousePressEvent以保证正常的事件处理
    QWidget::mousePressEvent(event);
}

void StateWidget::enterEvent(QEnterEvent *event)
{
    if(_curState == ClickLbState::Normal){
        qDebug()<<"enter , change to normal hover: "<< _normal_hover;
        setProperty("state",_normal_hover);
        repolish(this);
        update();

    }else{
        qDebug()<<"enter , change to selected hover: "<< _selected_hover;
        setProperty("state",_selected_hover);
        repolish(this);
        update();
    }

    QWidget::enterEvent(event);
}

void StateWidget::leaveEvent(QEvent *event)
{
    if(_curState == ClickLbState::Normal){
        // qDebug()<<"leave , change to normal : "<< _normal;
        setProperty("state",_normal);
        repolish(this);
        update();

    }else{
        // qDebug()<<"leave , change to select normal : "<< _selected;
        setProperty("state",_selected);
        repolish(this);
        update();
    }
    QWidget::leaveEvent(event);
}
