#ifndef PAGE_H
#define PAGE_H

#include <QWidget>
#include <QSharedPointer>

class BasicWidget;
class MainWindow;
class WeatherData;

class Page : public QWidget
{
    Q_OBJECT

public:
    virtual ~Page() = default;

    inline int numWidgets() const
    {
        return m_widgets.size();
    }

public slots:
    virtual void addNewWidget(const QSharedPointer<WeatherData> data) = 0;
    virtual void addErrorWidget(const QString &errMsg) = 0;
    void deleteWidgets();

protected:
    explicit Page(QWidget *parent = nullptr);

    QVector<BasicWidget*> m_widgets;
    MainWindow *mainWindow;
};

#endif // PAGE_H
