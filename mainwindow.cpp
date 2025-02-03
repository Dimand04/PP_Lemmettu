#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global.h"
#include <QDebug>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnWidth(0,105);
    ui->tableWidget->setColumnWidth(1,207);
    ui->tableWidget->setColumnWidth(2,202);
    ui->tableWidget->setColumnWidth(3,207);
    ui->tableWidget->setColumnWidth(4,110);
    ui->tableWidget->setColumnWidth(5,110);
    ui->tableWidget->setColumnWidth(5,207);
    ui->tw_bluda->setColumnWidth(0,90);
    ui->tw_bluda->setColumnWidth(1,244);
    ui->tw_bluda->setColumnWidth(2,214);
    ui->tw_bluda->setColumnWidth(3,500);
    ui->tableWidget_2->setColumnWidth(0,10);
    ui->tableWidget_2->setColumnWidth(1,200);
    ui->tableWidget_2->setColumnWidth(2,200);
    ui->tableWidget_2->setColumnWidth(3,250);
    ui->tableWidget_2->setColumnWidth(4,580);

    connect (ui->dobavit,&QPushButton::clicked,this, &MainWindow::insertData);
    connect (ui->qpb_newpr,&QPushButton::clicked,this, &MainWindow::glavMenu);
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::filterTableBySearch);
    connect(ui->lineEdit_7, &QLineEdit::textChanged, this, &MainWindow::filterTableBySearch);
    connect(ui->cb_sort, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::sortTableByColumn);
    connect(ui->rb_up, &QRadioButton::toggled, this, &MainWindow::sortTableByColumn);
    connect(ui->rb_down, &QRadioButton::toggled, this, &MainWindow::sortTableByColumn);
    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &MainWindow::TableWidgetRowClicked);
    connect(ui->pushButton_2, &QPushButton::clicked,this, &MainWindow::deleteRecord);
    connect (ui->qpb_bluda,&QPushButton::clicked,this, &MainWindow::glawbluda);
    connect (ui->dobavit_2,&QPushButton::clicked,this, &MainWindow::knopkadob);
    connect (ui->qpb_save,&QPushButton::clicked,this, &MainWindow::addNewFood);
    connect(ui->sort, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::loadOptions);
    connect(ui->poisk, &QLineEdit::textChanged, this, &MainWindow::loadOptions);
    connect (ui->exit_2,&QPushButton::clicked,this, &MainWindow::cancelAddproduct);
    connect (ui->exit_3,&QPushButton::clicked,this, &MainWindow::food_close);
    connect (ui->exit,&QPushButton::clicked,this, &MainWindow::product_close);
    connect (ui->qpb_newProv,&QPushButton::clicked,this, &MainWindow::showProvider);
    connect (ui->exit_5,&QPushButton::clicked,this, &MainWindow::provider_close);
    connect (ui->qpb_addProvider,&QPushButton::clicked,this, &MainWindow::addProvider);
    connect (ui->exit_4,&QPushButton::clicked,this, &MainWindow::addProvider_close);
    connect (ui->qpb_addProviderDb,&QPushButton::clicked,this, &MainWindow::addProviderDB);
    connect(ui->tw_bluda, &QTableWidget::cellClicked, this, &MainWindow::TableWidgetRowClicked1);
    connect (ui->exit,&QPushButton::clicked,this, &MainWindow::closeUpdateProduct);
    connect (ui->obnovit,&QPushButton::clicked,this, &MainWindow::updateProduct);
    //ui->rb_up->setChecked(true);

    loadtotable();
    loadSearchOptions();
    loadSortOptions();
    drfghj();
    fillProductsTable();
    loadProvToCB();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeUpdateProduct()
{
    ui->new_product->setGeometry(ui->new_product->x(),1000,ui->new_product->width(),ui-> new_product->height());
    ui->main_frame->setGeometry(ui->main_frame->x(),0,ui->main_frame->width(),ui-> main_frame->height());
}

void MainWindow::loadProvToCB()
{
    db = QSqlDatabase::database("qwerty");
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        ui->comboBox_2->clear();
        ui->comboBox_2->addItem("");
        query.prepare("SELECT id, name FROM providers");
        if(query.exec())
        {
            while (query.next()) {
                int subjectId = query.value(0).toInt();
                QString subjectName = query.value(1).toString();
                ui->comboBox_2->addItem(subjectName, subjectId);
            }
        }
    }
}

void MainWindow::glavMenu()
{
    ui->new_product->setGeometry(ui->new_product->x(),114,ui->new_product->width(),ui-> new_product->height());
    ui->main_frame->setGeometry(ui->main_frame->x(),1000,ui->main_frame->width(),ui-> main_frame->height());
    ui->dobavit->setVisible(true);
    ui->obnovit->setVisible(false);
    ui->pushButton_2->setVisible(false);
}

void MainWindow::product_close()
{
    ui->new_product->setGeometry(ui->new_product->x(),1000,ui->new_product->width(),ui-> new_product->height());
    ui->main_frame->setGeometry(ui->main_frame->x(),0,ui->main_frame->width(),ui-> main_frame->height());
}

void MainWindow::insertData()
{
    if (getDBConnection(db))
    {
        QSqlQuery query(db);
        QString name_product = ui->lineEdit->text();
        bool ok;
        int price_product = ui->lineEdit_2->text().toInt(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "Ошибка", "Некорректное значение цены");
            return;
        }
        QDate date_product = QDate::fromString(ui->lineEdit_3->text(), "yyyy-MM-dd");
        if (!date_product.isValid())
        {
            QMessageBox::warning(this, "Ошибка", "Некорректная дата производства");
            return;
        }
        QDate expir_product = QDate::fromString(ui->lineEdit_4->text(), "yyyy-MM-dd");
        if (!expir_product.isValid())
        {
            QMessageBox::warning(this, "Ошибка", "Некорректная дата истечения срока");
            return;
        }
        int provider_product = ui->comboBox_2->currentData().toInt();
        int remain_product = ui->lineEdit_6->text().toInt(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "Ошибка", "Некорректное значение остатка");
            return;
        }

        if (name_product.isEmpty() ||
            ui->lineEdit_2->text().isEmpty() ||
            ui->lineEdit_3->text().isEmpty() ||
            ui->lineEdit_4->text().isEmpty() ||
            ui->comboBox_2->currentIndex() == 0 || // Проверка, что выбран не пустой элемент
            ui->lineEdit_6->text().isEmpty())
        {
            QMessageBox::warning(this, "Ошибка", "Не все поля заполнены");
            return;
        }

        query.prepare("INSERT INTO product (name_product, price_product, date_product, expir_product, provider_product, remain_product) "
                      "VALUES (:name, :price, :date, :expir, :provider, :remain)");
        query.bindValue(":name", name_product);
        query.bindValue(":price", price_product);
        query.bindValue(":date", date_product);
        query.bindValue(":expir", expir_product);
        query.bindValue(":provider", provider_product);
        query.bindValue(":remain", remain_product);

        if (query.exec())
        {
            ui->lineEdit->clear();
            ui->lineEdit_2->clear();
            ui->lineEdit_3->clear();
            ui->lineEdit_4->clear();
            ui->comboBox_2->setCurrentIndex(0); // Сбрасываем выбор
            ui->lineEdit_6->clear();
            ui->new_product->setGeometry(ui->new_product->x(), 1000, ui->new_product->width(), ui->new_product->height());
            ui->main_frame->setGeometry(ui->main_frame->x(), 0, ui->main_frame->width(), ui->main_frame->height());

            loadtotable();
            fillProductsTable();

            currentID = -1; // Очистка значения currentID
        }
        else
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось выполнить запрос к базе данных.");
        }
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Нет подключения к базе данных");
    }
}

void MainWindow::updateProduct()
{
    if (getDBConnection(db))
    {
        QSqlQuery query(db);
        QString name_product = ui->lineEdit->text();
        bool ok;
        int price_product = ui->lineEdit_2->text().toInt(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "Ошибка", "Некорректное значение цены");
            return;
        }
        QDate date_product = QDate::fromString(ui->lineEdit_3->text(), "yyyy-MM-dd");
        if (!date_product.isValid())
        {
            QMessageBox::warning(this, "Ошибка", "Некорректная дата производства");
            return;
        }
        QDate expir_product = QDate::fromString(ui->lineEdit_4->text(), "yyyy-MM-dd");
        if (!expir_product.isValid())
        {
            QMessageBox::warning(this, "Ошибка", "Некорректная дата истечения срока");
            return;
        }
        int provider_product = ui->comboBox_2->currentData().toInt();
        int remain_product = ui->lineEdit_6->text().toInt(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "Ошибка", "Некорректное значение остатка");
            return;
        }

        if (currentID == -1 || // Проверка, что идентификатор продукта установлен
            name_product.isEmpty() ||
            ui->lineEdit_2->text().isEmpty() ||
            ui->lineEdit_3->text().isEmpty() ||
            ui->lineEdit_4->text().isEmpty() ||
            ui->comboBox_2->currentIndex() == 0 || // Проверка, что выбран не пустой элемент
            ui->lineEdit_6->text().isEmpty())
        {
            QMessageBox::warning(this, "Ошибка", "Не все поля заполнены");
            return;
        }

        qDebug()<<name_product;
        qDebug()<<price_product;
        qDebug()<<date_product;
        qDebug()<<expir_product;
        qDebug()<<provider_product;
        qDebug()<<remain_product;

        query.prepare("UPDATE product SET name_product = :name, price_product = :price, date_product = :date, expir_product = :expir, provider_product = :provider, remain_product = :remain WHERE id = :id");
        query.bindValue(":name", name_product);
        query.bindValue(":price", price_product);
        query.bindValue(":date", date_product);
        query.bindValue(":expir", expir_product);
        query.bindValue(":provider", provider_product);
        query.bindValue(":remain", remain_product);
        query.bindValue(":id", currentID);

        if (query.exec())
        {
            ui->lineEdit->clear();
            ui->lineEdit_2->clear();
            ui->lineEdit_3->clear();
            ui->lineEdit_4->clear();
            ui->comboBox_2->setCurrentIndex(0); // Сбрасываем выбор
            ui->lineEdit_6->clear();
            ui->new_product->setGeometry(ui->new_product->x(), 1000, ui->new_product->width(), ui->new_product->height());
            ui->main_frame->setGeometry(ui->main_frame->x(), 0, ui->main_frame->width(), ui->main_frame->height());

            loadtotable();
            fillProductsTable();

            currentID = -1; // Очистка значения currentID
        }
        else
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось выполнить запрос к базе данных.");
        }
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Нет подключения к базе данных");
    }
}

void MainWindow::loadSortOptions()
{
    ui->cb_sort->clear();
    ui->cb_sort->addItem("");
    int columnCount = ui->tableWidget->columnCount();
    for (int i = 0; i < columnCount; ++i)
    {
        QString columnName = ui->tableWidget->horizontalHeaderItem(i)->text();
        ui->cb_sort->addItem(columnName);
    }
}

void MainWindow::sortTableByColumn()
{
    QString selectedColumn = ui->cb_sort->currentText();
    if (selectedColumn.isEmpty())
    {
        loadtotable();
        return;
    }

    int columnIndex = -1;
    int columnCount = ui->tableWidget->columnCount();
    for (int i = 0; i < columnCount; ++i)
    {
        QString columnName = ui->tableWidget->horizontalHeaderItem(i)->text();
        if (columnName == selectedColumn)
        {
            columnIndex = i;
            break;
        }
    }

    if (columnIndex != -1)
    {
        Qt::SortOrder order = ui->rb_up->isChecked() ? Qt::AscendingOrder : Qt::DescendingOrder;
        ui->tableWidget->setSortingEnabled(false);
        for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
        {
            QTableWidgetItem *item = ui->tableWidget->item(row, columnIndex);
            if (columnIndex == 2 || columnIndex == 6)
            {
                item->setData(Qt::DisplayRole, item->data(Qt::EditRole).toInt());
            }
        }
        ui->tableWidget->setSortingEnabled(true);
        ui->tableWidget->sortItems(columnIndex, order);
    }
}


void MainWindow::loadSearchOptions()
{
    ui->comboBox->clear();
    ui->comboBox->addItem("");
    ui->comboBox->addItem("По номеру товара");
    ui->comboBox->addItem("По названию товара");
    ui->comboBox->addItem("По цене");
    ui->comboBox->addItem("По дате поступления");
    ui->comboBox->addItem("По сроку годности");
    ui->comboBox->addItem("По поставщику");
    ui->comboBox->addItem("По количеству");
}

void MainWindow::filterTableBySearch()
{
    QString searchOption = ui->comboBox->currentText();
    QString searchText = ui->lineEdit_7->text();
    ui->tableWidget->setRowCount(0);

    if (searchOption.isEmpty() || searchText.isEmpty())
    {
        loadtotable();
        return;
    }

    QString queryStr = "SELECT id, name_product, price_product, date_product, expir_product, provider_product, remain_product FROM product WHERE ";

    if (searchOption == "По номеру товара")
    {
        queryStr += "id LIKE ?";
    }
    else if (searchOption == "По названию товара")
    {
        queryStr += "name_product LIKE ?";
    }
    else if (searchOption == "По цене")
    {
        queryStr += "price_product LIKE ?";
    }
    else if (searchOption == "По дате поступления")
    {
        queryStr += "date_product LIKE ?";
    }
    else if (searchOption == "По сроку годности")
    {
        queryStr += "expir_product LIKE ?";
    }
    else if (searchOption == "По поставщику")
    {
        queryStr += "provider_product LIKE ?";
    }
    else if (searchOption == "По количеству")
    {
        queryStr += "remain_product LIKE ?";
    }

    if (getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare(queryStr);
        query.addBindValue("%" + searchText + "%");

        query.exec();

        int row = 0;
        while (query.next())
        {
            ui->tableWidget->insertRow(row);
            int product = query.value(0).toInt();
            QString name_product = query.value(1).toString();
            QString price_product = query.value(2).toString();
            QString date_product = query.value(3).toString();
            QString expir_product = query.value(4).toString();
            QString provider_product = query.value(5).toString();
            QString remain_product = query.value(6).toString();

            QTableWidgetItem *productItem = new QTableWidgetItem(QString::number(product));
            QTableWidgetItem *name_productItem = new QTableWidgetItem(name_product);
            QTableWidgetItem *price_productItem = new QTableWidgetItem(price_product);
            QTableWidgetItem *date_productItem = new QTableWidgetItem(date_product);
            QTableWidgetItem *expir_productItem = new QTableWidgetItem(expir_product);
            QTableWidgetItem *provider_productItem = new QTableWidgetItem(provider_product);
            QTableWidgetItem *remain_productItem = new QTableWidgetItem(remain_product);

            ui->tableWidget->setItem(row, 0, productItem);
            ui->tableWidget->setItem(row, 1, name_productItem);
            ui->tableWidget->setItem(row, 2, price_productItem);
            ui->tableWidget->setItem(row, 3, date_productItem);
            ui->tableWidget->setItem(row, 4, expir_productItem);
            ui->tableWidget->setItem(row, 5, provider_productItem);
            ui->tableWidget->setItem(row, 6, remain_productItem);
            row++;
        }
    }
}

void MainWindow::loadtotable()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    if(getDBConnection(db))
    {
        int row = 0;
        QSqlQuery query(db);
        query.prepare("SELECT product.id, product.name_product, product.price_product, product.date_product, product.expir_product, providers.name, product.remain_product "
                      "from product "
                      "INNER JOIN providers on product.provider_product = providers.id");
        if (query.exec())
        {
            while(query.next())
            {
                ui->tableWidget->insertRow(row);
                int product = query.value(0).toInt();
                QString name_product = query.value(1).toString();
                int price_product = query.value(2).toInt();
                QString date_product = query.value(3).toString();
                QString expir_product = query.value(4).toString();
                QString provider_product = query.value(5).toString();
                int remain_product = query.value(6).toInt();

                QTableWidgetItem *productItem = new QTableWidgetItem(QString::number(product));
                QTableWidgetItem *name_productItem = new QTableWidgetItem(name_product);
                QTableWidgetItem *price_productItem = new QTableWidgetItem(QString::number(price_product));
                QTableWidgetItem *date_productItem = new QTableWidgetItem(date_product);
                QTableWidgetItem *expir_productItem = new QTableWidgetItem(expir_product);
                QTableWidgetItem *provider_productItem = new QTableWidgetItem(provider_product);
                QTableWidgetItem *remain_productItem = new QTableWidgetItem(QString::number(remain_product));

                productItem->setData(Qt::EditRole, product);
                price_productItem->setData(Qt::EditRole, price_product);
                remain_productItem->setData(Qt::EditRole, remain_product);

                ui->tableWidget->setItem(row, 0, productItem);
                ui->tableWidget->setItem(row, 1, name_productItem);
                ui->tableWidget->setItem(row, 2, price_productItem);
                ui->tableWidget->setItem(row, 3, date_productItem);
                ui->tableWidget->setItem(row, 4, expir_productItem);
                ui->tableWidget->setItem(row, 5, provider_productItem);
                ui->tableWidget->setItem(row, 6, remain_productItem);
                row++;
            }
        }
    }
}

void MainWindow::TableWidgetRowClicked(int row, int column)
{
    QTableWidgetItem *item = ui->tableWidget->item(row, 0);
    if (item)
    {
        int product = item->text().toInt();
        loadCartDetails(product);
        currentID = -1;
        currentID = product;
    }
}

void MainWindow::TableWidgetRowClicked1(int row, int column)
{
    QTableWidgetItem *item = ui->tw_bluda->item(row, 0);
    if (item)
    {
        int product = item->text().toInt();
        loadFoodDetails(product);
    }
}

void MainWindow::loadFoodDetails(int product)
{
    if (getDBConnection(db))
    {
        // Загружаем данные из таблицы food
        QSqlQuery foodQuery(db);
        foodQuery.prepare("SELECT name_food, recipe_food, category_id FROM food WHERE id = ?;");
        foodQuery.addBindValue(product);
        if (foodQuery.exec() && foodQuery.next())
        {
            QString name_food = foodQuery.value(0).toString();
            QString recipe_food = foodQuery.value(1).toString();
            int category_id = foodQuery.value(2).toInt();

            // Устанавливаем текст в label_20 и plainTextEdit
            ui->label_20->setText(name_food);
            ui->plainTextEdit->setPlainText(recipe_food);

            // Загружаем данные из таблицы category
            QSqlQuery categoryQuery(db);
            categoryQuery.prepare("SELECT name_category FROM category WHERE id = ?;");
            categoryQuery.addBindValue(category_id);
            if (categoryQuery.exec() && categoryQuery.next())
            {
                QString name_category = categoryQuery.value(0).toString();
                // Устанавливаем текст в label_22
                ui->label_22->setText(name_category);
            }
            else
            {
                // Обработка ошибки, если не удалось найти категорию
                ui->label_22->setText("Категория не найдена");
            }
        }
        else
        {
            // Обработка ошибки, если не удалось найти блюдо
            ui->label_20->setText("Блюдо не найдено");
            ui->plainTextEdit->clear();
            ui->label_22->setText("");
        }
    }
    else
    {
        // Обработка ошибки соединения с базой данных
        QMessageBox::warning(this, "Ошибка", "Нет подключения к базе данных");
    }
}

void MainWindow::loadCartDetails(int product)
{
    // Установка геометрии элементов интерфейса
    ui->main_frame->setGeometry(ui->main_frame->x(), 1000, ui->main_frame->width(), ui->main_frame->height());
    ui->new_product->setGeometry(ui->new_product->x(), 114, ui->new_product->width(), ui->new_product->height());
    ui->pushButton_2->setGeometry(ui->pushButton_2->x(), 310, ui->pushButton_2->width(), ui->pushButton_2->height());
    ui->dobavit->setVisible(false);
    ui->obnovit->setVisible(true);
    ui->pushButton_2->setVisible(true);

    if (getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare("SELECT name_product, price_product, date_product, expir_product, provider_product, remain_product FROM product WHERE id = ?");
        query.addBindValue(product);
        if (query.exec() && query.next())
        {
            QString name_product = query.value(0).toString();
            QString price_product = query.value(1).toString();
            QString date_product = query.value(2).toString();
            QString expir_product = query.value(3).toString();
            QString provider_product = query.value(4).toString();
            QString remain_product = query.value(5).toString();

            ui->lineEdit->setText(name_product);
            ui->lineEdit_2->setText(price_product);
            ui->lineEdit_3->setText(date_product);
            ui->lineEdit_4->setText(expir_product);
            // ui->lineEdit_5->setText(provider_product); // Комментируем, т.к. не используется
            ui->lineEdit_6->setText(remain_product);

            // Устанавливаем соединение для кнопки сохранения
        }
        else
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось выполнить запрос к базе данных.");
        }
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Нет подключения к базе данных");
    }
}

void MainWindow::deleteRecord()
{
    int currentRow = ui->tableWidget->currentRow();
    int id = ui->tableWidget->item(currentRow, 0)->text().toInt();

    if (!getDBConnection(db))
    {
        QMessageBox::critical(this, "Ошибка", "Нет подключения к базе данных.");
        return;
    }

    // Начало транзакции
    if (!db.transaction())
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось начать транзакцию.");
        return;
    }
    QSqlQuery query(db);
    query.prepare("DELETE FROM food_product WHERE product_id = :id");
    query.bindValue(":id", id);
    if (!query.exec())
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось удалить связанные записи из таблицы food_product.");
        db.rollback(); // Откат транзакции в случае ошибки
        return;
    }
    // Удаление из таблицы product
    query.prepare("DELETE FROM product WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec())
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось удалить запись из таблицы product.");
        db.rollback(); // Откат транзакции в случае ошибки
        return;
    }

    // Удаление из таблицы food_product

    // Завершение транзакции
    if (!db.commit())
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось завершить транзакцию.");
        return;
    }

    // Успешное удаление
    ui->tableWidget->removeRow(currentRow);
    QMessageBox::information(this, "Успех", "Запись успешно удалена.");
    ui->new_product->setGeometry(ui->new_product->x(), 1000, ui->new_product->width(), ui->new_product->height());
    ui->main_frame->setGeometry(ui->main_frame->x(), 0, ui->main_frame->width(), ui->main_frame->height());
}

void MainWindow::glawbluda()
{
    ui->food->setGeometry(ui->food->x(),0,ui->food->width(),ui-> food->height());
    ui->main_frame->setGeometry(ui->main_frame->x(),1000,ui->main_frame->width(),ui-> main_frame->height());
}

void MainWindow::knopkadob()
{
    ui->new_food->setGeometry(ui->new_food->x(),84,ui->new_food->width(),ui-> new_food->height());
    ui->food->setGeometry(ui->food->x(),1000,ui->food->width(),ui->food->height());
    loadCategories();
}

void MainWindow::addNewFood()
{
    QList<QPair<QString, int>> selectedProducts = getSelectedProducts();
    QString name_food = ui->name_bludo->text();
    QString recipe_food = ui->recipe_bludo->toPlainText();
    int selectedCat = ui->cb_categories->currentData().toInt();

    if (ui->name_bludo->text().isEmpty() || ui->recipe_bludo->toPlainText().isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Не все поля заполнены");
        return;
    }

    if (getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare("INSERT INTO food (name_food, recipe_food, category_id) "
                      "VALUES (:name, :recipe, :cat)");
        query.bindValue(":name", name_food);
        query.bindValue(":recipe", recipe_food);
        query.bindValue(":cat", selectedCat);

        if (query.exec())
        {
            // Получаем ID нового блюда
            int food_id = query.lastInsertId().toInt();

            // Вставляем записи в таблицу food_product
            query.prepare("INSERT INTO food_product (food_id, product_id, product_count) "
                          "VALUES (:food_id, :product_id, :product_count)");

            for (const auto& product : selectedProducts)
            {
                query.bindValue(":food_id", food_id);
                query.bindValue(":product_id", product.first);
                query.bindValue(":product_count", product.second);
                query.exec();
            }

            // Очищаем поля ввода и скрываем форму
            ui->name_bludo->clear();
            ui->recipe_bludo->clear();
            ui->new_food->setGeometry(ui->new_food->x(), 1000, ui->new_food->width(), ui->new_food->height());
            ui->food->setGeometry(ui->food->x(), 0, ui->food->width(), ui->food->height());
            new_bludo();
        }
        else
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось добавить новое блюдо в базу данных");
        }
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Нет подключения к базе данных");
    }
}

void MainWindow::new_bludo()
{
    ui->tw_bluda->clearContents();
    ui->tw_bluda->setRowCount(0);
    if(getDBConnection(db))
    {
        int row = 0;
        QSqlQuery query(db);
        query.prepare("SELECT food.id, food.name_food, food.recipe_food, category.name_category "
                      "FROM food "
                      "INNER JOIN category on food.category_id = category.id");

        if (query.exec())
        {
            while(query.next())
            {
                ui->tw_bluda->insertRow(row);

                int food = query.value(0).toInt();
                QString name_food = query.value(1).toString();
                QString recipe_food = query.value(2).toString();
                QString cat_food = query.value(3).toString();

                QTableWidgetItem *foodItem = new QTableWidgetItem(QString::number(food));
                QTableWidgetItem *name_foodItem = new QTableWidgetItem(name_food);
                QTableWidgetItem *recipe_foodItem = new QTableWidgetItem(recipe_food);
                QTableWidgetItem *cat_foodItem = new QTableWidgetItem(cat_food);

                ui->tw_bluda->setItem(row, 0, foodItem);
                ui->tw_bluda->setItem(row, 1, name_foodItem);
                ui->tw_bluda->setItem(row, 2, cat_foodItem);
                ui->tw_bluda->setItem(row, 3, recipe_foodItem);
                row++;
            }
            ui->tw_bluda->hideColumn(0);
        }
    }
}

void MainWindow::drfghj()
{
    ui->sort->clear();
    ui->sort->addItem("");
    ui->sort->addItem("По номеру");
    ui->sort->addItem("По наименованию");
    ui->sort->addItem("По категориям");
}

void MainWindow::loadOptions()
{
    QString searchOption = ui->sort->currentText();
    QString searchText = ui->poisk->text();
    ui->tw_bluda->setRowCount(0);

    if (searchOption.isEmpty() || searchText.isEmpty())
    {
        new_bludo();
        return;
    }

    QString queryStr = "SELECT food.id, food.name_food, category.name_category, food.recipe_food "
                       "from food "
                       "inner join category on food.category_id = category.id where ";
    if (searchOption == "По номеру")
    {
        queryStr += "food.id LIKE ?";
    }
    else if (searchOption == "По наименованию")
    {
        queryStr += "food.name_food LIKE ?";
    }
    else if(searchOption == "По категориям")
    {
        queryStr += "category.name_category LIKE ?";
    }
    if (getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare(queryStr);

        if (searchOption == "По номеру")
        {
            query.addBindValue(searchText.toInt());
        }
        else
        {
            query.addBindValue("%" + searchText + "%");
        }
        query.exec();

        int row = 0;
        while (query.next())
        {
            ui->tw_bluda->insertRow(row);
            int food = query.value(0).toInt();
            QString name_food = query.value(1).toString();
            QString name_category = query.value(2).toString();
            QString recipe_food = query.value(3).toString();

            QTableWidgetItem *foodItem = new QTableWidgetItem(QString::number(food));
            QTableWidgetItem *name_foodItem = new QTableWidgetItem(name_food);
            QTableWidgetItem *name_foodCat = new QTableWidgetItem(name_category);
            QTableWidgetItem *recipe_foodItem = new QTableWidgetItem(recipe_food);

            ui->tw_bluda->setItem(row, 0, foodItem);
            ui->tw_bluda->setItem(row, 1, name_foodItem);
            ui->tw_bluda->setItem(row, 2, name_foodCat);
            ui->tw_bluda->setItem(row, 3, recipe_foodItem);
            row++;
        }
    }
}

void MainWindow::loadCategories()
{
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        ui->cb_categories->clear();
        query.prepare("select id, name_category from category");
        if(query.exec())
        {
            while(query.next())
            {
                int categoryID = query.value(0).toInt();
                QString categoryName = query.value(1).toString();
                ui->cb_categories->addItem(categoryName, categoryID);
            }
        }
    }
}

void MainWindow::cancelAddproduct()
{
    ui->new_food->setGeometry(ui->new_food->x(),1000,ui->new_food->width(),ui->new_food->height());
    ui->food->setGeometry(ui->food->x(),0,ui->food->width(),ui->food->height());
}

void MainWindow::food_close()
{
    ui->food->setGeometry(ui->food->x(),1000,ui->food->width(),ui->food->height());
    ui->main_frame->setGeometry(ui->main_frame->x(),0,ui->main_frame->width(),ui->main_frame->height());
}

void MainWindow::showProvider()
{
    ui->main_frame->setGeometry(ui->main_frame->x(),1000,ui->main_frame->width(),ui->main_frame->height());
    ui->provider->setGeometry(ui->provider->x(),0,ui->provider->width(),ui->provider->height());
    loadProviders();
}

void MainWindow::provider_close()
{
    ui->main_frame->setGeometry(ui->main_frame->x(),0,ui->main_frame->width(),ui->main_frame->height());
    ui->provider->setGeometry(ui->provider->x(),1000,ui->provider->width(),ui->provider->height());
}

void MainWindow::addProvider()
{
    ui->provider->setGeometry(ui->provider->x(),1000,ui->provider->width(),ui->provider->height());
    ui->addproviders->setGeometry(ui->addproviders->x(),0,ui->addproviders->width(),ui->addproviders->height());
}

void MainWindow::addProvider_close()
{
    ui->provider->setGeometry(ui->provider->x(),0,ui->provider->width(),ui->provider->height());
    ui->addproviders->setGeometry(ui->addproviders->x(),1000,ui->addproviders->width(),ui->addproviders->height());
}

void MainWindow::addProviderDB()
{
    if(getDBConnection(db))
    {
        QSqlQuery query(db);
        query.prepare("insert into providers (name, name_org, email, adress) values (?, ?, ?, ?)");
        query.addBindValue(ui->lineEdit_8->text());
        query.addBindValue(ui->lineEdit_9->text());
        query.addBindValue(ui->lineEdit_10->text());
        query.addBindValue(ui->lineEdit_11->text());
        if(query.exec())
        {
            QList<QMessageBox::Button> buttons = {QMessageBox::Ok};
            showMessage("Поставщик добавлен!", "", QMessageBox::Information, buttons);
            addProvider_close();
        }
    }
}

void MainWindow::loadProviders()
{
    if(getDBConnection(db))
    {
        int row = 0;
        ui->tableWidget_2->setRowCount(0);
        QSqlQuery query(db);
        query.prepare("select id, name, name_org, email, adress from providers");
        if (query.exec())
        {
            while(query.next())
            {
                ui->tableWidget_2->insertRow(row);

                int id = query.value(0).toInt();
                QString name = query.value(1).toString();
                QString name_org = query.value(2).toString();
                QString email = query.value(3).toString();
                QString adress = query.value(4).toString();

                QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(id));
                QTableWidgetItem *nameItem = new QTableWidgetItem(name);
                QTableWidgetItem *name_OrgItem = new QTableWidgetItem(name_org);
                QTableWidgetItem *emailItem = new QTableWidgetItem(email);
                QTableWidgetItem *adressItem = new QTableWidgetItem(adress);

                ui->tableWidget_2->setItem(row, 0, idItem);
                ui->tableWidget_2->setItem(row, 1, nameItem);
                ui->tableWidget_2->setItem(row, 2, name_OrgItem);
                ui->tableWidget_2->setItem(row, 3, emailItem);
                ui->tableWidget_2->setItem(row, 4, adressItem);
                row++;
            }
            ui->tableWidget_2->hideColumn(0);
        }
    }
}

void MainWindow::fillProductsTable()
{
    ui->tw_products->clearContents();
    ui->tw_products->setColumnHidden(1, true);
    ui->tw_products->setColumnWidth(0, 10);
    ui->tw_products->setColumnWidth(1, 20);
    ui->tw_products->setColumnWidth(2, 88);
    ui->tw_products->setColumnWidth(3, 50);
    db = QSqlDatabase::database("qwerty");
    if (getDBConnection(db))
    {
        QSqlQuery query(db);
        query.exec("SELECT id, name_product FROM product;");
        while (query.next())
        {
            ui->tw_products->insertRow(ui->tw_products->rowCount());
            QCheckBox *chkb = new QCheckBox();
            chkb->setCheckState(Qt::Unchecked);
            ui->tw_products->setCellWidget(ui->tw_products->rowCount() - 1, 0, chkb);
            ui->tw_products->setItem(ui->tw_products->rowCount() - 1, 1, new QTableWidgetItem(query.value(0).toString()));
            ui->tw_products->setItem(ui->tw_products->rowCount() - 1, 2, new QTableWidgetItem(query.value(1).toString()));
        }
        ui->tw_products->resizeRowsToContents();
    }
}

QList<QPair<QString, int>> MainWindow::getSelectedProducts()
{
    QList<QPair<QString, int>> selectedProducts;
    for (int i = 0; i < ui->tw_products->rowCount(); i++)
    {
        if (qobject_cast<QCheckBox*>(ui->tw_products->cellWidget(i, 0))->checkState() == Qt::Checked)
        {
            QString productId = ui->tw_products->item(i, 1)->text();
            int productCount = ui->tw_products->item(i, 3)->text().toInt();
            selectedProducts.append(qMakePair(productId, productCount));
        }
    }
    return selectedProducts;
}

void MainWindow::on_pushButton_3_clicked()
{
    int currentRow = ui->tw_bluda->currentRow();
    if (currentRow != -1)
    {
        QTableWidgetItem *item = ui->tw_bluda->item(currentRow, 0);
        if (item)
        {
            int food_id = item->text().toInt();
            checkAndConsumeIngredients(food_id);
        }
        else
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось получить ID блюда из таблицы.");
        }
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, выберите блюдо из таблицы.");
    }
}

void MainWindow::checkAndConsumeIngredients(int food_id)
{
    if (getDBConnection(db))
    {
        // Шаг 1: Получаем список ингредиентов и их количество для данного блюда
        QSqlQuery query(db);
        query.prepare("SELECT product_id, product_count FROM food_product WHERE food_id = ?;");
        query.addBindValue(food_id);

        if (!query.exec())
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось выполнить запрос к базе данных.");
            return;
        }

        QMap<int, int> requiredIngredients; // product_id -> product_count
        while (query.next())
        {
            int product_id = query.value(0).toInt();
            int product_count = query.value(1).toInt();
            requiredIngredients[product_id] = product_count;
        }

        // Шаг 2: Проверяем наличие ингредиентов на складе
        QMap<int, int> missingIngredients;
        QMap<int, int> currentStock;

        foreach (int product_id, requiredIngredients.keys())
        {
            QSqlQuery stockQuery(db);
            stockQuery.prepare("SELECT remain_product FROM product WHERE id = ?;");
            stockQuery.addBindValue(product_id);

            if (!stockQuery.exec() || !stockQuery.next())
            {
                QMessageBox::warning(this, "Ошибка", "Не удалось получить данные о наличии продукта на складе.");
                return;
            }

            int remain_product = stockQuery.value(0).toInt();
            currentStock[product_id] = remain_product;

            if (remain_product < requiredIngredients[product_id])
            {
                int missingCount = requiredIngredients[product_id] - remain_product;
                missingIngredients[product_id] = missingCount;
            }
        }

        // Шаг 3: Если какого-то ингредиента не хватает, выводим сообщение об ошибке
        if (!missingIngredients.isEmpty())
        {
            QString errorMessage = "Не хватает следующих ингредиентов:\n";
            foreach (int product_id, missingIngredients.keys())
            {
                errorMessage += QString("ID продукта: %1, нехватка: %2\n")
                                    .arg(product_id)
                                    .arg(missingIngredients[product_id]);
            }
            QMessageBox::warning(this, "Ошибка", errorMessage);
            return;
        }

        // Шаг 4: Если всех ингредиентов хватает, уменьшаем их количество на складе
        QSqlQuery updateQuery(db);
        foreach (int product_id, requiredIngredients.keys())
        {
            int new_remain_product = currentStock[product_id] - requiredIngredients[product_id];
            updateQuery.prepare("UPDATE product SET remain_product = ? WHERE id = ?;");
            updateQuery.addBindValue(new_remain_product);
            updateQuery.addBindValue(product_id);

            if (!updateQuery.exec())
            {
                QMessageBox::warning(this, "Ошибка", "Не удалось обновить данные о наличии продукта на складе для продукта с ID " + QString::number(product_id));
                return;
            }
        }

        QMessageBox::information(this, "Успех", "Ингредиенты успешно списаны со склада.");
        loadtotable();
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Нет подключения к базе данных");
    }
}
