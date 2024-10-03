#ifndef SEARCHLIST_H
#define SEARCHLIST_H

#include <QListWidget>
#include <QWidget>
#include <QObject>
#include <QEvent>
#include <QWheelEvent>
#include "loadingdlg.h"
#include "userdata.h"

class SearchList : public QListWidget
{
    Q_OBJECT
public:
    SearchList(QWidget* parent = nullptr);
    void CloseFindDlg();
    void SetSearchEdit(QWidget* edit);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void waitPending(bool pending = true);
    bool _send_pending;
    void addTipItem();
    std::shared_ptr<QDialog> _find_dlg;
    QWidget* _search_edit;
    LoadingDlg* _loadingDialog;

private slots:
    void slot_item_clicked(QListWidgetItem* item);
    void slot_user_search(std::shared_ptr<SearchInfo> si);
};

#endif // SEARCHLIST_H
