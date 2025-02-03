#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtWidgets/QMainWindow>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

    void insertData();
    void glavMenu();
    void loadtotable();
    void loadSearchOptions();
    void filterTableBySearch();
    void sortTableByColumn();
    void loadSortOptions();
    void TableWidgetRowClicked(int row, int column);
    void loadCartDetails(int orderId);
    void deleteRecord();
    void new_bludo();
    void glawbluda();
    void knopkadob();
    void loadOptions();
    void loadCategories();
    void addNewFood();
    void cancelAddproduct();
    void drfghj();
    void food_close();
    void product_close();
    void showProvider();
    void provider_close();
    void addProvider();
    void addProvider_close();
    void addProviderDB();
    void loadProviders();
    void fillProductsTable();
    void TableWidgetRowClicked1(int row, int column);
    void loadFoodDetails(int orderId);
    void on_pushButton_3_clicked();
    void checkAndConsumeIngredients(int food_id);
    void closeUpdateProduct();
    void loadProvToCB();
    void updateProduct();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "qwerty");
    QList<QPair<QString, int>> getSelectedProducts();
    int currentID;
};
#endif // MAINWINDOW_H
